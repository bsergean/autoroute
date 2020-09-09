'''
create virtualenv
pip install websocket_client
start push server

messages received per second: 0
messages received per second: 38364
messages received per second: 41431
messages received per second: 41400
messages received per second: 40756
messages received per second: 40923
messages received per second: 40931
messages received per second: 41216
messages received per second: 41836
messages received per second: 41497
messages received per second: 41751
messages received per second: 41625
messages received per second: 41505
'''

import websocket
ws = websocket.WebSocket()

import websocket
import _thread as thread
import time

messageCount = 0

def on_message(ws, message):
    global messageCount
    messageCount += 1

def on_error(ws, error):
    print(error)

def on_close(ws):
    print("### closed ###")

def on_open(ws):
    def run(*args):
        global messageCount
        while True:
            print(f'messages received per second: {messageCount}')
            messageCount = 0
            time.sleep(1)
    thread.start_new_thread(run, ())


if __name__ == "__main__":
    # websocket.enableTrace(True)
    url = "ws://localhost:8008/1000000"
    ws = websocket.WebSocketApp(url,
                                on_message = on_message,
                                on_error = on_error,
                                on_close = on_close)
    ws.on_open = on_open
    ws.run_forever()
