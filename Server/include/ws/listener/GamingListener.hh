//
// Created by FyS on 18/03/18.
//

#ifndef FREESOULS_WORLD_GAMINGLISTENNER_HH
#define FREESOULS_WORLD_GAMINGLISTENNER_HH

#include <memory>
#include <QueueContainer.hh>

//forward declarations
namespace fys {
    namespace pb {
        class FySMessage;
    }
    namespace ws {
        class WorldServer;
    }
}


namespace fys::ws::buslistener {

    /**
     * This class is a bus BusListener that is
     * Acknowledging player moves,
     * Achnowledging player interactions with the world
     */
    class GamingListener {

    public:
        enum { IndexInBus = 1 };

        explicit GamingListener(std::shared_ptr<WorldServer> &ws);

        void operator()(mq::QueueContainer<pb::FySMessage> msg);

    private:

    private:
        std::shared_ptr<WorldServer> _ws;

    };

}


#endif //FREESOULS_WORLD_GAMINGLISTENNER_HH
