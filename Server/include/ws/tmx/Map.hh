//
// Created by FyS on 11/12/17.
//

#ifndef FREESOULS_MAP_HH
#define FREESOULS_MAP_HH

#include <memory>
#include <vector>
#include <functional>
#include <variant>

namespace fys::ws {

    class WorldServer;

    /**
     * Type of element findable in a map
     * NI = No Interaction, those are empty elements
     */
    enum MapElemProperty {
        BLOCK,
        TRIGGER,
        NI
    };

    /**
     * Represent a logical tile
     * Has a property
     */
    struct MapElem {
        using Trigger = std::variant<
                std::function<void (uint, uint)>,
                std::function<void (uint, std::weak_ptr<WorldServer>)>
        >;

        MapElemProperty type = MapElemProperty::NI;
        std::unique_ptr<Trigger> trigger = nullptr;
    };


    class Map {

    public:
        using uptr = std::unique_ptr<Map>;

        Map(std::string &&tmxFileName);

        void setMapElems(std::vector<std::vector<MapElem> >&&);

    private:
        std::vector<std::vector<MapElem> > _mapElems;

    };

}

#endif //FREESOULS_MAP_HH
