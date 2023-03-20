#include <iostream>
#include <thread>

/* Way to generalize the Windows routing (only for Windows) */
#ifdef _WIN32
#define _WIN31_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

std::vector<char> vBuffer(1*1024);

void getData(asio::ip::tcp::socket& socket)
{
    socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
           [&](std::error_code ec, size_t len)
                   {
                        if (!ec)
                        {
                            std::cout<<"\n\nRead "<<len<<" bytes\n\n";

                            for (int i = 0; i < len; ++i)
                                std::cout<<vBuffer[i];

                            getData(socket);
                        }
                   });
}

int main() {
    asio::error_code ec;

    // unique instance_id
    asio::io_context context;

    asio::io_context::work idleWork(context);

    // start context in separate thread
    std::thread thrContext = std::thread([&]() { context.run(); });

    // get the address of the site
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address("51.38.81.49", ec), 80);

    asio::ip::tcp::socket socket(context);
    socket.connect(endpoint, ec);

    if (!ec)
        std::cout<<"Connected\n";
    else
        std::cout<<"Error: "<<ec.message()<<"\n";

    if (socket.is_open())
    {
        getData(socket);

        std::string sRequest =
                "GET /index.html HTTP/1.1\r\n"
                "Host: example.com\r\n"
                "Connection: close\r\n\r\n";

        socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);

        // Very bad practice to put hard-coded delays
//        using namespace std::chrono_literals;
//        std::this_thread::sleep_for(1000ms);

        // Better
        socket.wait(socket.wait_read);

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2200ms);

        context.stop();
        if (thrContext.joinable()) thrContext.join();
    }

    return 0;
}
