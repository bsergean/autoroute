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
import _thread as thread
import time
import sys
import argparse

messageCount = 0
target = 0
start = 0

def on_message(ws, message):
    global messageCount
    global target
    global start
    messageCount += 1

    target -= 1
    if target == 0:
        duration = time.time() - start
        duration = int(1000 * duration)
        print(f'AUTOROUTE Python websocket_client :: {duration} ms')
        sys.exit(0)

def on_error(ws, error):
    print(f'error: {error}')

def on_close(ws):
    print("### closed ###")

def on_open(ws):
    global start
    start = time.time()

    def run(*args):
        global messageCount
        while True:
            print(f'messages received per second: {messageCount}')
            messageCount = 0
            time.sleep(1)
    thread.start_new_thread(run, ())


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='websocket proxy.')
    parser.add_argument('--url', help='Remote websocket url',
                        default='ws://localhost:8008')
    parser.add_argument('--msg_count', help='Remote websocket url',
                        default=1000 * 1000, type=int)
    args = parser.parse_args()

    url = f'{args.url}/{args.msg_count}'
    target = args.msg_count

    # websocket.enableTrace(True)
    ws = websocket.WebSocketApp(url,
                                on_message = on_message,
                                on_error = on_error,
                                on_close = on_close)
    ws.on_open = on_open
    ws.run_forever()
