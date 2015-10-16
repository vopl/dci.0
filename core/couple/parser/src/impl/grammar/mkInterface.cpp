#include "../grammar.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkInterface()
    {
        ////////////////////////////////////////////////////////////////////////////////
        interface =
            toks.kwinterface[qi::_val = phx::construct<Interface>(phx::new_<SInterface>())] >
            (
                name[phx::bind(&SInterface::name, deref(qi::_val)) = qi::_1] |
                error(+"interface name expected")
            ) >
            -baseInterfaces[phx::bind(&SInterface::bases, deref(qi::_val)) = qi::_1] >
            (toks.ocb /*| error(+"'{' expected")*/) >
            *(
                //decls[phx::insert(phx::bind(&SInterface::decls, deref(qi::_val)), phx::end(phx::bind(&SInterface::decls, deref(qi::_val))), phx::begin(qi::_1), phx::end(qi::_1))] |

                decls[
                    phx::for_each(
                        qi::_1,
                        phx::lambda
                            (phx::local_names::_a = phx::bind(&SInterface::decls, deref(qi::_val)))
                            [phx::push_back(phx::local_names::_a, phx::arg_names::_1)]
                    )
                ] |

                method[phx::push_back(phx::bind(&SInterface::methods, deref(qi::_val)), qi::_1)]
            ) >
            (toks.ccb | error(+"'}' expected"));
    }

}}}}
