//
// Created by FyS on 11/12/17.
//

#ifndef FREESOULS_MAP_HH
#define FREESOULS_MAP_HH

#include <functional>
#include <variant>
#include <cmath>

static constexpr char COLLISION_LAYER[] = "Collision";

namespace tmx {
    class TileLayer;
}

namespace fys::ws {

    class WorldServer;

    /**
     * Type of element findable in a map
     * NI = No Interaction, those are empty elements
     */
    enum MapElemProperty {
        // element can block the path
        BLOCK,
        // element contain a trigger for an action
        TRIGGER,
        // nothing happen on this element
        NI
    };

    /**
     * Represent a logical tile
     * Has a property
     */
    class MapElem {
        using Trigger = std::variant<
                std::function<void (uint, uint)>,
                std::function<void (uint, std::weak_ptr<WorldServer>)>
        >;

    public:
        void setTypeElem(MapElemProperty type) {
            this->type = type;
        }

        void setLevel(const unsigned l) {
            levelFlags = static_cast<unsigned char>(std::pow(l, 2));
        }

        bool isLevel(const unsigned l) {
            return (levelFlags & static_cast<unsigned char>(std::pow(l, 2))) == static_cast<unsigned char>(std::pow(l, 2));
        }

    private:
        /**
         * bitmask
         * Highness of the ground elem
         */
        unsigned char levelFlags = 0;
        /**
         * Physical property of the map elem
         */
        MapElemProperty type = MapElemProperty::NI;
        /**
         * Potential trigger occured because of the map elem
         */
        std::unique_ptr<Trigger> trigger = nullptr;

    };


    class Map {

    public:
        using uptr = std::unique_ptr<Map>;

        Map(const std::string &tmxFileName);

    private:
        /**
         * \brief Just initialize the _mapElems at the correct size
         * \param map
         */
        void initCollisionMapFromLayer(const unsigned xMap, const unsigned  yMap, const tmx::TileLayer &tileLayer);

    private:
        std::vector<std::vector<MapElem>> _mapElems;

    };

}

#endif //FREESOULS_MAP_HH
