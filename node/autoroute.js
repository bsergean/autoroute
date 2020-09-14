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
