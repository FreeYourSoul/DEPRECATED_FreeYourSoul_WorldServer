//
// Created by FyS on 23/05/17.
//

#include <spdlog/spdlog.h>
#include <boost/asio.hpp>
#include <FySMessage.pb.h>
#include <FysBus.hh>
#include <WorldServer.hh>
#include "TcpConnection.hh"

//fys::network::TcpConnection::~TcpConnection() {
//    shuttingConnectionDown();
//}

fys::network::TcpConnection::TcpConnection(boost::asio::io_service& io_service) : _isShuttingDown(false), _socket(io_service) {
}

boost::asio::ip::tcp::socket& fys::network::TcpConnection::getSocket() {
    return _socket;
}

void fys::network::TcpConnection::send(google::protobuf::Message&& msg) {
    boost::asio::streambuf b;
    std::ostream os(&b);
    msg.SerializeToOstream(&os);

    _socket.async_write_some(b.data(),
                             [this](const boost::system::error_code& ec, [[maybe_unused]]std::size_t bytesTransferred) {
                                 if (ec && !_isShuttingDown) {
                                     spdlog::get("c")->debug("An Error Occurred during writing {}", ec.message());
                                     shuttingConnectionDown();
                                 }
                             }
    );
}

void fys::network::TcpConnection::readOnSocket(fys::mq::FysBus<pb::FySMessage, ws::BUS_QUEUES_SIZE>::ptr &fysBus) {
    std::memset(_buffer, 0, MESSAGE_BUFFER_SIZE);
    _socket.async_read_some(boost::asio::buffer(_buffer, MESSAGE_BUFFER_SIZE),
                             [this, fysBus](boost::system::error_code ec, const std::size_t byteTransferred) {
                                this->handleRead(ec, byteTransferred, fysBus);
                            });
}

void fys::network::TcpConnection::handleRead(const boost::system::error_code &error, const size_t bytesTransferred,
                                             fys::mq::FysBus<pb::FySMessage, ws::BUS_QUEUES_SIZE>::ptr fysBus) {
    if (!((boost::asio::error::eof == error) || (boost::asio::error::connection_reset == error)) && !_isShuttingDown) {
        mq::QueueContainer<pb::FySMessage> containerMsg;
        pb::FySMessage message;

        message.ParseFromArray(_buffer, static_cast<int>(bytesTransferred));
        containerMsg.setIndexSession(this->_sessionIndex);
        containerMsg.setOpCodeMsg(message.type());
        spdlog::get("c")->debug("Raw Message to read on bus :{} Container op code : {} bytetransfered : {} with index: {}",
                                message.ShortDebugString(), containerMsg.getOpCodeMsg(), bytesTransferred, _sessionIndex);
        if (_sessionIndex == 501)
            return;
        containerMsg.setContained(std::move(message));
        fysBus->pushInBus(std::move(containerMsg));
        readOnSocket(fysBus);
    }
    else
        shuttingConnectionDown();

}

void fys::network::TcpConnection::shuttingConnectionDown() {
    if (!_isShuttingDown) {
        spdlog::get("c")->debug("Shutting down socket connection...");
        _isShuttingDown = true;
        try {
            _customShutdownHandler();
        }
        catch (std::exception &e) {
            spdlog::get("c")->error("An exception has been thrown during socket close {}", e.what());
        }
        _isShuttingDown = false;
    }
}

uint fys::network::TcpConnection::getSessionIndex() const {
    return _sessionIndex;
}

void fys::network::TcpConnection::setSessionIndex(uint _sessionIndex) {
    TcpConnection::_sessionIndex = _sessionIndex;
}

void fys::network::TcpConnection::setCustomShutdownHandler(const std::function<void()> &customShutdownHandler) {
    TcpConnection::_customShutdownHandler = customShutdownHandler;
}

std::string fys::network::TcpConnection::getIpAddress() const {
    return _socket.remote_endpoint().address().to_string();
}

ushort fys::network::TcpConnection::getPort() const {
    return _socket.remote_endpoint().port();
}

const std::function<void()> &fys::network::TcpConnection::getCustomShutdownHandler() const {
    return _customShutdownHandler;
}
