//
// Created by FyS on 18/03/18.
//

#ifndef FREESOULS_WORLD_WORLDENGINE_HPP
#define FREESOULS_WORLD_WORLDENGINE_HPP

#include <PlayerDataType.hh>
#include <memory>

namespace fys::ws {
    class Map;
    class WorldEngine;
}

namespace fys::ws {

    class WorldEngine {
        constexpr auto TIME_LOOP = std::chrono::milliseconds(33);

    public:
        ~WorldEngine() = default;
        WorldEngine(const std::string& tmxMapFilePath);

        void runWorldLoop();

        void changeStatePlayer(const uint idx, const float angle, const PlayerState state);
        void initPlayerPosition(const uint idx, MapPosition &&pos);

    private:
        void updatePlayersPositions(time_t lag);
        void increaseObjectPool(const uint minSize);

    private:
        std::unique_ptr<ws::Map> _map;
        std::vector<ws::PlayerMapData> _playersMapData;

    };

}


#endif //FREESOULS_WORLD_WORLDENGINE_HPP
