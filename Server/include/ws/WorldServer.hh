//
// Created by FyS on 23/05/17.
//

#ifndef FREESOULS_GATEWAY_HH
#define FREESOULS_GATEWAY_HH

#include <boost/asio/ip/tcp.hpp>
#include <SessionManager.hh>
#include <FysBus.hh>
#include <TcpConnection.hh>
#include <Context.hh>
#include <FySMessage.pb.h>
#include <Map.hh>
#include <ClusterManager.hh>
#include <PlayerManager.hh>

namespace fys::ws {

    class WorldServer {

    public:
        using uptr = std::unique_ptr<WorldServer>;
        using ptr = std::shared_ptr<WorldServer>;
        using wptr = std::weak_ptr<WorldServer>;

    public:
        ~WorldServer();
        WorldServer(const Context &, boost::asio::io_service&,
                    fys::mq::FysBus<fys::pb::FySMessage, BUS_QUEUES_SIZE>::ptr&);

        static inline ptr create(const Context &ctx, boost::asio::io_service &ios,
                                 fys::mq::FysBus<fys::pb::FySMessage, BUS_QUEUES_SIZE>::ptr &fysBus) {
            return std::make_shared<WorldServer>(ctx, ios, fysBus);
        }

        void runPlayerAccept();
        void connectToGateway(const Context &ctx);

        void connectAndAddWorldServerInCluster(const std::string &clusterKey, const std::string &token,
                                               const std::string &ip, const std::string &port);

        network::PlayerManager &getGamerConnections() { return _gamerConnections; }
        network::ClusterManager &getWorldServerCluster() { return _worldServerCluster; }
        const network::TcpConnection::uptr &getGtwConnection() const { return _gtwConnection; }

    private:
        void notifyGateway(const std::string &id) const;

    private:
        boost::asio::io_service &_ios;
        boost::asio::ip::tcp::acceptor _acceptorPlayer;
        fys::mq::FysBus<fys::pb::FySMessage, BUS_QUEUES_SIZE>::ptr _fysBus;

        network::PlayerManager _gamerConnections;
        network::ClusterManager _worldServerCluster;
        network::TcpConnection::uptr _gtwConnection;

        std::vector<fys::ws::Map> _map;
    };

}

#endif //FREESOULS_GATEWAY_HH
