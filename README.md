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
// node/autoroute.js
//
// With ws@7.3.1
// and 
// node --version
// v13.11.0
//
// In a different terminal, start a push server:
// $ ws push_server
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

# Missing languages

Others I would like to try:

* Java
* Scala
* Ziglang
* C++ (boost beast + websocketpp library)
