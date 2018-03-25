//
// Created by FyS on 18/03/18.
//

#include <spdlog/spdlog.h>
#include <thread>
#include <Map.hh>
#include <chrono>
#include <PlayerDataType.hh>
#include <PlayerManager.hh>
#include "WorldEngine.hh"

fys::ws::WorldEngine::WorldEngine(const std::string &tmxMapFilePath) :
        _map(std::make_unique<fys::ws::Map>(tmxMapFilePath)),
        _playersMapData(network::PlayerManager::CONNECTION_NUMBER){
}

void fys::ws::WorldEngine::runWorldLoop() {
    while (true) {
        std::time_t current = std::time(nullptr);

        this->updatePlayersPositions(current);

        std::time_t endTick = std::time(nullptr);
        std::time_t durationSleep = (((current * 1000) + TIME_WORLD_LOOP) - (endTick * 1000));
        if (durationSleep > 0) {
            std::chrono::duration<std::time_t, std::milli> dur(durationSleep);
            std::this_thread::sleep_for(dur);
        }
    }
}

void fys::ws::WorldEngine::updatePlayersPositions(std::time_t current) {
    for (PlayerMapData &p : _playersMapData) {
        if (p._state != PlayerState::MOVE_OFF) {
            int timeMove = this->getTimesToMove(current * 1000, p);

            float futureX = p._pos.x + (p._velocity.speed * std::cos(p._velocity.angle));
            float futureY = p._pos.y + (p._velocity.speed * std::sin(p._velocity.angle));
            MapElemProperty prop = _map->getMapElementPropertyAtPosition(futureX, futureY);

            std::printf("fx %f fy %f prop %d times %d\n", p._pos.x, p._pos.x, timeMove);
            if (prop != MapElemProperty::BLOCK) {
                do {
                    p._pos.x = futureX;
                    p._pos.y = futureY;
                    if (prop == MapElemProperty::TRIGGER)
                        _map->triggerForPlayer(futureX, futureY, p);
                    if (timeMove > 1) {
                        futureX = p._pos.x + (p._velocity.speed * std::cos(p._velocity.angle));
                        futureY = p._pos.y + (p._velocity.speed * std::sin(p._velocity.angle));
                    }
                } while (--timeMove > 0);
            }
            else
                spdlog::get("c")->critical("BING YOUPI");
        }
    }
}

int fys::ws::WorldEngine::getTimesToMove(const time_t currentInMillisec, const fys::ws::PlayerMapData &playerData) {
    time_t timeLastMove = playerData._initRequestTime;
    if (playerData._lastTimeMoved > 0)
        timeLastMove = playerData._lastTimeMoved;
    std::printf("current %ld lastTimeMoved %ld\n", currentInMillisec, timeLastMove);
    return static_cast<int>((currentInMillisec -  timeLastMove) / GAME_PACE);
}

void fys::ws::WorldEngine::initPlayerPosition(uint idx, fys::ws::MapPosition &&pos) {
    if (idx >= _playersMapData.size())
        increaseObjectPool(idx);
    fys::ws::PlayerMapData pmd;

    pmd._state = PlayerState::MOVE_OFF;
    pmd._pos = std::move(pos);
    _playersMapData.at(idx) = std::move(pmd);
}

void fys::ws::WorldEngine::changeStatePlayer(uint idx, float angle, const PlayerState state) {
    if (idx < _playersMapData.size()) {
        spdlog::get("c")->error("A player at index {} tried to change moving status ({}) with angle."
                                "Pool of current player state is {}", idx, static_cast<unsigned int>(state), _playersMapData.size());
    }
    else {
        if (state == PlayerState::MOVE_ON)
            _playersMapData.at(idx)._velocity.angle = angle;
        _playersMapData.at(idx)._state = state;
    }
}

void fys::ws::WorldEngine::increaseObjectPool(uint minSize) {
    _playersMapData.resize(minSize + 100);
}

void fys::ws::WorldEngine::changePlayerMovingState(uint idx, const time_t timeMove, double angle) {
    _playersMapData.at(idx)._initRequestTime = timeMove;
    _playersMapData.at(idx)._velocity.angle = static_cast<float>(angle);
    _playersMapData.at(idx)._state = (timeMove) ? fys::ws::PlayerState::MOVE_ON : fys::ws::PlayerState::MOVE_OFF;
}
