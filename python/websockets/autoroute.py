#!/usr/bin/env python3

# websocket send client

import argparse
import asyncio
import websockets
import time

try:
    import uvloop
    uvloop.install()
except ImportError:
    print('uvloop not available')
    pass

msgCount = 0
stopTimer = False

async def timer():
    global msgCount
    global stopTimer

    while not stopTimer:
        print(f'Received messages per second: {msgCount}')
        msgCount = 0

        await asyncio.sleep(1)


async def client(args):
    global msgCount
    global stopTimer

    asyncio.ensure_future(timer())

    target = args.msg_count
    args.url += f'/{target}'
    async with websockets.connect(args.url) as ws:
        start = time.time()
        async for message in ws:
            msgCount += 1

            target -= 1
            if target == 0:
                break

    duration = time.time() - start
    duration = int(1000 * duration)
    print(f'AUTOROUTE Python websockets :: {duration} ms')
    stopTimer = True


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='websocket proxy.')
    parser.add_argument('--url', help='Remote websocket url',
                        default='ws://localhost:8008')
    parser.add_argument('--msg_count', help='Remote websocket url',
                        default=1000 * 1000, type=int)
    args = parser.parse_args()

    asyncio.get_event_loop().run_until_complete(client(args))
