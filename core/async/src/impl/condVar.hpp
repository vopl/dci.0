#pragma once

#include "waitable.hpp"

namespace dci { namespace async { namespace impl
{

    class CondVar
        : public Waitable
    {
    public:
        CondVar();
        ~CondVar();
    };

}}}
