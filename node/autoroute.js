//
// With ws@7.3.1
// and 
// node --version
// v13.11.0
//
// In a different terminal, start a push server:
// $ ws push_server -q
//
// $ node devnull_client.js
// messages received per second: 16643
// messages received per second: 28065
// messages received per second: 28432
// messages received per second: 22207
// messages received per second: 28805
// messages received per second: 28694
// messages received per second: 28180
// messages received per second: 28601
// messages received per second: 28698
// messages received per second: 28931
// messages received per second: 27975
//
const WebSocket = require('ws');

if (process.argv.length < 3) {
  console.log('Needs url and msg count as parameters');
  process.exit(1);
}

let url = process.argv[2];
let msgCount = process.argv[3];
msgCount = parseInt(msgCount);
let target = msgCount;

url += `/${msgCount}`
const ws = new WebSocket(url)

ws.on('open', function open() {
  console.log('connected');
});

ws.on('close', function open() {
  console.log('remote connection closed');
  process.exit();
});

var receivedMessages = 0;

setInterval(function timeout() {
  console.log(`messages received per second: ${receivedMessages}`)
  receivedMessages = 0;
}, 1000);

ws.on('message', function incoming(data) {
  receivedMessages += 1;

  target -= 1;
  if (target == 0) {
    console.log(`received all messages (${msgCount})`);
    process.exit();
  }
});
