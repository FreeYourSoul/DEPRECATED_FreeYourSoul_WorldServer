//
// Created by FyS on 18/03/18.
//

#ifndef FREESOULS_WORLD_PLAYERMAPDATA_HH
#define FREESOULS_WORLD_PLAYERMAPDATA_HH

#include <zconf.h>

namespace fys::ws {

    struct Velocity {
        double speed = 1.00;
        double angle = 0.00;
    };

    struct MapPosition {
        double x;
        double y;
    };

    struct PlayerMapData {
        MapPosition _pos;
        Velocity _velocity;
    };

}


#endif //FREESOULS_WORLD_PLAYERMAPDATA_HH
