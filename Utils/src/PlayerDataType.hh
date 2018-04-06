//
// Created by FyS on 18/03/18.
//

#ifndef FREESOULS_WORLD_PLAYERMAPDATA_HH
#define FREESOULS_WORLD_PLAYERMAPDATA_HH

#include <atomic>
#include <zconf.h>
#include <ctime>
#include <list>
#include <algorithm>
#include <PlayerManager.hh>

namespace fys::ws {
    static constexpr double TIME_WORLD_LOOP = 100;
    static constexpr double GAME_PACE = 16.6666;
    static constexpr int MAX_ACTION_PER_CYCLE = static_cast<const int>(TIME_WORLD_LOOP / GAME_PACE) + 1;


    struct Velocity {
        float speed = 0.025;
        float angle = 0.0;
    };

    struct VelocityOnTime : Velocity {
        double time;
    };

    struct MapPosition {
        float x = 0.0;
        float y = 0.0;
    };

    /**
     * \brief Class representing a pool of actions, this pool is limited by the number represented by the anonymous
     * inner-enum NUMBER_ACTION_ACCEPTED.
     * \note
     * In order to differentiate executed actions and pending actions, a vector of previous executed actions is stored.
     * If the main vector (_timeToExecute) is different at a certain index than the previous action at the same index,
     * it means that the action has not been executed yet and has to be processed
     */
    class ExecutionActions {
        enum {
            NUMBER_ACTION_ACCEPTED = MAX_ACTION_PER_CYCLE
        };

    public:
        ExecutionActions() :
                _timeToExecute(NUMBER_ACTION_ACCEPTED, 0),
                _prevExecuted(NUMBER_ACTION_ACCEPTED, 0),
                _velocityAction(NUMBER_ACTION_ACCEPTED),
                _cursor(0) {}

        void addAction(double executionTime, float angle) {
            _timeToExecute.at(_cursor) = executionTime;
            _velocityAction.at(_cursor).angle = angle;
            _cursor = (_cursor + 1) % NUMBER_ACTION_ACCEPTED;
        }

        std::vector<Velocity> getActionsToExecute() const {
            std::vector<Velocity> toRet;

            for (unsigned i = 0; i < _timeToExecute.size(); ++i) {
                if (_timeToExecute.at(i) != _prevExecuted.at(i)) {
                    VelocityOnTime vot;
                    vot.angle = _velocityAction.at(i).angle;
                    vot.speed = _velocityAction.at(i).speed;
                    vot.time = _timeToExecute.at(i);
                    _prevExecuted.at(i) = _timeToExecute.at(i);
                    toRet.push_back(std::move(vot));
                }
            }
            if (!toRet.empty())
                std::sort(toRet.begin(), toRet.end(), [](const Velocity &lhs, const Velocity &rhs) -> bool {
                    return static_cast<const VelocityOnTime&>(lhs).time < static_cast<const VelocityOnTime&>(rhs).time;
                });
            return toRet;
        }

    private:
        std::vector<double> _timeToExecute;
        mutable std::vector<double> _prevExecuted;
        std::vector<Velocity> _velocityAction;
        uint _cursor;
    };

    /**
     * Data Oriented structure, contains only vector member, the index of each element represent a different player
     */
    struct PlayersMapData {
        /**
         * A vector representing the position of each player on the map
         */
        std::vector<MapPosition> _pos;
        /**
         * A vector representing Actions to executes on the player
         */
        std::vector<ExecutionActions> _actionsExec;

        PlayersMapData() :
                _pos(network::PlayerManager::CONNECTION_NUMBER),
                _actionsExec(network::PlayerManager::CONNECTION_NUMBER) {}

        std::size_t playersSize() const {
            return _pos.size();
        }
    };

}


#endif //FREESOULS_WORLD_PLAYERMAPDATA_HH
