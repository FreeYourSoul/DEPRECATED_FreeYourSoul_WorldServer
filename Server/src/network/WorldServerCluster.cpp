//
// Created by FyS on 04/03/18.
//

#include <spdlog/spdlog.h>
#include <TcpConnection.hh>
#include "WorldServerCluster.hh"

fys::network::WorldServerCluster::WorldServerCluster(uint size) : SessionManager(size) {
    setName("World Server Manager");
}

void fys::network::WorldServerCluster::setUpNeighborhood(const std::vector<std::pair<NeighborWS, std::string>> &neighbour) {
    for (auto neighbourPair : neighbour) {
        _awaitedNeighbour[neighbourPair.second] = neighbourPair.first;
    }
}

void fys::network::WorldServerCluster::addIncomingWorldServer(const std::string &positionId,
                                                              const std::string &ipIncomingWs,
                                                              fys::network::Token &&tk) {
    if (_awaitedNeighbour.find(positionId) != _awaitedNeighbour.cend()) {
        spdlog::get("c")->debug("Server is incoming {} with ip {}", positionId, ipIncomingWs);
        _incomingWorldServer[ipIncomingWs] = tk;
    }
    else {
        spdlog::get("c")->debug("Server {} attempted to connect but is not neighbour server", positionId);
    }
}

bool fys::network::WorldServerCluster::connectWorldServerToCluster(uint indexInSession, const std::string &positionId, Token &&tk) {
    const NeighborWS nws = _awaitedNeighbour.at(positionId);

    if (_neighbourWS.find(nws) == _neighbourWS.end()) {
        const std::string ip = getIp(indexInSession);
        const auto findIt = _incomingWorldServer.find(ip);

        if (findIt != _incomingWorldServer.cend()) {
            if (std::equal(tk.cbegin(), tk.cend(), _incomingWorldServer.at(ip).cbegin())) {
                _neighbourWS[_awaitedNeighbour.at(positionId)] = indexInSession;
                _incomingWorldServer.erase(findIt);
                return true;
            }
            disconnectUser(indexInSession, _incomingWorldServer.at(ip));
            spdlog::get("c")->warn("The given token is wrong for index {} ip {}", indexInSession, ip);
        }
        spdlog::get("c")->critical("The given ip {} is not registered as accepted ip, "
                                   "this should have already been checked!", ip);
    }
    spdlog::get("c")->error("A server with positionId {} is already registered", positionId);
    return false;
}
