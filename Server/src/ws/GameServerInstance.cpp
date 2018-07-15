//
// Created by FyS on 28/08/17.
//

#include "GameServerInstance.hh"

namespace fys::ws {
    
GameServerInstance::GameServerInstance(const ushort port, const std::string &ip, const std::string &positionId) :
 _port(port), _ip(ip), _positionId(positionId) {}

GameServerInstance::GameServerInstance(GameServerInstance &&other) noexcept :
        _ip(std::move(other._ip)), _port(other._port), _positionId(std::move(other._positionId))
{}

GameServerInstance &GameServerInstance::operator=(GameServerInstance other) {
    std::swap(_ip, other._ip);
    std::swap(_positionId, other._positionId);
    other._port = other._port;
    return *this;
}

void GameServerInstance::setPositionId(const std::string &positionId) {
    _positionId = positionId;
}

const std::string &GameServerInstance::getPositionId() const {
    return _positionId;
}

const std::string &GameServerInstance::getIp() const {
    return _ip;
}

void GameServerInstance::setIp(const std::string &ip) {
    GameServerInstance::_ip = ip;
}

ushort GameServerInstance::getPort() const {
    return _port;
}

void GameServerInstance::setPort(const ushort port) {
    GameServerInstance::_port = port;
}

}