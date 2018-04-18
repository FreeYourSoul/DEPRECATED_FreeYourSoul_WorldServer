//
// Created by FyS on 04/03/18.
//

#include <spdlog/spdlog.h>
#include <TcpConnection.hh>
#include "WorldServerCluster.hh"

fys::network::WorldServerCluster::WorldServerCluster(uint size) : SessionManager(size) {
    setName("World Server Manager");
}

void fys::network::WorldServerCluster::addIncomingWorldServer(const std::string &positionId,
                                                              const std::string &ipIncomingWs,
                                                              const fys::network::Token &tokenIncomingWs) {
    if (_awaitedNeighboors.find(positionId) != _awaitedNeighboors.cend()) {
        spdlog::get("c")->debug("Server {} connected to cluster with ip {}", positionId, ipIncomingWs);
        _incomingWorldServer[ipIncomingWs] = tokenIncomingWs;
    }
    else {
        spdlog::get("c")->debug("Server {} attempted to connect but is not neighbour server", positionId);
    }
}

uint fys::network::WorldServerCluster::addWorldServer(const std::shared_ptr<fys::network::TcpConnection> &newConnection) {
    const auto findIt = _incomingWorldServer.find(newConnection->getIpAddress());

    if (findIt == _incomingWorldServer.end()) {
        spdlog::get("c")->warn("The player isn't registered on the accepted ip list, ip trying to connect is {}", newConnection->getIpAddress());
        return std::numeric_limits<uint>::max();
    }
    return addConnection(newConnection);
}

bool fys::network::WorldServerCluster::connectWorldServerToCluster(uint indexInSession, const fys::network::Token &tk) {
    const std::string ip = getIp(indexInSession);
    const auto findIt = _incomingWorldServer.find(ip);

    if (findIt != _incomingWorldServer.cend()) {
        if (std::equal(tk.cbegin(), tk.cend(), _incomingWorldServer.at(ip).cbegin())) {
            _incomingWorldServer.erase(findIt);
            return true;
        }
        disconnectUser(indexInSession, _incomingWorldServer.at(ip));
        spdlog::get("c")->warn("The given token is wrong for index {} ip {}", indexInSession, ip);
    }
    spdlog::get("c")->error("The given ip {} is not registered as accepted ip, "
                            "this should have already been checked!", ip);
    return false;
}
