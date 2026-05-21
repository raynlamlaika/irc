package com.irc.irc_back_end;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.InetSocketAddress;
import java.time.Instant;
import java.util.List;
import java.util.UUID;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.atomic.AtomicBoolean;

import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.NotBlank;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.http.MediaType;
import org.springframework.stereotype.Component;
import org.springframework.web.servlet.mvc.method.annotation.SseEmitter;

@Component
class IrcService {

    private static final Logger log = LoggerFactory.getLogger(IrcService.class);

    private final Object connectionLock = new Object();
    private final List<SseEmitter> emitters = new CopyOnWriteArrayList<>();
    private final List<IrcEvent> recentEvents = new CopyOnWriteArrayList<>();
    private final AtomicBoolean connected = new AtomicBoolean(false);

    private Socket socket;
    private PrintWriter out;
    private BufferedReader in;
    private Thread listenerThread;
    private String userId;
    private final AtomicBoolean connecting = new AtomicBoolean(false);

    public IrcConnectionStatus connect(IrcConnectionRequest request) throws IOException {
        if (connected.get()) {
            return status();
        }

        if (!connecting.compareAndSet(false, true)) {
            // another thread is already trying to connect — return current status so caller can retry/poll
            return status();
        }

        IrcConnectionStatus result;
        IrcEvent connectEvent = null;
        try {
            synchronized (connectionLock) {
                // ensure previous resources are cleaned up before opening a new socket
                disconnectInternal();

                // use a connect timeout to avoid blocking indefinitely
                socket = new Socket();
                socket.connect(new InetSocketAddress(request.host(), request.port()), 5000);

                out = new PrintWriter(socket.getOutputStream(), true);
                in = new BufferedReader(new InputStreamReader(socket.getInputStream()));

                if (request.password() != null && !request.password().isBlank()) {
                    out.println("PASS " + request.password());
                }
                out.println("NICK " + request.nickname());
                out.println("USER " + request.nickname() + " 0 * :" + request.realName());

                userId = UUID.randomUUID().toString();
                connected.set(true);
                startListenerThread();
                connectEvent = new IrcEvent("system", "Connected to IRC server", Instant.now());
                result = status();
            }
        } finally {
            connecting.set(false);
        }

        if (connectEvent != null) {
            publish(connectEvent);
        }

        return result;
    }

    public IrcConnectionStatus status() {
        return new IrcConnectionStatus(connected.get());
    }

    public void disconnect() {
        IrcEvent disconnectEvent;
        synchronized (connectionLock) {
            disconnectInternal();
            disconnectEvent = new IrcEvent("system", "Disconnected from IRC server", Instant.now());
        }

        publish(disconnectEvent);
    }

    public IrcEvent joinChannel(String channel)
    {
        ensureConnected();
        out.println("JOIN " + channel);

        IrcEvent event = new IrcEvent(currentUserId(), "JOIN " + channel, Instant.now());
        publish(event);
        return event;
    }

    public void sendMessage(String message, String target) {
        ensureConnected();
        out.println("PRIVMSG " + target + " :" + message);
        publish(new IrcEvent(currentUserId(), target + ": " + message, Instant.now()));
    }

    public IrcEvent connectJoinAndSend(IrcChannelMessageRequest request) throws IOException {
        connect(new IrcConnectionRequest(
            request.host(),
            request.port(),
            request.password(),
            request.nickname(),
            request.realName()));

        joinChannel(request.channel());
        sendMessage(request.message(), request.channel());

        return new IrcEvent(currentUserId(), request.channel() + ": " + request.message(), Instant.now());
    }

    public IrcHistoryResponse history() {
        return new IrcHistoryResponse(recentEvents);
    }

    public SseEmitter subscribe() {
        SseEmitter emitter = new SseEmitter(0L);
        emitters.add(emitter);

        emitter.onCompletion(() -> emitters.remove(emitter));
        emitter.onTimeout(() -> emitters.remove(emitter));
        emitter.onError((error) -> emitters.remove(emitter));

        try {
            emitter.send(SseEmitter.event()
                .name("history")
                .data(recentEvents, MediaType.APPLICATION_JSON));
        } catch (IOException exception) {
            emitters.remove(emitter);
        }

        return emitter;
    }

    private void ensureConnected() {
        if (!connected.get() || out == null) {
            throw new IllegalStateException("Not connected to an IRC server");
        }
    }

    private void startListenerThread() {
        listenerThread = new Thread(() -> {
            try {
                String line;
                while ((line = in.readLine()) != null) {
                    if (line.startsWith("PING ")) {
                        out.println("PONG " + line.substring(5));
                    }
                    publish(new IrcEvent("server", line, Instant.now()));
                }
            } catch (IOException exception) {
                if (connected.get()) {
                    log.warn("IRC listener stopped: {}", exception.getMessage());
                    publish(new IrcEvent("system", "IRC connection closed", Instant.now()));
                }
            } finally {
                connected.set(false);
            }
        }, "irc-listener");
        listenerThread.setDaemon(true);
        listenerThread.start();
    }

    private void disconnectInternal() {
        connected.set(false);

        if (listenerThread != null) {
            listenerThread.interrupt();
            listenerThread = null;
        }

        closeQuietly(in);
        closeQuietly(out);
        closeQuietly(socket);

        in = null;
        out = null;
        socket = null;
        userId = null;
    }

    private String currentUserId() {
        return userId != null ? userId : "unknown";
    }

    private void publish(IrcEvent event) {
        recentEvents.add(event);
        if (recentEvents.size() > 100) {
            recentEvents.remove(0);
        }

        for (SseEmitter emitter : emitters) {
            try {
                emitter.send(SseEmitter.event()
                    .name("irc-event")
                    .data(event, MediaType.APPLICATION_JSON));
            } catch (IOException exception) {
                emitters.remove(emitter);
            }
        }
    }

    private void closeQuietly(AutoCloseable closeable) {
        if (closeable == null) {
            return;
        }

        try {
            closeable.close();
        } catch (Exception ignored) {
        }
    }
}

record IrcConnectionRequest(@NotBlank String host, @Min(1) int port, String password, @NotBlank String nickname, @NotBlank String realName) {
}

record IrcConnectionStatus(boolean connected) {
}

record IrcChannelRequest(@NotBlank String channel) {
}

record IrcMessageRequest(@NotBlank String channel, @NotBlank String message) {
}

record IrcChannelMessageRequest(
    @NotBlank String host,
    @Min(1) int port,
    String password,
    @NotBlank String nickname,
    @NotBlank String realName,
    @NotBlank String channel,
    @NotBlank String message) {
}

record IrcEvent(String source, String message, Instant timestamp) {
}

record IrcHistoryResponse(List<IrcEvent> events) {
}