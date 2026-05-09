# ft_irc

A simple Internet Relay Chat (IRC) server written in C++98 for the 42 Network curriculum.
This project recreates the core behavior of an IRC server using sockets, non-blocking I/O, and multiplexing with `poll()`.

---

## Features

* Multiple client connections
* Non-blocking sockets
* I/O multiplexing using `poll()`
* Authentication with password
* Nickname and username management
* Channel creation and management
* Private messages
* Channel operators
* IRC commands implementation
* Compatible with IRC clients like `LimeChat` and `nc`

---

## Technologies

* C++98
* BSD sockets
* TCP/IP
* `poll()`
* UNIX system calls

---

## Compilation

```bash
make
```

This will generate:

```bash
./ircserv
```

---

## Usage

```bash
./ircserv <port> <password>
```

Example:

```bash
./ircserv 6667 mypassword
```

---

## Connecting with netcat

```bash
nc localhost 6667
```

Then authenticate:

```bash
PASS mypassword
NICK hamza
USER hamza 0 * :Hamza
```

---

## Implemented Commands

| Command | Description                    |
| ------- | ------------------------------ |
| PASS    | Server password authentication |
| NICK    | Set nickname                   |
| USER    | Set username                   |
| JOIN    | Join/create channel            |
| PART    | Leave channel                  |
| PRIVMSG | Send private message           |
| KICK    | Kick user from channel         |
| INVITE  | Invite user                    |
| TOPIC   | Change/view topic              |
| MODE    | Manage channel modes           |
| QUIT    | Disconnect                     |

---

## Channel Modes

| Mode | Description                     |
| ---- | ------------------------------- |
| +i   | Invite-only channel             |
| +t   | Only operators can change topic |
| +k   | Channel password                |
| +o   | Give operator privileges        |
| +l   | User limit                      |

---

## Architecture

### Server

The server:

* Creates a TCP socket
* Binds it to a port
* Listens for incoming connections
* Uses `poll()` to manage multiple clients without threads or forks

### Client

Each connected client stores:

* Socket FD
* Nickname
* Username
* Authentication state
* Buffers

### Channel

Channels manage:

* Members
* Operators
* Topic
* Modes
* Invitations

---

## Non-blocking I/O

The server uses:

```cpp
fcntl(fd, F_SETFL, O_NONBLOCK);
```

This prevents blocking operations on:

* `accept()`
* `recv()`
* `send()`

---

## poll()

The main loop continuously monitors sockets:

```cpp
poll(pollfds.data(), pollfds.size(), -1);
```

This allows one thread to handle many clients efficiently.

---

## Error Handling

Handled cases include:

* Client disconnection
* Partial reads
* `EAGAIN`
* `EWOULDBLOCK`
* `SIGPIPE`
* Invalid commands
* Duplicate nicknames

---

## Example Workflow

```text
Client -> socket()
       -> connect()

Server -> socket()
       -> bind()
       -> listen()
       -> poll()
       -> accept()
       -> recv()
       -> send()
```

---

## Testing

Useful commands:

```bash
telnet localhost 6667
```

```bash
nc localhost 6667
```

Stress test:

```bash
yes | nc localhost 6667
```

---

## Learning Objectives

This project helps understand:

* TCP networking
* Socket programming
* IRC protocol basics
* Event-driven servers
* Multiplexing
* File descriptors
* System programming
* Network protocols

---

## Authors

* Hamza Khairi
* Rayane Lamlaika

---

## References

* [https://modern.ircdocs.horse/#errneedmoreparams-461](RFC 1459 (IRC Protocol))
* Linux man pages

```bash
man 2 socket
man 2 poll
man 2 recv
man 2 send
```

---

## License

This project is for educational purposes at 42 School.
