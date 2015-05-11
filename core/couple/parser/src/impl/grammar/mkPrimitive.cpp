#include "../grammar.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkPrimitive()
    {
        ////////////////////////////////////////////////////////////////////////////////
        primitive =
            (
                toks.kwbool     [qi::_a = meta::PrimitiveKind::bool_    ] |
                toks.kwstring   [qi::_a = meta::PrimitiveKind::string   ] |
                toks.kwint8     [qi::_a = meta::PrimitiveKind::int8     ] |
                toks.kwint16    [qi::_a = meta::PrimitiveKind::int16    ] |
                toks.kwint32    [qi::_a = meta::PrimitiveKind::int32    ] |
                toks.kwint64    [qi::_a = meta::PrimitiveKind::int64    ] |
                toks.kwuint8    [qi::_a = meta::PrimitiveKind::uint8    ] |
                toks.kwuint16   [qi::_a = meta::PrimitiveKind::uint16   ] |
                toks.kwuint32   [qi::_a = meta::PrimitiveKind::uint32   ] |
                toks.kwuint64   [qi::_a = meta::PrimitiveKind::uint64   ] |
                toks.kwreal32   [qi::_a = meta::PrimitiveKind::real32   ] |
                toks.kwreal64   [qi::_a = meta::PrimitiveKind::real64   ]
            ) >
            qi::eps
                [qi::_val = phx::construct<Primitive>(phx::new_<SPrimitive>())]
                [phx::bind(&SPrimitive::kind, deref(qi::_val)) = qi::_a];

    }

}}}}
