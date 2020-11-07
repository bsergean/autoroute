# Autoroute

A project similar in spirit to [Autobahn](https://crossbar.io/autobahn/), the great test suite for WebSocket. Autobahn and autoroute means freeway in German and French, respectively.

The goal of this project is to measure how fast a websocket client can receive N messages sent from a websocket server, with various languages and library.

# Language matrix

| Language  | Library                  |
|-----------|--------------------------|
| C         | libwebsockets [1]        |
| C++       | IXWebSocket [2]          |
| C++       | websocketpp Beast [3]    |
| C++       | Boost Beast [2]          |
| Ruby      | faye-websocket [3]       |
| Python    | websockets [4]           |
| Python    | websocket_client [5]     |
| Node.js   | ws [6]                   |
| C#        | System.Net.WebSockets [7]|
| Go        | nhooyr.io [8]            |
| Rust      | tungstenite [9]          |

[1]: https://libwebsockets.org/
[2]: https://github.com/machinezone/IXWebSocket
[3]: https://github.com/zaphoyd/websocketpp
[4]: https://github.com/boostorg/beast
[5]: https://github.com/faye/faye-websocket-ruby
[6]: https://github.com/aaugustin/websockets
[7]: https://pypi.org/project/websocket_client/
[8]: https://github.com/websockets/ws
[9]: https://docs.microsoft.com/en-us/dotnet/api/system.net.websockets.websocket
[10]: https://github.com/nhooyr/websocket
[11]: https://github.com/snapview/tungstenite-rs

# Running

With docker installed and running, enter one of the folders (cpp/ixwebsocket, go, csharp, etc...) and type `make`. Or to run it for all runtimes, run make this from the top level folder.

![picture](recording.svg)

The javascript/node autoroute file is relatively simple and helps understanding how to write a new bench for a given language.

```javascript
//
// In a different terminal, start a push server
// $ docker run -p8008:8008 --init -it bsergean/ws:10.3.4 push_server --host 0.0.0.0
//
// or if you have ws installed locally
// $ ws push_server 
// 
//
// $ node autoroute.js ws://localhost:8008 1000000
// messages received per second: 16643
// messages received per second: 28065
// messages received per second: 28432
// messages received per second: 22207
// ...
//
const WebSocket = require('ws');

// Parse input parameters, url + number of message to receive
if (process.argv.length < 3) {
  console.log('Needs url and msg count as parameters');
  process.exit(1);
}

let url = process.argv[2];
let msgCount = process.argv[3];
msgCount = parseInt(msgCount);
let target = msgCount;

// Build the url
url += `/${msgCount}`

// Create a websocket connection, with the zlib extension disabled
const ws = new WebSocket(url, {perMessageDeflate: false})

// When the connection is established, record a timestamp
ws.on('open', function open() {
  startTime = new Date();
  console.log('connected');
});

// Every second, print how many messages we did receive
var receivedMessages = 0;

// Use a timer to print something every second
setInterval(function timeout() {
  console.log(`messages received per second: ${receivedMessages}`)
  receivedMessages = 0;
}, 1000);

// Each time we receive a message, check whether we did receive all
// of them. If this is the case, compute the total run duration and exit
ws.on('message', function incoming(data) {
  receivedMessages += 1;

  target -= 1;
  if (target == 0) {
    endTime = new Date();
    let duration = endTime - startTime; //in ms
    console.log(`AUTOROUTE Python websockets :: ${duration} ms`)
    process.exit();
  }
});
```

# Results

.Net is the winner ... by a big margin, I wonder how they do this.
Otherwise in the grand scheme of things everyone is in the same ballpark,
except websocket_client library which is a bit slower. I'm tempted to say, use
whatever language you like the best :)

```
autoroute_1  | AUTOROUTE Python websocket_client :: 39252 ms
autoroute_1  | AUTOROUTE Python websockets :: 15254 ms
autoroute_1  | AUTOROUTE rust :: 11.20s
autoroute_1  | [2020-11-07 19:03:27.925] [info] AUTOROUTE IXWebSocket :: 9585 ms
autoroute_1  | AUTOROUTE libwebsockets :: 10159 ms
autoroute_1  | AUTOROUTE C++ beast :: 12005 ms
autoroute_1  | AUTOROUTE C++ websocketpp :: 9813 ms
autoroute_1  | AUTOROUTE .Net :: 1814.1672 ms
autoroute_1  | AUTOROUTE node.js :: 8538 ms
autoroute_1  | "AUTOROUTE Ruby :: 13326 ms"
```

# Missing languages

Others I would like to try:

* Java
* Scala
* Ziglang
