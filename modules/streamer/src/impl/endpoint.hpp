#pragma once

#include <dci/couple/runtime.hpp>
#include "streamer.hpp"

namespace impl
{
    using namespace dci::couple::runtime;
    using namespace streamer;

    //////////////////////////////////////////////////////////////////
    //events

    //general
    struct attach {};
    struct detach {};

    struct start {};
    struct stop {};
    struct reset {};


    struct null {};
    struct unknown {};

    //channel
    struct hello {};
    struct bye {};

    //links
    struct links_create {};
    struct links_destroy {};
    struct links_generation {};
    struct links_commitGeneration {};


    //link
    struct link_request {};
    struct link_response {};
    struct link_message {};

    //serviceHub
    struct link_inject {};
    struct link_invite {};
    struct link_eject {};

    //amortisseur
    struct recovery {};






    template <class Derived>
    class Endpoint
    {
    public:
        Endpoint();
        ~Endpoint();

    public:
        Future< > attachChannel(Channel &&arg_0);
        Future< Channel> detachChannel();

    private:
        void flow(Bytes &&in);

        //наружу события
        //  канал присоединен
        //  канал запущен
        //  ошибка в канале
        //  входящее сообщение
        //  канал отсоединен

        //снаружи события
        //  исходящее сообщение



        /*
         * машины
         *      общее состояние канала и его опции
         *          контейнер своих линков, контейнер чужих линков
         *
         *
         */

    private:
        enum class State
        {
            null,           //пустое состояние
            initialization, //договорится о версии, порядке байт, опциях
            working,        //штатная работа
            fail,           //ошибка в канале
        };

    private:
        State   _state;
        Channel _channel;

    private:

        //state

        //settings (version, endianness, options,...)

        //linksLocal2Remote
        //linksRemote2Local

    };
}
