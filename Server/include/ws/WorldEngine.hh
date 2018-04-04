//
// Created by FyS on 18/03/18.
//

#ifndef FREESOULS_WORLD_WORLDENGINE_HPP
#define FREESOULS_WORLD_WORLDENGINE_HPP

#include <memory>
#include <vector>
#include <ctime>
#include <PlayerDataType.hh>

//forward declarations
namespace fys::ws {
    class Map;
    class WorldEngine;
    class MapPosition;

    enum class PlayerState : unsigned int;
}

namespace fys::ws {

    class WorldEngine {
        static constexpr double TIME_WORLD_LOOP = 100;
        static constexpr double GAME_PACE = 16.6666;

    public:
        ~WorldEngine() = default;
        WorldEngine(const std::string &tmxMapFilePath);

        void runWorldLoop();

        void initPlayerPosition(uint idx, MapPosition &&pos);

        /**
         * \brief A player change its moving state,
         * \warning this method  has to be called from a listener, it is not thread safe so the caller has to ensure
         *          being the only one using it
         * \param idx indexInSession of the gamer session
         * \param timeMove timestamp at which the move is called, if this value is set to 0, the moving state is STOP
         * \param angle angle in degree value, if this value is set to 0, the moving state is STOP
         */
        void changePlayerMovingState(uint idx, double timeMove = 0, double angle = 0);


    private:
        void updatePlayersPositions(double currentTime, double &lag);

    private:
        std::unique_ptr<ws::Map> _map;
        ws::PlayersMapData _playersMapData;

    };

}


#endif //FREESOULS_WORLD_WORLDENGINE_HPP
