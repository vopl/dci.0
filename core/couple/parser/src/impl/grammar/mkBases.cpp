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
        baseInterfaces =
            toks.colon[qi::_val = phx::construct<BaseInterfaces>(phx::new_<SBaseInterfaces>())] >
            (
                (
                    scopedName[phx::push_back(phx::bind(&SBaseInterfaces::scopedNames, deref(qi::_val)), qi::_1)] |
                    error(+"base interface type expected")
                ) % toks.comma
            );
    }

}}}}
