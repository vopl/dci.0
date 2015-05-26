#pragma once

namespace dci { namespace site
{
    class Manager;
}}


namespace dci { namespace site { namespace testHub
{
    enum class Stage
    {
        null,
        noenv,  //ничего нет
        min,    //mm, logger, async
        mload,  //после загрузки модулей
        mstart  //после старта модулей
    };

    Stage stage();
    Manager *manager();

}}}
