//
// Created by FyS on 18/03/18.
//

#ifndef FREESOULS_WORLD_WORLDENGINE_HPP
#define FREESOULS_WORLD_WORLDENGINE_HPP

#include <memory>
#include <vector>

//forward declarations
namespace fys::ws {
    class Map;
    class WorldEngine;
    class MapPosition;
    class PlayerMapData;

    enum class PlayerState : unsigned int;
}

namespace fys::ws {

    class WorldEngine {
        static constexpr int TIME_LOOP = 33;

    public:
        ~WorldEngine() = default;
        WorldEngine(const std::string& tmxMapFilePath);

        void runWorldLoop();

        void changeStatePlayer(const uint idx, const float angle, const PlayerState state);
        void initPlayerPosition(const uint idx, MapPosition &&pos);

    private:
        void updatePlayersPositions(time_t current);
        void increaseObjectPool(const uint minSize);
        int getTimesToMove(const time_t current, const PlayerMapData &playerData);

    private:
        std::unique_ptr<ws::Map> _map;
        std::vector<ws::PlayerMapData> _playersMapData;

    };

}


#endif //FREESOULS_WORLD_WORLDENGINE_HPP
