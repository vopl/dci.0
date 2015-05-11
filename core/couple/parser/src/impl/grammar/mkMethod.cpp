#include "../grammar.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkMethod()
    {
        ////////////////////////////////////////////////////////////////////////////////
        method =
            (
                toks.kwin [qi::_a = MethodDirection::in ] |
                toks.kwout[qi::_a = MethodDirection::out]
            )
                [qi::_val = phx::construct<Method>(phx::new_<SMethod>())]
                [phx::bind(&SMethod::direction, deref(qi::_val)) = qi::_a] >
            (
                name[phx::bind(&SMethod::name, deref(qi::_val)) = qi::_1] |
                error(+"method name expected")
            ) >
            (toks.colon | error(+"':' expected")) >
            (
                (toks.ob | error(+"'(' expected")) >
                (
                    -(methodParam[phx::push_back(phx::bind(&SMethod::query, deref(qi::_val)), qi::_1)] % toks.comma)
                ) >
                (toks.cb | error(+"')' expected"))
            ) >
            (
                (
                    toks.arrow >
                    (
                        typeUse[phx::push_back(phx::bind(&SMethod::reply, deref(qi::_val)), qi::_1)] |
                        (
                            (toks.ob | error(+"'(' expected")) >
                            (
                                -(typeUse[phx::push_back(phx::bind(&SMethod::reply, deref(qi::_val)), qi::_1)] % toks.comma)
                            ) >
                            (toks.cb | error(+"')' expected"))
                        )
                    )
                )
                    [phx::bind(&SMethod::noreply, deref(qi::_val)) = false] |
                qi::eps
                    [phx::bind(&SMethod::noreply, deref(qi::_val)) = true]
            ) >
            (toks.semi | error(+"';' expected"));
    }

}}}}
