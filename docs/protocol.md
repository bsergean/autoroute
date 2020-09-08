# Protocol

The protocol to run the test is very simple, our goal being that writing a new client to benchmark your library/language is easy.

1. A test server needs to run.
2. The client connect to the test server with a url: ws://test-server:port/N. N is the number of messages that the client requests the server to send.
3. Every second, the client should display how many messages it was able to receive. Messages are sent in TEXT mode.
4. When the N messages have been received, the client should close the connection, and display statistics.

# Demo

## Server

```
$ ws push_server
[2020-09-03 19:13:00.093] [info] Listening on 127.0.0.1:8008
[2020-09-03 19:13:02.244] [info] New connection
[2020-09-03 19:13:02.244] [info] remote ip: 127.0.0.1
[2020-09-03 19:13:02.244] [info] id: 0
[2020-09-03 19:13:02.244] [info] Uri: /10
[2020-09-03 19:13:02.244] [info] Headers:
[2020-09-03 19:13:02.244] [info] Connection: Upgrade
[2020-09-03 19:13:02.244] [info] Host: localhost:8008
[2020-09-03 19:13:02.244] [info] Sec-WebSocket-Extensions: permessage-deflate; server_max_window_bits=15; client_max_window_bits=15
[2020-09-03 19:13:02.244] [info] Sec-WebSocket-Key: 08B97CBH3dafEh5805b3D3==
[2020-09-03 19:13:02.244] [info] Sec-WebSocket-Version: 13
[2020-09-03 19:13:02.244] [info] Upgrade: websocket
[2020-09-03 19:13:02.244] [info] User-Agent: ixwebsocket/10.3.3 macos ssl/SecureTransport zlib 1.2.11
```

## Client

To illustrate what is going on, we will use a simple client that display every messages receive. ws provides such a client with its connect subcommand. There are many websocket cli that can be used as well. wscat is a node.js one.

Here we request 10 messages, by connecting to *ws://localhost:8008/10*.

```
$ ws connect ws://localhost:8008/10
Type Ctrl-D to exit prompt...
Connecting to url: ws://localhost:8008/10
> [2020-09-03 19:13:02.245] [info] ws_connect: connected
[2020-09-03 19:13:02.245] [info] Uri: /10
[2020-09-03 19:13:02.245] [info] Headers:
[2020-09-03 19:13:02.245] [info] Connection: Upgrade
[2020-09-03 19:13:02.245] [info] Sec-WebSocket-Accept: QSSQuryFa/YA3e3TXyHC/vIvF68=
[2020-09-03 19:13:02.245] [info] Sec-WebSocket-Extensions: permessage-deflate; server_max_window_bits=15; client_max_window_bits=15
[2020-09-03 19:13:02.245] [info] Server: ixwebsocket/10.3.3 macos ssl/SecureTransport zlib 1.2.11
[2020-09-03 19:13:02.245] [info] Upgrade: websocket
[2020-09-03 19:13:02.245] [info] Received 13 bytes
ws_connect: received message: hello world
[2020-09-03 19:13:02.245] [info] Received 5 bytes
ws_connect: received message: hello world
[2020-09-03 19:13:02.245] [info] Received 4 bytes
ws_connect: received message: hello world
[2020-09-03 19:13:02.245] [info] Received 4 bytes
ws_connect: received message: hello world
[2020-09-03 19:13:02.245] [info] Received 4 bytes
ws_connect: received message: hello world
[2020-09-03 19:13:02.245] [info] Received 4 bytes
ws_connect: received message: hello world
[2020-09-03 19:13:02.245] [info] Received 4 bytes
ws_connect: received message: hello world
[2020-09-03 19:13:02.245] [info] Received 4 bytes
ws_connect: received message: hello world
[2020-09-03 19:13:02.245] [info] Received 4 bytes
ws_connect: received message: hello world
[2020-09-03 19:13:02.245] [info] Received 4 bytes
ws_connect: received message: hello world
[2020-09-03 19:13:02.245] [info] Received pong ixwebsocket::heartbeat::30s::0
```
