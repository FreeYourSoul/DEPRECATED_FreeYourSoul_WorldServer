//
// Created by FyS on 18/03/18.
//

#include <spdlog/spdlog.h>

#include <thread>
#include <chrono>

#include <Map.hh>
#include <PlayerDataType.hh>
#include <PlayerManager.hh>
#include "WorldEngine.hh"

namespace fys::ws {

static inline double getCurrentTimeInMillisec() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

WorldEngine::WorldEngine(const std::string &tmxMapFilePath) :
        _map(std::make_unique<Map>(tmxMapFilePath)),
        _playersMapData() {
}

void WorldEngine::runWorldLoop() {
    double timeEpochStart;

    while (true) {
        timeEpochStart = getCurrentTimeInMillisec();

        this->updatePlayersPositions(timeEpochStart);

        const double sleepTime = (timeEpochStart + TIME_WORLD_LOOP) - getCurrentTimeInMillisec();
        if (sleepTime > 0) {
            std::chrono::duration<double, std::milli> dur(sleepTime);
            std::this_thread::sleep_for(dur);
        }
    }
}

void WorldEngine::updatePlayersPositions(double currentTime) {
    for (uint idx = 0; idx < _playersMapData.playersSize(); ++idx) {
        for (Velocity actionVelocity : _playersMapData._actionsExec.at(idx).getActionsToExecute()) {
            float futureX = _playersMapData._pos.at(idx).x + (actionVelocity.speed * std::cos(actionVelocity.angle));
            float futureY = _playersMapData._pos.at(idx).y + (actionVelocity.speed * std::sin(actionVelocity.angle));
            MapElemProperty prop = _map->getMapElementPropertyAtPosition(futureX, futureY);

//            std::printf("x %f y %f speed %f\n fx %f fy %f\n currentTime %f\n\n",
//                        _playersMapData._pos.at(idx).x * 24, _playersMapData._pos.at(idx).y * 24, actionVelocity.speed,
//                        futureX * 24, futureY * 24, currentTime);

            if (prop != MapElemProperty::BLOCK) {
                _playersMapData._pos.at(idx).x = futureX;
                _playersMapData._pos.at(idx).y = futureY;
                if (prop == MapElemProperty::TRIGGER)
                    _map->triggerForPlayer(futureX, futureY, _playersMapData);
            }
            else
                spdlog::get("c")->critical("BING YOUPI");
        }
    }
}

void WorldEngine::initPlayerMapData(uint idx, MapPosition &&pos) {
    if (idx >= _playersMapData.playersSize()) {
        _playersMapData._actionsExec.resize(_playersMapData.playersSize() + 100);
        _playersMapData._pos.resize(_playersMapData.playersSize() + 100);
    }
    _playersMapData._pos.at(idx) = std::move(pos);
}

void WorldEngine::changePlayerMovingState(uint idx, double timeMove, double angle) {
    _playersMapData._actionsExec.at(idx).addAction(timeMove + GAME_PACE, static_cast<float>(angle));
}

void WorldEngine::changePlayerMovingState(uint idx) {
}

    void WorldEngine::notifyNeighbourhoodServer(uint idxPlayer, fys::network::WorldServerCluster &cluster) const {
    const MapPosition &pos = _playersMapData._pos.at(idxPlayer);
}

}
