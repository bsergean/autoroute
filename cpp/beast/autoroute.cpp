#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

#include <atomic>
#include <mutex>
#include <thread>
#include <chrono>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

// Sends a WebSocket message and prints the response
int main(int argc, char** argv)
{
    try
    {
        // Check command line arguments.
        if (argc != 4)
        {
            std::cerr <<
                "Usage: websocket-client-sync <host> <port> <count>\n" <<
                "Example:\n" <<
                "    websocket-client-sync localhost 8008 1000000\n";
            return EXIT_FAILURE;
        }
        auto const host = argv[1];
        auto const port = argv[2];

        std::stringstream ss;
        ss << argv[3];
        int msgCount = 0;
        ss >> msgCount;

        std::atomic<uint64_t> receivedCountPerSecs(0);
        std::atomic<uint64_t> target(1000 * 1000);
        std::mutex conditionVariableMutex;
        std::atomic<bool> stop(false);

        auto timer = [&stop, &receivedCountPerSecs] {
            while (!stop)
            {
                //
                // We cannot write to sentCount and receivedCount
                // as those are used externally, so we need to introduce
                // our own counters
                //
                std::stringstream ss;
                std::cerr << "messages received per second: "
                    << receivedCountPerSecs << std::endl;

                receivedCountPerSecs = 0;

                auto duration = std::chrono::seconds(1);
                std::this_thread::sleep_for(duration);
            }
        };

        std::thread t1(timer);

        // The io_context is required for all I/O
        net::io_context ioc;

        // These objects perform our I/O
        tcp::resolver resolver{ioc};
        websocket::stream<tcp::socket> ws{ioc};

        // Look up the domain name
        auto const results = resolver.resolve(host, port);

        // Make the connection on the IP address we get from a lookup
        net::connect(ws.next_layer(), results.begin(), results.end());

        // Set a decorator to change the User-Agent of the handshake
        ws.set_option(websocket::stream_base::decorator(
                    [](websocket::request_type& req)
                    {
                    req.set(http::field::user_agent,
                            std::string(BOOST_BEAST_VERSION_STRING) +
                            " websocket-client-coro");
                    }));

        // Perform the websocket handshake
        std::string uri("/");
        uri += std::to_string(msgCount);

        ws.handshake(host, uri);

        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < msgCount; ++i)
        {
            // This buffer will hold the incoming message
            beast::flat_buffer buffer;

            // Read a message into our buffer
            ws.read(buffer);

            receivedCountPerSecs++;
        }

        auto now = std::chrono::high_resolution_clock::now();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
        auto ms = milliseconds.count();
        std::cerr << "AUTOROUTE C++ beast :: " << ms << " ms" << std::endl;

        // Close the WebSocket connection
        ws.close(websocket::close_code::normal);

        // If we get here then the connection is closed gracefully

        // terminate our 'printf' thread
        stop = true;
        t1.join();
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
