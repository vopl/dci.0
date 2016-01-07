#pragma once

#include <dci/couple/runtime.hpp>
#include <dci/couple/serialize/stream.hpp>
#include "linkId.hpp"
#include "messageType.hpp"
#include <vector>
#include <tuple>

namespace spares { namespace protocol
{
    using namespace dci::couple::runtime;

    //level 0 - bytes

    //level 1 - packets
    struct Unknown
    {
        MessageType _type;
        Bytes       _data;
    };

    struct Unknown4Link
    {
        MessageType _type;
        LinkId      _linkId;
        Bytes       _data;
    };

    //level 2 - typed messages
    namespace details
    {
        template <class D, std::uint32_t type>
        struct BaseMessage
        {
            using Base = BaseMessage<D, type>;
            static constexpr MessageType _type = (type << 1) | 0;

            template <class F>
            void enumerateFieldsMove(F f)
            {
                static_cast<D *>(this)->enumerateFields([this, &f](auto &x) {
                    f(std::move(x));
                });
            }
            template <class F>
            void enumerateFields(F)
            {
            }
        };
        template <class D, std::uint32_t type> constexpr MessageType BaseMessage<D, type>::_type;

        template <class D, std::uint32_t type>
        struct BaseMessage4Link : BaseMessage<D, 0>
        {
            using Base = BaseMessage4Link<D, type>;
            static constexpr MessageType _type = (type << 1) | 1;

            LinkId      _linkId;

            template <class F>
            void enumerateFields(F f)
            {
                f(_linkId);
            }
        };
        template <class D, std::uint32_t type> constexpr MessageType BaseMessage4Link<D, type>::_type;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    struct Null : details::BaseMessage<Null, 0>
    {
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    struct Hello : details::BaseMessage<Hello, 1>
    {
        std::uint32_t               _entropy;
        std::uint32_t               _version;
        std::vector<std::string>    _options;

        template <class F>
        void enumerateFields(F f)
        {
            Base::enumerateFields(f);
            f(_entropy);
            f(_version);
            f(_options);
        }

    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    struct Bye : details::BaseMessage<Bye, 2>
    {
    };


//    inject
//    invite
//    eject

//    request
//    response
//    message
//    close

//    commit
//    restore


}}


namespace dci { namespace couple { namespace serialize
{

#define _9815604562894710285679345(S)                       \
    template <> struct ValueTraits< S> : S                  \
    {                                                       \
        static const ValueKind _kind = ValueKind::struct_;  \
        template <class F>                                  \
        static void enumerateFields(S &&v, F f)             \
        {                                                   \
            v.enumerateFieldsMove(f);                       \
        }                                                   \
        template <class F>                                  \
        static void enumerateFields(S &v, F f)              \
        {                                                   \
            v.enumerateFields(f);                           \
        }                                                   \
    };                                                      \
                                                         /**/

    _9815604562894710285679345(::spares::protocol::Null)
    _9815604562894710285679345(::spares::protocol::Hello)
    _9815604562894710285679345(::spares::protocol::Bye)

#undef _9815604562894710285679345
}}}
