//
// Created by FyS on 31/08/17.
//

#ifndef FREESOULS_AUTHENTICATION_HH
#define FREESOULS_AUTHENTICATION_HH

#include <QueueContainer.hh>
#include <memory>

//forward declarations
namespace fys {
    namespace pb {
        class FySMessage;
        class LoginMessage;
    }
    namespace ws {
        class WorldServer;
        class Context;
    }
}

namespace fys::ws::buslistener {

    /**
     * \brief Authenticator is a BusListener that is
     *  acknowledging new server connecting on the Universe
     *  acknowledging new players connecting and authenticate them
     */
    class Authenticator {

    public:
        enum { IndexInBus = 0 };

        explicit Authenticator(std::shared_ptr<WorldServer> &ws);

        void operator()(mq::QueueContainer<fys::pb::FySMessage> msg);

    private:
        /**
         * \brief Notify the current server instance of another world server joining the universe
         * \details This method is called by the FySGateway which acknowledge a new World Server connection in the
         * cluster and forward this information to the members of the cluster.
         * The server is checked if it is on the good position id (position id of the world server is nearby)
         * \note reminder, universe is how we call a world server cluster
         * \param loginMessage full notification message received
         */
        void notifyServerIncoming(pb::LoginMessage &&loginMessage);
        /**
         * Authenticate a WorldServer in the current world server instance
         * \param indexSession in the player session manager to root reply
         * \param loginMessage full authentication message received
         */
        void authWorldServer(uint indexSession, fys::pb::LoginMessage &&loginMessage);

        /**
         * \brief Notify the current server instance of a new player connecting soon, give its ip address and authentication
         * token to verify its identity. The real authentication is done in
         * \link fys::ws::buslistener::Authenticator::authPlayer(uint, pb::LoginMessage&&)
         * \param indexSession
         * \param loginMsg
         */
        void notifyPlayerIncoming(fys::pb::LoginMessage &&loginMsg);
        /**
         * Authenticate a player in the current world server instance
         * \param indexSession in the player session manager to root reply
         * \param loginMessage full authentication message received
         */
        void authPlayer(uint indexSession, pb::LoginMessage &&loginMessage);

    private:
        std::shared_ptr<WorldServer> _ws;

    };

}


#endif //FREESOULS_AUTHENTICATION_HH
