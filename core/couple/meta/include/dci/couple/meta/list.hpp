#pragma once

#include "signed.hpp"
#include "container1.hpp"

namespace dci { namespace couple { namespace meta
{
    class List
        : public himpl::FaceLayout<impl::List, Signed, Container1>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        List() = delete;
        ~List() = delete;

        static const TypeConcrete _concrete = TypeConcrete::list;

    };

}}}
