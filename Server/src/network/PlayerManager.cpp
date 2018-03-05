//
// Created by FyS on 05/03/18.
//

#include <spdlog/spdlog.h>
#include "PlayerManager.hh"

fys::network::PlayerManager::PlayerManager(const uint size) : SessionManager(size), _incomingPlayer(size) {}

void fys::network::PlayerManager::addIncomingPlayer(const std::string &ipIncoPlayer, const Token& tokenIncoPlayer) {
    _incomingPlayer[ipIncoPlayer] = tokenIncoPlayer;
}

const uint fys::network::PlayerManager::addPlayerConnection(const fys::network::TcpConnection::ptr &newConnection) {
    auto findIt = _incomingPlayer.find(newConnection->getIpAddress());

    if (findIt == _incomingPlayer.end()) {
        spdlog::get("c")->warn("The player isn't registered on the accepted ip list");
        return 0;
    }
    return addConnection(newConnection);
}

const bool fys::network::PlayerManager::consumePlayerAcceptedToken(const std::string &ip, const Token &token) {
    auto findIt = _incomingPlayer.find(ip);

    if (findIt != _incomingPlayer.end()) {
        if (std::equal(token.begin(), token.end(), _incomingPlayer.at(ip).begin())) {
            _incomingPlayer.erase(findIt);
            return true;
        }
        spdlog::get("c")->warn("The given token is wrong for ip {}", ip);
    }
    spdlog::get("c")->error("The given ip {} is not registered as accepted ip, "
                                    "this should have already been checked!", ip);
    return false;
}


