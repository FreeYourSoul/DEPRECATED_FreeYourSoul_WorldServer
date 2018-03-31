//
// Created by FyS on 18/03/18.
//

#ifndef FREESOULS_WORLD_PLAYERMAPDATA_HH
#define FREESOULS_WORLD_PLAYERMAPDATA_HH

#include <atomic>
#include <zconf.h>
#include <ctime>

namespace fys::ws {

    struct Velocity {
        float speed = 0.025;
        float angle = 0.0;
    };

    struct MapPosition {
        float x = 0.0;
        float y = 0.0;
    };

    struct PlayerMapData {
        bool _isMoving = false;
        MapPosition _pos;
        Velocity _velocity;
        double _executeActionTime = 0;
    };

}


#endif //FREESOULS_WORLD_PLAYERMAPDATA_HH
