//
// Created by FyS on 04/03/18.
//

#include <TcpConnection.hh>
#include "WorldServerCluster.hh"

void fys::network::WorldServerCluster::addConnectionInCluster(ws::GameServerInstance &&clusterMemberToAdd) {
    _worldServerCluster.push_back(std::move(clusterMemberToAdd));
}

const fys::ws::GameServerInstance &
fys::network::WorldServerCluster::getWorldServerByPositionId(const std::string &positionId) {
    auto it = std::find_if(_worldServerCluster.begin(), _worldServerCluster.end(),
                           [&positionId](const fys::ws::GameServerInstance &gsi){ return positionId == gsi.getPositionId(); }
                           );
    return *it;
}
