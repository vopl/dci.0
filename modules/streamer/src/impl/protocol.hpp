#pragma once

#include <dci/couple/runtime.hpp>

namespace impl { namespace protocol
{
    /*
     *
     * заголовок - тип сообщения и идентификатор линка
     *      байт 0
     *          extendType: 1   // 0 - [тип занимает 1 байт], 1 - [тип занимает 2 байта]
     *          linkArea:2      // количество байт для кодирования линка, 0,1,2,4
     *          type0:5         // тип сообщения
     *      байт 1 (при extendType=1)
     *          type1:8         // расширенный тип сообщения
     *
     *      далее 0-4 байт (в зависимости от linkArea) идет идентификатора линка
     *          бит 0 - признак стороны - хозяина адресного пространства идентификатора линка
     *          дальнейшие биты - собственно, идентификатор
     *
     *
     * тело - нарезанный на чанки бинарный поток. Чанк:
     *      1-4 байта - длина чанка и признак последнего чанка
     *
     *      старший бит в каждом 0,1,2 байте - признак наличия еще одного байта
     *      младший бит в 0 байте - признак не последнего чанка (1 - не последний, 0 - последний)
     *
     *      размер как целое получается отбрасыванием всех признаковых бит и слепливанием в одно. Битность размера - до 28бит (256Мб) (6,7,7,8)
     *
     *      некоторые типы сообщений обязаны быть расположенными в едином чанке (напр helo, recovery, links_create, ...)
     *
     *
     *
     *
     *
     *
     *
     *
     */


    enum class Type : std::uint16_t
    {
        null            = 0,

        //endpoint
        hello,
        end,

        links_create,
        links_destroy,
        links_generation,
        links_commitGeneration,

        //serviceHub, interaction
        link_request,
        link_response,
        link_message,

        //serviceHub, control
        link_inject,
        link_invite,
        link_eject,

        //amortisseur
        recovery,

    };








    /* hello
     *      byte order
     *      version
     *      options [cookie]
     */

    /* recovery
     *      cookie
     *      sequence
     */

    /* createLinks
     *      linkId array
     */

    /* destroyLinks
     *      linkId array
     */

    /* linkRequest
     *      linkId
     *      callId
     *      serialized data
     */

    /* linkResponse
     *      linkId
     *      callId
     *      serialized data
     */

    /* linkMessage
     *      linkId
     *      serialized data
     */

}}
