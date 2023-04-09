//
// Created by Emil Tsanev on 7.4.2023 г..
//

#ifndef NETCOMMON_NET_CONNECTION_H
#define NETCOMMON_NET_CONNECTION_H

#include "net_tsqueue.h"
#include "net_message.h"

namespace olc::net
{
    /*  Connection Template class
     *  - will handle the Client/Server communication
     *  - It is deriving from std::enable_shared_from_this<connection<T>>, in order for the
     *  shared_from_this to be called inside the class connection and obtain a std::shared_ptr
     */
    template<typename T>
    class connection : public std::enable_shared_from_this<connection<T>> {

    public:

        connection() {}
        virtual ~connection() {}

    public:
        bool ConnectToServer();
        bool Disconnect();
        bool IsConnected();

    public:
        bool Send(const message<T>& msg);

    protected:
        // Each connection has unique socket to a remote
        asio::ip::tcp::socket m_socket;

        // This ctx is shared with the whole asio instance
        asio::io_context& m_asioContext;

        // Queue to hold all messages to be sent to the remote side of this connection
        tsqueue<message<T>> m_qMessagesOut;

        // This references the incoming queue of the parent object
        // Should be owned_messages and reference because the "owner"
        // of this connection is expected to provide the queue with incoming messages
        tsqueue<owned_message<T>>& m_qMessagesIn;
    };
}

#endif //NETCOMMON_NET_CONNECTION_H
