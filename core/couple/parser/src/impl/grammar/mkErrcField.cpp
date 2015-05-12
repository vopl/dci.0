#include "../grammar.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkErrcField()
    {
        ////////////////////////////////////////////////////////////////////////////////
        errcField =
            name
                [qi::_val = phx::construct<ErrcField>(phx::new_<SErrcField>())]
                [phx::bind(&SErrcField::name, deref(qi::_val)) = qi::_1] >
            (
                (
                    toks.colon >
                    (
                        toks.quotedString[phx::bind(&SErrcField::description, deref(qi::_val)) = phx::bind(&Grammar::extractStringFromQuoted, qi::_1)] |
                        error(+"quoted string expected")
                    )
                ) |
                qi::eps[phx::bind(&SErrcField::description, deref(qi::_val)) = phx::bind(&SName::value, deref(phx::bind(&SErrcField::name, deref(qi::_val))))]
            ) >
            (toks.semi | error(+"';' expected"));
    }

}}}}
