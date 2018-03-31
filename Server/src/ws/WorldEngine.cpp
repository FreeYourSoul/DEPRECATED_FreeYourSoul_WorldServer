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

static inline auto getCurrentTimeInMillisec() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

fys::ws::WorldEngine::WorldEngine(const std::string &tmxMapFilePath) :
        _map(std::make_unique<fys::ws::Map>(tmxMapFilePath)),
        _playersMapData(network::PlayerManager::CONNECTION_NUMBER){
}

void fys::ws::WorldEngine::runWorldLoop() {
    double timeEpochStart = 0;
    double previousStart = 0;
    double lag = 0;

    while (true) {
        timeEpochStart = getCurrentTimeInMillisec();
        lag += (timeEpochStart - previousStart);

        this->updatePlayersPositions(timeEpochStart, lag);

        double sleepTime = (timeEpochStart + TIME_WORLD_LOOP) - getCurrentTimeInMillisec();
        if (sleepTime > 0) {
            std::chrono::duration<double, std::milli> dur(sleepTime);
            std::this_thread::sleep_for(dur);
        }
        previousStart = timeEpochStart;
    }
}

void fys::ws::WorldEngine::updatePlayersPositions(double currentTime, double &lag) {
    for (PlayerMapData &p : _playersMapData) {
        do {
            if (this->hasToMove(currentTime, p)) {
                float futureX = p._pos.x + (p._velocity.speed * std::cos(p._velocity.angle));
                float futureY = p._pos.y + (p._velocity.speed * std::sin(p._velocity.angle));
                MapElemProperty prop = _map->getMapElementPropertyAtPosition(futureX, futureY);

                std::printf("x %f y %f speed %f\n fx %f fy %f\n currentTime %f actionTime %f\n\n",
                            p._pos.x* 24, p._pos.y* 24, p._velocity.speed,
                            futureX, futureY, currentTime, p._executeActionTime);
                if (prop != MapElemProperty::BLOCK) {
                    p._pos.x = futureX;
                    p._pos.y = futureY;
                    if (prop == MapElemProperty::TRIGGER)
                        _map->triggerForPlayer(futureX, futureY, p);
                }
                else
                    spdlog::get("c")->critical("BING YOUPI");
            }
            lag -= GAME_PACE;
        } while (lag >= GAME_PACE);
    }
}

inline bool fys::ws::WorldEngine::hasToMove(double currentInMilliseconds, PlayerMapData &playerData) const {
    if (playerData._isMoving) {
        playerData._isMoving = false;
        return currentInMilliseconds <= playerData._executeActionTime;
    }
    return false;
}

void fys::ws::WorldEngine::initPlayerPosition(uint idx, fys::ws::MapPosition &&pos) {
    if (idx >= _playersMapData.size())
        increaseObjectPool(idx);
    fys::ws::PlayerMapData pmd;

    pmd._isMoving = false;
    pmd._pos = std::move(pos);
    _playersMapData.at(idx) = pmd;
}

void fys::ws::WorldEngine::increaseObjectPool(uint minSize) {
    _playersMapData.resize(minSize + 100);
}

void fys::ws::WorldEngine::changePlayerMovingState(uint idx, double timeMove, double angle) {
    if (timeMove && (angle == _playersMapData.at(idx)._velocity.angle ||
                     _playersMapData.at(idx)._executeActionTime > 0)) {
        _playersMapData.at(idx)._executeActionTime = timeMove + GAME_PACE;
        _playersMapData.at(idx)._isMoving = true;
    }
    else if (!timeMove) {
        _playersMapData.at(idx)._executeActionTime = 0;
    }
    _playersMapData.at(idx)._velocity.angle = static_cast<float>(angle);
}
