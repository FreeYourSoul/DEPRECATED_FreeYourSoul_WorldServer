//
// Created by FyS on 18/03/18.
//

#include <Map.hh>
#include <PlayerManager.hh>
#include <PlayerDataType.hh>
#include "WorldEngine.hh"

fys::ws::WorldEngine::WorldEngine(const std::string &tmxMapFilePath) :
        _map(std::make_unique<fys::ws::Map>(tmxMapFilePath)),
        _playersMapData(network::PlayerManager::CONNECTION_NUMBER){
}
