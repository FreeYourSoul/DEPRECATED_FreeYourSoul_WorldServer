//
// Created by FyS on 18/03/18.
//

#ifndef FREESOULS_WORLD_PLAYERMAPDATA_HH
#define FREESOULS_WORLD_PLAYERMAPDATA_HH

#include <atomic>
#include <zconf.h>
#include <ctime>
#include <PlayerManager.hh>

namespace fys::ws {

    struct Velocity {
        float speed = 0.025;
        float angle = 0.0;
    };

    struct MapPosition {
        float x = 0.0;
        float y = 0.0;
    };

    struct PlayersMapData {
        std::vector<bool> _isMoving;
        std::vector<double> _executeActionTime;
        std::vector<MapPosition> _pos;
        std::vector<Velocity> _velocity;

        PlayersMapData() :
                _isMoving(network::PlayerManager::CONNECTION_NUMBER, false),
                _executeActionTime(network::PlayerManager::CONNECTION_NUMBER, 0),
                _pos(network::PlayerManager::CONNECTION_NUMBER),
                _velocity(network::PlayerManager::CONNECTION_NUMBER) {}

        std::size_t playersSize() const {
            return _pos.size();
        }
    };

}


#endif //FREESOULS_WORLD_PLAYERMAPDATA_HH
