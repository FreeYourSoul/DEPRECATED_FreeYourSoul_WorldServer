//
// Created by FyS on 05/03/18.
//

#ifndef FREESOULS_WORLD_PLAYERMANAGER_HH
#define FREESOULS_WORLD_PLAYERMANAGER_HH

#include "SessionManager.hh"

namespace fys::network {

    class PlayerManager : public SessionManager {

    public:
        PlayerManager(const uint size);

        /**
         * \brief Add an entry in the accepted ip/token for new player connection
         * \param ipIncoPlayer
         * \param tokenIncoPlayer
         */
        void addIncomingPlayer(const std::string& ipIncoPlayer, const Token& tokenIncoPlayer);
        /**
         * \brief Add a player connection (call the SessionManager #fys::network::SessionManager::addConnection()) if
         * the given ip is in the accepted incoming player ip (stored in #_incomingPlayer)
         * \param newConnection
         * \return
         */
        const uint addPlayerConnection(const TcpConnection::ptr& newConnection);
        /**
         * \brief Check if the token corresponding to the given ip is correct
         * \param ip used as key to get the accepted token for the incoming player
         * \param token to check
         * \return true if the given token match the accepted one, return false otherwise
         */
        const bool consumePlayerAcceptedToken(const std::string &ip, const Token &token);

    private:
        std::unordered_map<std::string, Token> _incomingPlayer;

    };

}

#endif //FREESOULS_WORLD_PLAYERMANAGER_HH
