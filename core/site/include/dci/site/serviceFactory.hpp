#pragma once

#include <system_error>

namespace dci { namespace site
{
    struct ServiceFactory
    {
        virtual ~ServiceFactory() = 0;
        virtual void createService(void *outFuture) = 0;
    };

    inline ServiceFactory::~ServiceFactory(){}
}}
