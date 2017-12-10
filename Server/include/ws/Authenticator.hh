//
// Created by FyS on 31/08/17.
//

#ifndef FREESOULS_AUTHENTICATION_HH
#define FREESOULS_AUTHENTICATION_HH

#include <FySMessage.pb.h>
#include <FySAuthenticationLoginMessage.pb.h>
#include "WorldServer.hh"

namespace fys {
    namespace ws {
        namespace buslistener {

            class Authenticator {

            public:
                enum { IndexInBus = 0 };

                explicit Authenticator(WorldServer::ptr&);

                void operator()(mq::QueueContainer<fys::pb::FySMessage> msg);

            private:
                void notifyNewPlayer(fys::pb::LoginMessage  &&indexSession);
                void authPlayer(const uint indexSession, pb::LoginMessage &&loginMessage);

            private:
                WorldServer::ptr _gtw;
            };

        }
    }
}


#endif //FREESOULS_AUTHENTICATION_HH
