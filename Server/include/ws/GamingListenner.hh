//
// Created by FyS on 18/03/18.
//

#ifndef FREESOULS_WORLD_GAMINGLISTENNER_HH
#define FREESOULS_WORLD_GAMINGLISTENNER_HH

namespace fys::ws::buslistener {

    /**
     * This class is a bus BusListener that is
     * Acknowledging player moves,
     * Achnowledging player interactions with the world
     */
    class GamingListenner {

    public:
        enum { IndexInBus = 1 };

    };

}


#endif //FREESOULS_WORLD_GAMINGLISTENNER_HH
