//
// Created by FyS on 23/05/17.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "InfiniteRecursion"

#include <WorldServer.hh>
#include <FySAuthenticationLoginMessage.pb.h>

static constexpr int RETRY_TIMER = 5;
static constexpr char MAGIC_PASSWORD[] = "42Magic42FyS";

fys::ws::WorldServer::~WorldServer() = default;

fys::ws::WorldServer::WorldServer(const fys::ws::Context &ctx, boost::asio::io_service &ios, fys::mq::FysBus<fys::pb::FySMessage, BUS_QUEUES_SIZE>::ptr &fysBus) :
        _ios(ios),
        _acceptorPlayer(_ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), ctx.getPort())),
        _fysBus(fysBus),
        _gamerConnections(1000),
        _worldServerCluster(10),
        _gtwConnection(std::make_unique<fys::network::TcpConnection>(ios))
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

void fys::ws::WorldServer::connectToGateway(const fys::ws::Context &ctx) {
    std::cout << "Connect to the gateway on host:" << ctx.getGtwIp() << " on port:" << ctx.getGtwPort() << std::endl;
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(ctx.getGtwIp()), ctx.getGtwPort());

    _gtwConnection->getSocket().async_connect(endpoint, [this, &ctx](const boost::system::error_code& error) {
        if (error) {
            boost::asio::deadline_timer timer(_ios);
            std::cout << "Warn: Error while trying to connect to the gateway" << std::endl;
            timer.expires_from_now(boost::posix_time::seconds(RETRY_TIMER));
            timer.wait();
            this->connectToGateway(ctx);
        }
        else
            this->notifyGateway(ctx.getPositionId());
    });
}

void fys::ws::WorldServer::notifyGateway(const std::string &id) const {
    fys::pb::FySMessage msg;
    fys::pb::LoginMessage loginMsg;
    fys::pb::LoginGameServer gameServerMessage;

    gameServerMessage.set_isworldserver(true);
    gameServerMessage.set_magicpassword(std::string(MAGIC_PASSWORD).append(id));
    loginMsg.set_typemessage(fys::pb::LoginMessage_Type_LoginGameServer);
    loginMsg.mutable_content()->PackFrom(gameServerMessage);
    msg.set_type(fys::pb::AUTH);
    msg.mutable_content()->PackFrom(loginMsg);
    _gtwConnection->send(std::move(msg));

    std::cout << "gateway notified " << msg.ShortDebugString() << std::endl;
}

void fys::ws::WorldServer::addWorldServerInCluster(const std::string& clusterKey, const fys::network::TcpConnection::ptr &newConnection) {
    _worldServerCluster.addConnectionInCluster(clusterKey, newConnection);
}

#pragma clang diagnostic pop