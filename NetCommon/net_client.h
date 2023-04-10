//
// Created by Emil Tsanev on 7.4.2023 г..
//

#ifndef NETCOMMON_NET_CLIENT_H
#define NETCOMMON_NET_CLIENT_H

#include "net_common.h"
#include "net_message.h"
#include "net_tsqueue.h"
#include "net_connection.h"

namespace olc::net
{
    /* Client class
     * - responsible for setting up ASIO and connection
     * - access point to talk to the server
     */
    template <typename T>
    class client_interface
    {
    public:
        client_interface() : m_socket(m_context)
        {
            // Initialize the socket with the io context
        }

        virtual ~client_interface()
        {
            // Before destruction try to disconnect
            Disconnect();
        }

    public:
        // Connect to server with hostname/ip-address and port
        bool Connect(const std::string& host, const uint16_t port)
        {
            try
            {
                m_connection = std::make_unique<connection<T>>(); //TODO

                // Resolve hostname/ip-adders into tangible physical address
                asio::ip::tcp::resolver resolver(m_context);
                m_endpoints = resolver.resolve(host, std::to_string(port));

                m_connection->ConnectToServer(m_endpoints);

                thrContext = std::thread([this]() { m_context.run(); });
            }
            catch (std::exception& e)
            {
                std::cerr<<"Client Exception: "<<e.what()<<"\n";
                return false;
            }

        }

        // Disconnect from server
        void Disconnect()
        {
            if (IsConnected())  // Disconnect if connection exists
            {
                m_connection->Disconnect();
            }

            // Finish the ASIO context, because we are done with it
            m_context.stop();
            // and ASIO's context thread
            if (thrContext.joinable())
                thrContext.join();

            // Destroy the connection object
            m_connection.release();
        }

        // Check if client is actually connected to a server
        bool IsConnected()
        {
            if (m_connection)
                return m_connection->IsConnected();
            else
                return false;
        }

        // Retrieve queue of messages from server
        tsqueue<owned_message<T>>& Incoming()
        {
            return m_qMessagesIn;
        }

    protected:
        // asio ctx that handles the data transfer
        asio::io_context m_context;
        // but needs a thread of its own to execute its work commands
        std::thread thrContext;
        // hardware socket that is connected to the server
        asio::ip::tcp::socket m_socket;
        // The client has single instance of "connection" obj, far data transfer
        std::unique_ptr<connection<T>> m_connection;

    private:
        // This is the thread safe queue of incomming msgs from the server
        tsqueue<owned_message<T>> m_qMessagesIn;
    };
}

#endif //NETCOMMON_NET_CLIENT_H
