//
// Created by FyS on 04/03/18.
//

#ifndef FREESOULS_WORLD_CLUSTERMANAGER_HH
#define FREESOULS_WORLD_CLUSTERMANAGER_HH

#include <bits/unordered_set.h>
#include <unordered_map>
#include "SessionManager.hh"

//forward declarations
namespace fys::ws {
}

namespace fys::network {

    class WorldServerCluster : public SessionManager {
    public:
        /**
         * 8 possibles Neighbors
         */
        enum { CONNECTION_NUMBER = 8 };
        enum NeighborWS {
            TLWS,
            TRWS,
            DLWS,
            DRWS,
            TWS,
            DWS,
            LWS,
            RWS
        };

        WorldServerCluster(uint size);

        /**
         * \brief Add an entry in the accepted ip/token for new WorldServer connection
         * \param ipIncomingWs
         * \param tokenIncomingWs
         */
        void addIncomingWorldServer(const std::string &positionId, const std::string &ipIncomingWs,
                                            const fys::network::Token &tokenIncomingWs);
        /**
         * \brief Add a world server connection (call the SessionManager #fys::network::SessionManager::addConnection()) if
         * the given ip is in the accepted incoming world server ip (stored in #_incomingWorldServer)
         * \param newConnection
         * \return
         */
        uint addWorldServer(const std::shared_ptr<TcpConnection> &newConnection);
        /**
         * \brief Check if the token corresponding to the given ip is correct and connect the world server
         * \details Connect the world server to the cluster thanks to the given token. This token has to match the one
         * given during the incoming world server registering phase (by call of
         * #addIncomingWorldServer(const std::string&, const Token&) ). If it doesn't match, the WorldServer's session
         * is disconnected because of failed authentication.
         *
         * \param indexInSession used to find the ip of the user
         * \param tk to check
         * \return true if the given token match the accepted one, return false otherwise
         */
        bool connectWorldServerToCluster(uint indexInSession, const Token &tk);

    private:
        std::unordered_map<std::string, Token> _incomingWorldServer;
        std::unordered_map<std::string, NeighborWS> _awaitedNeighboors;
        std::unordered_map<NeighborWS, uint> _neighborWS;
    };
}


#endif //FREESOULS_WORLD_CLUSTERMANAGER_HH
