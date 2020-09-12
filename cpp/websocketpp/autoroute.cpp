/*
 * Copyright (c) 2016, Peter Thorson. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the WebSocket++ Project nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL PETER THORSON BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * g++ autoroute.cpp -o autoroute -lboost_system -lssl -lcrypto
 */

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include <iostream>
#include <atomic>
#include <mutex>
#include <thread>
#include <chrono>

typedef websocketpp::client<websocketpp::config::asio_client> client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

std::atomic<uint64_t> receivedCountPerSecs(0);
std::atomic<uint64_t> target(0);
std::atomic<bool> stop(false);
std::chrono::time_point<std::chrono::high_resolution_clock> start;

// This message handler will be invoked once for each incoming message. It
// prints the message and then sends a copy of the message back to the server.
void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg)
{
    receivedCountPerSecs++;

    target--;
    if (target == 0)
    {
        auto now = std::chrono::high_resolution_clock::now();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
        auto ms = milliseconds.count();

        std::cerr << "AUTOROUTE C++ websocketpp :: " << ms << std::endl;

        stop = true;
        websocketpp::lib::error_code ec;
        c->close(hdl, websocketpp::close::status::going_away, "autoroute run completed", ec);
    }
}

void on_open(client* c, websocketpp::connection_hdl hdl)
{
    std::cerr << "connection opened" << std::endl;
    start = std::chrono::high_resolution_clock::now();
}

void on_close(client* c, websocketpp::connection_hdl hdl)
{
    std::cerr << "connection closed" << std::endl;
}

int main(int argc, char* argv[])
{
    auto timer = [] {
        while (!stop)
        {
    	    //
    	    // We cannot write to sentCount and receivedCount
    	    // as those are used externally, so we need to introduce
    	    // our own counters
    	    //
            std::stringstream ss;
            std::cerr << "messages received per second: " << receivedCountPerSecs << std::endl;
    
    	    receivedCountPerSecs = 0;
    
    	    auto duration = std::chrono::seconds(1);
    	    std::this_thread::sleep_for(duration);
        }
    };
    
    std::thread t1(timer);

    std::string url = argv[1];

	std::stringstream ss;
	ss << argv[2];
    int msgCount = 0;
	ss >> msgCount;

    target = msgCount;

    url += "/";
    url += argv[2];

    try {
        // Create a client endpoint
        client c;

        // Set logging to be pretty verbose (everything except message payloads)
        c.set_access_channels(websocketpp::log::alevel::all);
        c.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize ASIO
        c.init_asio();

        // Register our message handler
        c.set_message_handler(bind(&on_message,&c,::_1,::_2));
        c.set_open_handler(bind(&on_open,&c,::_1));

        websocketpp::lib::error_code ec;
        client::connection_ptr con = c.get_connection(url, ec);
        if (ec) {
            std::cout << "could not create connection because: " << ec.message() << std::endl;
            return 0;
        }

        // Note that connect here only requests a connection. No network messages are
        // exchanged until the event loop starts running in the next line.
        c.connect(con);

        // Start the ASIO io_service run loop
        // this will cause a single connection to be made to the server. c.run()
        // will exit when this connection is closed.
        c.run();

        // stop the progress reporting
        t1.join();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    }
}
