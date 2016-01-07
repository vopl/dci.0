#pragma once

#include <dci/couple/runtime.hpp>
#include "protocol/typedMessage.hpp"

namespace spares { namespace protocol
{






    struct Hello : TypedMessage<Hello, 1, false>
    {
        std::uint32_t               _version;
        std::uint32_t               _entropy;
        std::vector<std::string>    _options;

        auto fields() { return std::tie(_version, _entropy, _options); }
    };


//    enum class Type : std::uint32_t
//    {
//        null            = 0,

//        //endpoint
//        hello,
//        end,

//        links_create,
//        links_destroy,
//        links_generation,
//        links_commitGeneration,

//        //serviceHub, interaction
//        link_request,
//        link_response,
//        link_message,

//        //serviceHub, control
//        link_inject,
//        link_invite,
//        link_eject,

//        //amortisseur
//        recovery,

//    };


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
