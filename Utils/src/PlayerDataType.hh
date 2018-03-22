//
// Created by FyS on 18/03/18.
//

#ifndef FREESOULS_WORLD_PLAYERMAPDATA_HH
#define FREESOULS_WORLD_PLAYERMAPDATA_HH

#include <zconf.h>

namespace fys::ws {

    struct Velocity {
        float speed = 1.00;
        float angle = 0.00;
    };

    struct MapPosition {
        float x = 0.0;
        float y = 0.0;
    };

    enum PlayerState {
        MOVE_ON = 0,
        MOVE_OFF = 1
    };

    struct PlayerMapData {
        PlayerState _state = PlayerState::MOVE_OFF;
        MapPosition _pos;
        Velocity _velocity;
        std::time_t _initRequestTime;
        std::time_t _lastTimeMoved;
    };

}


#endif //FREESOULS_WORLD_PLAYERMAPDATA_HH
