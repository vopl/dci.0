#pragma once

#include "common.hpp"

namespace impl { namespace fsm { namespace endpoint
{
    namespace inEvents
    {
        namespace common
        {
            struct Attach {mutable Channel _channel;};
            struct Detach {};
        }

        namespace read
        {
            struct Flow {Bytes _data;};

            struct SubscribeNone {};
            struct SubscribeStepByStep {};
            struct SubscribeContinuous {};

            struct SubscriptionOk {};
            struct SubscriptionFail {};

            struct Reset {};
        }

        namespace write
        {
            struct SubscribeNone {};
            struct SubscribeStepByStep {};
            struct SubscribeContinuous {};

            struct Message {mutable Bytes _data;};
            struct MessageOk {};
            struct MessageFail {};

            struct Reset {};
        }
    }

    namespace outEvents
    {
        namespace common
        {
            struct Start {};
            struct Stop {};
        }

        namespace read
        {
            struct MessageHeader {};
            struct MessageBodyChunk {};
            struct Fail {};
        }

        namespace write
        {
            struct Ok {};
            struct Fail {};

            struct SubscribeNone {};
            struct SubscribeStepByStep {};
            struct SubscribeContinuous {};
        }
    }
}}}
