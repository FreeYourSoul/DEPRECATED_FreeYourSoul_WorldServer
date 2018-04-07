//
// Created by FyS on 04/03/18.
//

#ifndef FREESOULS_WORLD_CLUSTERMANAGER_HH
#define FREESOULS_WORLD_CLUSTERMANAGER_HH

#include <bits/unordered_set.h>

//forward declarations
namespace fys::ws {
    class GameServerInstance;
}

namespace fys::network {

    class WorldServerCluster {

    public:
        void addConnectionInCluster(ws::GameServerInstance &&clusterMemberToAdd);
        const ws::GameServerInstance &getWorldServerByPositionId(const std::string &positionId);

    private:
        std::vector<ws::GameServerInstance> _worldServerCluster;
    };
}


#endif //FREESOULS_WORLD_CLUSTERMANAGER_HH
