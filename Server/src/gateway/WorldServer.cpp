//
// Created by FyS on 23/05/17.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "InfiniteRecursion"

#include <WorldServer.hh>

fys::ws::WorldServer::~WorldServer() = default;

fys::ws::WorldServer::WorldServer(const fys::ws::Context &ctx, boost::asio::io_service &ios, fys::mq::FysBus<fys::pb::FySMessage, BUS_QUEUES_SIZE>::ptr &fysBus) :
        _ios(ios),
        _acceptorPlayer(_ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), ctx.getPort())),
        _fysBus(fysBus),
        _gamerConnections(1000)
{}

void fys::ws::WorldServer::runPlayerAccept() {
    const network::TcpConnection::ptr session = network::TcpConnection::create(_ios);

    _acceptorPlayer.async_accept(session->getSocket(),

            [this, session](const boost::system::error_code& e) {
                session->readOnSocket(_fysBus);
                this->_gamerConnections.addConnection(session);
                this->runPlayerAccept();
            }

    );
}

const fys::network::SessionManager &fys::ws::WorldServer::getGamerConnections() const {
    return _gamerConnections;
}

const fys::network::TcpConnection::ptr &fys::ws::WorldServer::get_gtwConnection() const {
    return _gtwConnection;
}

#pragma clang diagnostic pop