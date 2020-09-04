# Autoroute

A project similar in spirit to [Autobahn](https://crossbar.io/autobahn/), the great test suite for WebSocket.

# Running

This is probably only working on my machine so far ...

1. The ws container needs to be built locally. Clone [IXWebSocket](https://github.com/machinezone/IXWebSocket), and from the top folder type `env DOCKER_REPO=local make docker`. Ultimately we will upload the ws container to a repository which allows anonymous access.
2. Enter one of the folders (cpp/ixwebsocket) and type `env DOCKER_REPO=local docker-compose up`.

```
ixwebsocket$ docker-compose up
WARNING: Found orphan containers (ixwebsocket_compile_1) for this project. If you removed or renamed this service in your compose file, you can run this command with the --remove-orphans flag to clean it up.
Recreating ixwebsocket_push_1 ... done
Recreating ixwebsocket_autoroute_1 ... done
Attaching to ixwebsocket_push_1, ixwebsocket_autoroute_1
autoroute_1  | Connecting to ws://push:8008...
autoroute_1  | [2020-09-03 01:58:48.006] [info] messages received: 0 per second 0 total
push_1       | [2020-09-03 01:58:47.592] [info] Listening on 0.0.0.0:8008
push_1       | [2020-09-03 01:58:48.010] [info] New connection
push_1       | [2020-09-03 01:58:48.010] [info] remote ip: 172.18.0.3
push_1       | [2020-09-03 01:58:48.010] [info] id: 0
push_1       | [2020-09-03 01:58:48.010] [info] Uri: /1000000
push_1       | [2020-09-03 01:58:48.010] [info] Headers:
push_1       | [2020-09-03 01:58:48.010] [info] Connection: Upgrade
push_1       | [2020-09-03 01:58:48.010] [info] Host: push:8008
push_1       | [2020-09-03 01:58:48.010] [info] sec-websocket-extensions: 
push_1       | [2020-09-03 01:58:48.010] [info] Sec-WebSocket-Key: b39g2AC08hHaeHfc3AAc9H==
push_1       | [2020-09-03 01:58:48.010] [info] Sec-WebSocket-Version: 13
push_1       | [2020-09-03 01:58:48.010] [info] Upgrade: websocket
push_1       | [2020-09-03 01:58:48.010] [info] User-Agent: ixwebsocket/10.3.3 linux ssl/mbedtls 2.16.6 zlib 1.2.11
autoroute_1  | [2020-09-03 01:58:48.010] [info] ws_autoroute: connected
autoroute_1  | [2020-09-03 01:58:48.010] [info] Uri: /1000000
autoroute_1  | [2020-09-03 01:58:48.010] [info] Headers:
autoroute_1  | [2020-09-03 01:58:48.011] [info] Connection: Upgrade
autoroute_1  | [2020-09-03 01:58:48.011] [info] Sec-WebSocket-Accept: 2yd/rfELh+bkgEs5pn3pNE5ayag=
autoroute_1  | [2020-09-03 01:58:48.011] [info] Server: ixwebsocket/10.3.3 linux ssl/mbedtls 2.16.6 zlib 1.2.11
autoroute_1  | [2020-09-03 01:58:48.011] [info] Upgrade: websocket
autoroute_1  | [2020-09-03 01:58:49.006] [info] messages received: 118319 per second 118322 total
autoroute_1  | [2020-09-03 01:58:50.007] [info] messages received: 119008 per second 237334 total
autoroute_1  | [2020-09-03 01:58:51.007] [info] messages received: 110385 per second 347719 total
autoroute_1  | [2020-09-03 01:58:52.007] [info] messages received: 118564 per second 466283 total
autoroute_1  | [2020-09-03 01:58:53.007] [info] messages received: 116756 per second 583039 total
autoroute_1  | [2020-09-03 01:58:54.008] [info] messages received: 120838 per second 703879 total
autoroute_1  | [2020-09-03 01:58:55.008] [info] messages received: 120765 per second 824647 total
autoroute_1  | [2020-09-03 01:58:56.008] [info] messages received: 123100 per second 947758 total
autoroute_1  | [2020-09-03 01:58:56.433] [info] Received pong ixwebsocket::heartbeat::30s::0
autoroute_1  | [2020-09-03 01:58:57.010] [info] messages received: 52241 per second 1000000 total
... it should stop here but does not right now because of a bug ...
```

# Languages

* C / C++
* Python
* Node.js
* Ruby
* C#
* Swift (no docker yet)

Others I would like to try:

* Go
* Java
* Scala
* Rust
* Ziglang
* C++ (boost beast + websocketpp library)
