//
// Created by FyS on 05/03/18.
//

#ifndef FREESOULS_WORLD_PLAYERMANAGER_HH
#define FREESOULS_WORLD_PLAYERMANAGER_HH

#include "SessionManager.hh"

namespace fys::network {

    // forward declaration
    class TcpConnection;

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
        const uint addPlayerConnection(const std::shared_ptr<TcpConnection>& newConnection);
        /**
         * \brief Check if the token corresponding to the given ip is correct and connect the user
         * \note Connect the user thanks to the given token. This token has to match the one given during the incoming
         * player registering phase (by call of #addIncomingPlayer(const std::string&, const Token&) ). If it
         * doesn't match, the player's session is disconnected because of failed authentication.
         * \param indexInSession used to find the ip of the user
         * \param token to check
         * \return true if the given token match the accepted one, return false otherwise
         */
        const bool connectPlayerWithToken(const uint indexInSession, const Token &token);

        const bool isAuthenticated(const uint indexInSession) const;

    private:
        std::unordered_map<std::string, Token> _incomingPlayer;

    };

}

#endif //FREESOULS_WORLD_PLAYERMANAGER_HH