//
// Created by FyS on 31/08/17.
//

#ifndef FREESOULS_AUTHENTICATION_HH
#define FREESOULS_AUTHENTICATION_HH

#include <FySMessage.pb.h>
#include <FySAuthenticationLoginMessage.pb.h>
#include "WorldServer.hh"

namespace fys::ws::buslistener {

    /**
     * \brief Authenticator is a BusListener that is
     *  acknowledging new server connecting on the Universe
     *  acknowledging new players connecting and authenticate them
     */
    class Authenticator {

    public:
        enum { IndexInBus = 0 };

        explicit Authenticator(WorldServer::ptr&);

        void operator()(mq::QueueContainer<fys::pb::FySMessage> msg);

    private:
        void notifiedServer(fys::pb::LoginMessage &&indexSession);
        void authPlayer(const uint indexSession, pb::LoginMessage &&loginMessage);

    private:
        WorldServer::ptr _gtw;
    };

}


#endif //FREESOULS_AUTHENTICATION_HH
