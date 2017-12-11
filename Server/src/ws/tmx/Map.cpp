//
// Created by FyS on 11/12/17.
//

#include "tmx/Map.hh"

fys::ws::Map::Map() {
}

void fys::ws::Map::setMapElems(std::vector<std::vector<fys::ws::MapElem>> &&mapElems) {
    _mapElems = std::move(mapElems);
}
