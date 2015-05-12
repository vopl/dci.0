#include "../grammar.hpp"
#include "../parse.hpp"
#include "../grammarError.hpp"
#include "../currentPosRaii.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{
    std::vector<Decl> Grammar::doInclude(const Token::token_value_type &str)
    {
        std::string fileName = extractStringFromQuoted(str);


        Scope res;
        {
            CurentPosRaii fnr(std::begin(str), _parseState);
            res = impl::parse(fileName, _parseState);
        }

        if(!res)
        {
            throw GrammarError(std::begin(str), "unable to include file");
        }

        return res->decls;
    }

    void Grammar::mkInclude()
    {
        ////////////////////////////////////////////////////////////////////////////////
        include =
            toks.kwinclude[qi::_val = phx::construct<std::vector<Decl>>()] >
            (
                toks.quotedString[qi::_val = phx::bind(&Grammar::doInclude, this, qi::_1)] |
                error(+"file name expected")
            ) >
            (toks.semi | error(+"';' expected"));
    }

}}}}
