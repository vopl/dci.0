#include "../grammar.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkDecls()
    {
        ////////////////////////////////////////////////////////////////////////////////
        decls =
            +(
                decl[phx::push_back(qi::_val, qi::_1)] |
                include[append(qi::_val, qi::_1)]
            );
    }

}}}}
