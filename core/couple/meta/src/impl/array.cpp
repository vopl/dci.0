#include "array.hpp"

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

}}}}
