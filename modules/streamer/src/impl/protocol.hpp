#pragma once

#include <dci/couple/runtime.hpp>

namespace impl { namespace protocol
{
    enum class DataUnitType : std::uint8_t
    {
        null            = 0x00,

        hello           = 0x03,
        recovery        = 0x04,

        createLinks     = 0x07,
        destroyLinks    = 0x08,

        linkRequest     = 0x83,
        linkResponse    = 0x84,
        linkMessage     = 0x85,

        end             = 0x0b,
    };

    using DataUnitSequence = std::uint16_t;





    struct DataUnitHeader
    {
        DataUnitType        _type;
        DataUnitSequence    _sequence;
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
