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
    double timeEpochStart = 0;
    double timeEpochEnd = 0;
    double previousStart = 0;

    while (true) {
        auto start = std::chrono::high_resolution_clock::now();
        timeEpochStart = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();

        this->updatePlayersPositions(timeEpochStart, previousStart);

        auto end = std::chrono::high_resolution_clock::now();
        timeEpochEnd = std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch()).count();
        double sleepTime = (timeEpochStart + TIME_WORLD_LOOP) - timeEpochEnd;
        if (sleepTime > 0) {
            std::chrono::duration<double, std::milli> dur(sleepTime);
            std::this_thread::sleep_for(dur);
        }
        previousStart = timeEpochStart;
    }
}

void fys::ws::WorldEngine::updatePlayersPositions(double current, double previous) {
    for (PlayerMapData &p : _playersMapData) {
        if (this->hasToMove(current, p)) {
            float futureX = static_cast<float>(p._pos.x + (((current - previous) / 100) * (p._velocity.speed * std::cos(p._velocity.angle))));
            float futureY = static_cast<float>(p._pos.y + (((current - previous) / 100) * (p._velocity.speed * std::sin(p._velocity.angle))));
            MapElemProperty prop = _map->getMapElementPropertyAtPosition(futureX, futureY);

            std::printf("x %f y %f speed %f angle %f cos %f \n",
                        p._pos.x* 24, p._pos.y* 24, p._velocity.speed, p._velocity.angle,
                        p._velocity.speed * std::cos(p._velocity.angle));
            std::printf("fx %f fy %f %lf\n",
                        futureX * 24, futureY* 24, (current - previous));
            if (prop != MapElemProperty::BLOCK) {
                p._pos.x = futureX;
                p._pos.y = futureY;
                if (prop == MapElemProperty::TRIGGER)
                    _map->triggerForPlayer(futureX, futureY, p);
            }
            else
                spdlog::get("c")->critical("BING YOUPI");
        }
    }
}

inline bool fys::ws::WorldEngine::hasToMove(double currentInMilliseconds, PlayerMapData &playerData) const {
    return playerData._executeActionTime - currentInMilliseconds <= 0;
}

void fys::ws::WorldEngine::initPlayerPosition(uint idx, fys::ws::MapPosition &&pos) {
    if (idx >= _playersMapData.size())
        increaseObjectPool(idx);
    fys::ws::PlayerMapData pmd;

    pmd._state = PlayerState::MOVE_OFF;
    pmd._pos = std::move(pos);
    _playersMapData.at(idx) = std::move(pmd);
}

void fys::ws::WorldEngine::increaseObjectPool(uint minSize) {
    _playersMapData.resize(minSize + 100);
}

void fys::ws::WorldEngine::changePlayerMovingState(uint idx, double timeMove, double angle) {
    if (timeMove && (angle == _playersMapData.at(idx)._velocity.angle ||
                     _playersMapData.at(idx)._executeActionTime < std::numeric_limits<double>::max())) {
        _playersMapData.at(idx)._executeActionTime = timeMove + GAME_PACE;
        _playersMapData.at(idx)._state = fys::ws::PlayerState::MOVE_ON;
    }
    else if (!timeMove) {
        _playersMapData.at(idx)._executeActionTime = std::numeric_limits<double>::max();
        _playersMapData.at(idx)._state = fys::ws::PlayerState::MOVE_OFF;
    }
    _playersMapData.at(idx)._velocity.angle = static_cast<float>(angle);
}
