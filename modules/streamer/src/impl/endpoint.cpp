#include "endpoint.hpp"

namespace impl
{
    template <class Derived>
    Future< > Endpoint<Derived>::attachChannel(Channel &&arg_0)
    {
        assert(0);
    }

    template <class Derived>
    Future< Channel> Endpoint<Derived>::detachChannel()
    {
        assert(0);
    }
}

#include "../handlers/channelHub.hpp"
#include "../handlers/serviceHub.hpp"
#include "../handlers/amortisseur.hpp"

template class impl::Endpoint<handlers::ChannelHub>;
template class impl::Endpoint<handlers::ServiceHub>;
template class impl::Endpoint<handlers::Amortisseur>;


/*
    по каналу ходят пакеты, служебные и полезные


    пакет
        тип
        серийник (растущий номер в потоке, циклический по 16 битам например)

    служебный пакет : пакет
        размер данных
        блок данных фиксированной структуры в зависимости от типа и указанного выше размера


    полезный пакет : пакет
        номер субканала
        данные верхнего уровня, побиты на чанки, каждый из которых маркирован длиной чанка и признаком конца














    стадии
        установление бинарных форматов (порядок байт, ...)
        установление версии протокола и доступных опций
        восстановление утерянного состояния
        начало
        обмен
        завершение
*/
