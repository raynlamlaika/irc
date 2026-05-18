# IRC Back End

Spring Boot backend that bridges a web chat client to a C++ IRC server.

## What it exposes

- `POST /irc/connect`
- `POST /irc/disconnect`
- `GET /irc/status`
- `POST /irc/join`
- `POST /irc/message`
- `GET /irc/history`
- `GET /irc/events` as server-sent events

## Request examples

### Connect

```json
{
  "host": "127.0.0.1",
  "port": 6667,
  "password": "password",
  "nickname": "nickname",
  "realName": "Real Name"
}
```

### Join channel

```json
{
  "channel": "#general"
}
```

### Send message

```json
{
  "channel": "#general",
  "message": "hello from the web client"
}
```

## Run

```bash
./mvnw spring-boot:run
```

The backend is CORS-enabled for browser clients and streams IRC traffic through `/irc/events`.