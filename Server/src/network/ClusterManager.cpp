//
// Created by FyS on 04/03/18.
//

#include "ClusterManager.hh"

fys::network::ClusterManager::ClusterManager(const uint size) : SessionManager(size) {}

uint fys::network::ClusterManager::addConnectionInCluster(const std::string &clusterKey,
                                                          const fys::network::TcpConnection::ptr &newConnection) {
    uint indexInClusterSession = addConnection(newConnection);

    if (indexInClusterSession >= _clusterKey.size())
        _clusterKey.resize(_clusterKey.size() * 2);
    _clusterKey[indexInClusterSession] = clusterKey;
}
