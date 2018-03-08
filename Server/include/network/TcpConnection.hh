//
// Created by FyS on 23/05/17.
//

#ifndef FREESOULS_TCPCONNECTION_HH
#define FREESOULS_TCPCONNECTION_HH

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/write.hpp>
#include <Context.hh>
#include <FysBus.hh>
#include <FySMessage.pb.h>

namespace fys::network {

    /**
     * This class reprensent a connection of a client to the Server
     */
    class TcpConnection : public std::enable_shared_from_this<TcpConnection>
    {
    public:
        using uptr = std::unique_ptr<TcpConnection>;
        using ptr = std::shared_ptr<TcpConnection>;
        using wptr = std::weak_ptr<TcpConnection>;

        static inline ptr create(boost::asio::io_service& io_service) {
            return std::make_shared<TcpConnection>(io_service);
        }

        explicit TcpConnection(boost::asio::io_service& io_service);

        boost::asio::ip::tcp::socket& getSocket();

        void readOnSocket(fys::mq::FysBus<fys::pb::FySMessage, ws::BUS_QUEUES_SIZE>::ptr &fysBus);
        void send(google::protobuf::Message&& msg);

        void setCustomShutdownHandler(const std::function<void()> &customShutdownHandler);
        void setSessionIndex(uint _sessionIndex);

        uint getSessionIndex() const;
        /**
         * \brief Get the remoge ip address
         * \return
         */
        std::string getIpAddress() const;
        /**
         * \brief Get the remote port
         * \return remote port
         */
        ushort getPort() const;


    private:

        void shuttingConnectionDown();

        void handleRead(const boost::system::error_code &error, size_t bytesTransferred,
                        fys::mq::FysBus<fys::pb::FySMessage, ws::BUS_QUEUES_SIZE>::ptr);

    private:
        bool _isShuttingDown;
        uint _sessionIndex;
        boost::asio::ip::tcp::socket _socket;

        static const int MESSAGE_BUFFER_SIZE = 512;
        mutable u_char _buffer[MESSAGE_BUFFER_SIZE];

        std::function<void ()> _customShutdownHandler;
    };

}

#endif //FREESOULS_TCPCONNECTION_HH
