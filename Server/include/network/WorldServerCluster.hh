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
        void addIncomingWorldServer(const std::string &ipIncomingWs, const Token &tokenIncomingWs);
        /**
         * \brief Add a player connection (call the SessionManager #fys::network::SessionManager::addConnection()) if
         * the given ip is in the accepted incoming player ip (stored in #_incomingPlayer)
         * \param newConnection
         * \return
         */
        uint addWorldServer(const std::shared_ptr<TcpConnection> &newConnection);

    private:
        std::unordered_map<std::string, Token> _incomingWorldServer;
        std::unordered_map<std::string, NeighborWS> _awaitedNeighboors;
        std::unordered_map<NeighborWS, uint> _neighborWS;
    };
}


#endif //FREESOULS_WORLD_CLUSTERMANAGER_HH
