#pragma once

#include <dci/couple/runtime.hpp>

namespace impl { namespace endpoint
{
    using namespace dci::couple::runtime;


    using LinkId = std::uint32_t;
    constexpr LinkId _nullLinkId = 0;
    using MessageType = std::uint32_t;
    constexpr MessageType _nullMessageType = 0;

    struct Message
    {
        MessageType     _type {_nullMessageType};
        LinkId          _linkId {_nullLinkId};
        Bytes           _body;

        /*
            everywhere in header - little endian

            header 1-16 bytes:
                uint2   typeSize -> 1,2,3,4 bytes
                uint2   linkIdSize-> 0,1,2,4 bytes
                uint2   bodyLengthSize -> 0,1,2,4 bytes
                uint2   reserved

                byte[typeSize] type
                byte[linkIdSize] linkId
                byte[bodyLengthSize] bodyLength

            body:
                byte[bodyLength] body

        */
    };


    bool deserializeHeader(Bytes &data, Message &msg, std::uint32_t &bodyLength);
    Bytes serialize(Message &&msg);
}}
