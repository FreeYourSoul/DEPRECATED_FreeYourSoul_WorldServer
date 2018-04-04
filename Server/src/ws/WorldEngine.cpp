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
        _playersMapData() {
}

void fys::ws::WorldEngine::runWorldLoop() {
    double timeEpochStart = getCurrentTimeInMillisec();
    double previousStart = timeEpochStart;
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
    for (uint idx = 0; idx < _playersMapData.playersSize(); ++idx) {
        do {
            if (this->hasToMove(idx, currentTime)) {
                float futureX = _playersMapData._pos.at(idx).x + (_playersMapData._velocity.at(idx).speed *
                                                                  std::cos(_playersMapData._velocity.at(idx).angle));
                float futureY = _playersMapData._pos.at(idx).y + (_playersMapData._velocity.at(idx).speed *
                                                                  std::sin(_playersMapData._velocity.at(idx).angle));
                MapElemProperty prop = _map->getMapElementPropertyAtPosition(futureX, futureY);

                std::printf("x %f y %f speed %f\n fx %f fy %f\n currentTime %f actionTime %f\nlag %f\n\n",
                            _playersMapData._pos.at(idx).x* 24, _playersMapData._pos.at(idx).y* 24, _playersMapData._velocity.at(idx).speed,
                            lag, futureX, futureY, currentTime, _playersMapData._executeActionTime);

                if (prop != MapElemProperty::BLOCK) {
                    _playersMapData._pos.at(idx).x = futureX;
                    _playersMapData._pos.at(idx).y = futureY;
                    if (prop == MapElemProperty::TRIGGER)
                        _map->triggerForPlayer(futureX, futureY, _playersMapData);
                }
                else
                    spdlog::get("c")->critical("BING YOUPI");
            }
            if (lag - GAME_PACE > 0)
                lag -= GAME_PACE;
        } while (lag >= GAME_PACE);
    }
}

bool fys::ws::WorldEngine::hasToMove(uint idx, double currentInMilliseconds) {
    if (_playersMapData._isMoving.at(idx)) {
        _playersMapData._isMoving[idx] = false;
        return currentInMilliseconds <= _playersMapData._executeActionTime.at(idx);
    }
    return false;
}

void fys::ws::WorldEngine::initPlayerPosition(uint idx, fys::ws::MapPosition &&pos) {
    if (idx >= _playersMapData.playersSize()) {
        _playersMapData._isMoving.resize(_playersMapData.playersSize() + 100, false);
        _playersMapData._executeActionTime.resize(_playersMapData.playersSize() + 100, 0);
        _playersMapData._velocity.resize(_playersMapData.playersSize() + 100);
        _playersMapData._pos.resize(_playersMapData.playersSize() + 100);
    }
    _playersMapData._pos.at(idx) = std::move(pos);
}

void fys::ws::WorldEngine::changePlayerMovingState(uint idx, double timeMove, double angle) {
    if (timeMove && (angle == _playersMapData._velocity.at(idx).angle ||
                     _playersMapData._executeActionTime.at(idx) > 0)) {
        _playersMapData._executeActionTime.at(idx) = timeMove + GAME_PACE;
        _playersMapData._isMoving.at(idx) = true;
    }
    else if (!timeMove) {
        _playersMapData._executeActionTime.at(idx) = 0;
    }
    _playersMapData._velocity.at(idx).angle = static_cast<float>(angle);
}
