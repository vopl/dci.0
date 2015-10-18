#pragma once

#include <dci/site/api.hpp>
#include <system_error>

namespace dci { namespace site
{
    struct APIDCI_SITE ServiceFactory
    {
        virtual ~ServiceFactory();
        virtual void createService(void *outFuture) = 0;
    };
}}
