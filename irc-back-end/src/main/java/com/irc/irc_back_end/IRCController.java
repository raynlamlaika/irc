package com.irc.irc_back_end;

import java.io.IOException;

import jakarta.validation.Valid;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.servlet.mvc.method.annotation.SseEmitter;


@RestController
@RequestMapping("/irc")
@CrossOrigin(origins = "*")
public class IRCController
{
    private final IrcService irc;

    public IRCController(IrcService irc)
    {
        this.irc = irc;
    }

    @PostMapping("/connect")
    public ResponseEntity<IrcConnectionStatus> connect(@Valid @RequestBody IrcConnectionRequest request)
    {
        try
        {
            return ResponseEntity.ok(irc.connect(request));
        }
        catch (IOException e)
        {
            return ResponseEntity.internalServerError().build();
        }
    }

    @PostMapping("/disconnect")
    public ResponseEntity<Void> disconnect()
    {
        irc.disconnect();
        return ResponseEntity.noContent().build();
    }

    @GetMapping("/status")
    public IrcConnectionStatus status()
    {
        return irc.status();
    }

    @PostMapping("/join")
    public ResponseEntity<IrcEvent> join(@Valid @RequestBody IrcChannelRequest request) {
        return ResponseEntity.ok(irc.joinChannel(request.channel()));
    }

    @PostMapping("/message")
    public ResponseEntity<Void> message(@Valid @RequestBody IrcMessageRequest request) {
        irc.sendMessage(request.message(), request.channel());
        return ResponseEntity.noContent().build();
    }

    @PostMapping("/connect-message")
    public ResponseEntity<IrcEvent> connectMessage(@Valid @RequestBody IrcChannelMessageRequest request)
    {
        try
        {
            return ResponseEntity.ok(irc.connectJoinAndSend(request));
        }
        catch (IOException e)
        {
            return ResponseEntity.internalServerError().build();
        }
    }

    @GetMapping(value = "/events", produces = "text/event-stream")
    public SseEmitter events() {
        return irc.subscribe();
    }

    @GetMapping("/history")
    public IrcHistoryResponse history() {
        return irc.history();
    }
}