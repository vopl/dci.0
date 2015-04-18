#pragma once

#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")

#include <dci/himpl/faceLayout.hpp>

#include <vector>
#include <string>

namespace dci { namespace couple { namespace meta
{
    template <class E>
    class Compound
        : public himpl::FaceLayout<impl::Compound<E>>
    {
    public:
        using himpl::FaceLayout<impl::Compound<E>>::FaceLayout;
        Compound() = delete;
        ~Compound() = delete;

        const std::vector<const E *> &elements() const;
    };

}}}
