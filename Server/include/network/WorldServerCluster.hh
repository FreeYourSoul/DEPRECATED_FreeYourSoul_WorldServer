//
// Created by FyS on 04/03/18.
//

#ifndef FREESOULS_WORLD_CLUSTERMANAGER_HH
#define FREESOULS_WORLD_CLUSTERMANAGER_HH

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
         * \brief Setup all incoming neigbhoors given as parameter
         * \param neighbour
         */
        void setUpNeighborhood(const std::vector<std::pair<NeighborWS, std::string>> &neighbour);

        /**
         * \brief Add an entry in the accepted ip/token for new WorldServer connection
         * \param ipIncomingWs
         * \param tokenIncomingWs
         */
        void addIncomingWorldServer(const std::string &positionId, const std::string &ipIncomingWs,
                                    fys::network::Token &&tokenIncomingWs);

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
        bool connectWorldServerToCluster(uint indexInSession, Token &&tk);

    private:
        /**
         * Map of <PositionId, Token>
         */
        std::unordered_map<std::string, Token> _incomingWorldServer; // todo strongly typed Token required
        /**
         * Map of <PositionId, enumPosition>
         */
        std::unordered_map<std::string, NeighborWS> _awaitedNeighbour;
        std::unordered_map<NeighborWS, uint> _neighbourWS;
    };
}


#endif //FREESOULS_WORLD_CLUSTERMANAGER_HH
