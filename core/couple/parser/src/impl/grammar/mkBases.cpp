#include "../grammar.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkBases()
    {
        ////////////////////////////////////////////////////////////////////////////////
        baseStructs =
            toks.colon[qi::_val = phx::construct<BaseStructs>(phx::new_<SBaseStructs>())] >
            (
                (
                    scopedName[phx::push_back(phx::bind(&SBaseStructs::scopedNames, deref(qi::_val)), qi::_1)] |
                    error(+"base struct name expected")
                ) % toks.comma
            );

        ////////////////////////////////////////////////////////////////////////////////
        baseIfaces =
            toks.colon[qi::_val = phx::construct<BaseIfaces>(phx::new_<SBaseIfaces>())] >
            (
                (
                    scopedName[phx::push_back(phx::bind(&SBaseIfaces::scopedNames, deref(qi::_val)), qi::_1)] |
                    error(+"base iface type expected")
                ) % toks.comma
            );
    }

}}}}
