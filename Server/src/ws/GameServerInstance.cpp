//
// Created by FyS on 28/08/17.
//

#include "GameServerInstance.hh"

fys::ws::GameServerInstance::GameServerInstance(const ushort port, const std::string &ip, const std::string &positionId) :
 _port(port), _ip(ip), _positionId(positionId) {}

fys::ws::GameServerInstance::GameServerInstance(fys::ws::GameServerInstance &&other) noexcept :
        _ip(std::move(other._ip)), _port(other._port), _positionId(std::move(other._positionId))
{}

fys::ws::GameServerInstance &fys::ws::GameServerInstance::operator=(fys::ws::GameServerInstance other) {
    std::swap(_ip, other._ip);
    std::swap(_positionId, other._positionId);
    other._port = other._port;
    return *this;
}

void fys::ws::GameServerInstance::setPositionId(const std::string &positionId) {
    _positionId = positionId;
}

const std::string &fys::ws::GameServerInstance::getPositionId() const {
    return _positionId;
}

const std::string &fys::ws::GameServerInstance::getIp() const {
    return _ip;
}

void fys::ws::GameServerInstance::setIp(const std::string &ip) {
    GameServerInstance::_ip = ip;
}

ushort fys::ws::GameServerInstance::getPort() const {
    return _port;
}

void fys::ws::GameServerInstance::setPort(const ushort port) {
    GameServerInstance::_port = port;
}
