#include "map.hpp"
#include <dci/couple/runtime/signBuilder.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
{
    Map::Map()
        : Container2(TypeConcrete::map)
    {
    }

    Map::~Map()
    {

    }

    void Map::makeSign()
    {
        runtime::SignBuilder s;

        s.add("map");
        s.add(_elementType1->concreteSign());
        s.add(_elementType2->concreteSign());

        setSign(s.finish());
    }

}}}}
