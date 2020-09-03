#!/usr/bin/env python3

# websocket send client

import argparse
import asyncio
import websockets

try:
    import uvloop
    uvloop.install()
except ImportError:
    print('uvloop not available')
    pass

msgCount = 0
msgCountTotal = 0
stopTimer = False

async def timer():
    global msgCount
    global msgCountTotal
    global stopTimer

    while not stopTimer:
        print(f'Received messages: {msgCount} per second {msgCountTotal} total')
        msgCount = 0

        await asyncio.sleep(1)


async def client(args):
    global msgCount
    global msgCountTotal
    global stopTimer

    asyncio.ensure_future(timer())

    args.url += f'/{args.msg_count}'
    async with websockets.connect(args.url) as ws:
        async for message in ws:
            msgCount += 1
            msgCountTotal += 1

            if msgCountTotal == args.msg_count:
                break

    print('Stopping timer')
    stopTimer = True


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='websocket proxy.')
    parser.add_argument('--url', help='Remote websocket url',
                        default='ws://localhost:8008')
    parser.add_argument('--msg_count', help='Remote websocket url',
                        default=1000 * 1000, type=int)
    args = parser.parse_args()

    asyncio.get_event_loop().run_until_complete(client(args))