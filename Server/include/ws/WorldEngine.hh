//
// Created by FyS on 18/03/18.
//

#ifndef FREESOULS_WORLD_WORLDENGINE_HPP
#define FREESOULS_WORLD_WORLDENGINE_HPP

#include <memory>
#include <vector>
#include <ctime>

#include <PlayerDataType.hh>
#include <WorldServerCluster.hh>

//forward declarations
namespace fys::ws {
    class Map;
    class WorldEngine;
    class MapPosition;

    enum class PlayerState : unsigned int;
}

namespace fys::ws {

    class WorldEngine {

    public:
        ~WorldEngine() = default;
        WorldEngine(const std::string &tmxMapFilePath);

        void runWorldLoop();

        void initPlayerMapData(uint idx, MapPosition &&pos);

        /**
         * \brief A player change its moving state,
         * \warning this method  has to be called from a listener, it is not thread safe so the caller has to ensure
         *          being the only one using it
         * \param idx indexInSession of the gamer session
         * \param timeMove timestamp at which the move is called, if this value is set to 0, the moving state is STOP
         * \param angle angle in degree value, if this value is set to 0, the moving state is STOP
         */
        void changePlayerMovingState(uint idx, double timeMove, double angle);
        void changePlayerMovingState(uint idx);

        /**
         * \brief Check if the player identified by the given index is in a
         * \param idxPlayer index of the player
         * \param cluster neighbour world servers connections
         */
        void notifyNeighbourhoodServer(uint idxPlayer, fys::network::WorldServerCluster &cluster) const;

    private:
        void updatePlayersPositions(double currentTime);

    private:
        std::unique_ptr<ws::Map> _map;
        ws::PlayersMapData _playersMapData;

    };

}


#endif //FREESOULS_WORLD_WORLDENGINE_HPP
