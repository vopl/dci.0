#include "../grammar.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkEnumField()
    {
        ////////////////////////////////////////////////////////////////////////////////
        enumField =
            name
                [qi::_val = phx::construct<EnumField>(phx::new_<SEnumField>())]
                [phx::bind(&SEnumField::name, deref(qi::_val)) = qi::_1] >
            (
                (
                    toks.eq >
                    (
                        (toks.uint | toks.nint | toks.hint)
                            [phx::bind(&SEnumField::value, deref(qi::_val)) = phx::construct<std::string>(phx::begin(qi::_1), phx::end(qi::_1))] |
                        error(+"enum value expected")
                    )
                ) |
                qi::eps
            ) >
            (toks.semi | error(+"';' expected"));
    }

}}}}
