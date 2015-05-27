#pragma once

#include <dci/himpl/layoutProvider.hpp>
#include LAYOUTPROVIDERFILE("layoutProvider.hpp")
#include <dci/himpl/faceLayout.hpp>

#include "api.hpp"
#include <string>

namespace dci { namespace couple { namespace meta
{
    class APIDCI_COUPLE_META Named
        : public himpl::FaceLayout<impl::Named>
    {
    public:
        using ThisFaceLayout::ThisFaceLayout;
        Named() = delete;
        ~Named() = delete;

        //имя
        const std::string &name() const;

    };

}}}
