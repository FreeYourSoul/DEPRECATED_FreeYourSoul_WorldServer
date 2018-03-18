//
// Created by FyS on 18/03/18.
//

#ifndef FREESOULS_WORLD_WORLDENGINE_HPP
#define FREESOULS_WORLD_WORLDENGINE_HPP

#include <memory>

namespace fys {
    namespace ws {
        class Map;
        class WorldEngine;
    }
}

namespace fys::ws {

    class WorldEngine {

    public:
        ~WorldEngine() = default;
        WorldEngine(const std::string& tmxMapFilePath);

    private:

    private:
        std::unique_ptr<ws::Map> _map;
    };

}


#endif //FREESOULS_WORLD_WORLDENGINE_HPP
