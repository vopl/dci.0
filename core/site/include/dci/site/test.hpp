#pragma once

#include "api.hpp"

namespace dci { namespace site
{
    class Manager;

    enum class TestStage
    {
        null,
        noenv,  //ничего нет
        min,    //mm, logger, async
        mload,  //после загрузки модулей
        mstart  //после старта модулей
    };

    APIDCI_SITE TestStage testStage();
    APIDCI_SITE Manager *testManager();

}}
