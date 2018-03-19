//
// Created by FyS on 18/03/18.
//

#include <spdlog/spdlog.h>
#include <PlayerManager.hh>
#include <Map.hh>
#include "WorldEngine.hh"

fys::ws::WorldEngine::WorldEngine(const std::string &tmxMapFilePath) :
        _map(std::make_unique<fys::ws::Map>(tmxMapFilePath)),
        _playersMapData(network::PlayerManager::CONNECTION_NUMBER){
}

void fys::ws::WorldEngine::runWorldLoop() {
    std::time_t lastTime = std::time(nullptr);
    std::time_t accumulatedLag = 0;

    while (true) {
        std::time_t current = std::time(nullptr);
        std::time_t elapsed = current - lastTime;
        lastTime = current;
        accumulatedLag += elapsed;

        updatePlayersPositions(elapsed);

        std::time_t endTick = std::time(nullptr);
        std::time_t durationSleep = (((current * 1000) + TIME_LOOP) - (endTick * 1000));
        std::chrono::duration<std::time_t, std::milli> d(durationSleep);
        std::this_thread::sleep_for(d);
    }
}

void fys::ws::WorldEngine::updatePlayersPositions(time_t elapsed) {

}

void fys::ws::WorldEngine::initPlayerPosition(const uint idx, fys::ws::MapPosition &&pos) {
    if (idx >= _playersMapData.size())
        increaseObjectPool(idx);
    fys::ws::PlayerMapData pmd;

    pmd._state = PlayerState::MOVE_OFF;
    pmd._pos = std::move(pos);
    _playersMapData.at(idx) = std::move(pmd);
}

void fys::ws::WorldEngine::changeStatePlayer(const uint idx, const float angle, const PlayerState state) {
    if (idx < _playersMapData.size()) {
        spdlog::get("c")->error("A player at index {} tried to change moving status ({}) with angle."
                                "Pool of current player state is {}", idx, state, _playersMapData.size());
    }
    else {
        if (state == PlayerState::MOVE_ON)
            _playersMapData.at(idx)._velocity.angle = angle;
        _playersMapData.at(idx)._state = state;
    }
}

void fys::ws::WorldEngine::increaseObjectPool(const uint minSize) {
    _playersMapData.resize(minSize + 100);
}
