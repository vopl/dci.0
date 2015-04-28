#include "array.hpp"
#include <dci/couple/runtime/signBuilder.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
{
    Array::Array()
        : Container1(TypeConcrete::array)
    {
    }

    Array::~Array()
    {

    }

    void Array::setSize(std::uint32_t v)
    {
        _size = v;
    }

    std::uint32_t Array::size() const
    {
        return _size;
    }

    void Array::makeSign()
    {
        runtime::SignBuilder s;

        s.add("array");
        s.add(_elementType->concreteSign());
        s.add(size());

        setSign(s.finish());
    }

}}}}
