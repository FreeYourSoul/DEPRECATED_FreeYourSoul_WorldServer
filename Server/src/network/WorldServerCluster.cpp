//
// Created by FyS on 04/03/18.
//

#include <TcpConnection.hh>
#include "WorldServerCluster.hh"


fys::network::WorldServerCluster::WorldServerCluster(uint size) : SessionManager(size) {
    setName("World Server Manager");
}

void fys::network::WorldServerCluster::addIncomingWorldServer(const std::string &ipIncomingWs,
                                                              const fys::network::Token &tokenIncomingWs) {
    _incomingWorldServer[ipIncomingWs] = tokenIncomingWs;
}
