#pragma once

#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")

#include <dci/himpl/faceLayout.hpp>
#include "api.hpp"
#include <vector>

namespace dci { namespace couple { namespace meta
{
    template <class B>
    class APIDCI_COUPLE_META Inheritable
        : public himpl::FaceLayout<impl::Inheritable<B>>
    {
    public:
        using himpl::FaceLayout<impl::Inheritable<B>>::FaceLayout;
        Inheritable() = delete;
        ~Inheritable() = delete;

        const std::vector<const B *> &bases() const;

    };

}}}
