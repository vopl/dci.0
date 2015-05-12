#include "grammar.hpp"
#include "grammarError.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{

    Grammar::Grammar(ParseState &parseState)
        : Grammar::base_type(file)
        , _parseState(parseState)
    {

        mkPrimitive();
        mkList();
        mkSet();
        mkMap();
        mkPtr();
        mkArray();
        mkName();
        mkScopedName();
        mkTypeUse();
        mkAlias();
        mkBases();
        mkVariantField();
        mkVariant();
        mkStructField();
        mkStruct();
        mkEnumField();
        mkEnum();
        mkErrcField();
        mkErrc();
        mkMethodParam();
        mkMethod();
        mkIface();
        mkScope();
        mkInclude();
        mkDecl();
        mkDecls();
        mkFile();

        ////////////////////////////////////////////////////////////////////////////////
        error = boost::spirit::repository::qi::iter_pos[ phx::throw_(phx::construct<GrammarError>(qi::_1, qi::_r1))];
    }

    std::string Grammar::extractStringFromQuoted(const Token::token_value_type &str)
    {
        std::string content(std::begin(str), std::end(str));

        assert(content.size() >= 2);
        assert('"' == content[0] && '"' == content[content.size()-1]);

        content.erase(content.begin());
        content.erase(content.end()-1);

        std::size_t pos{0};
        while(std::string::npos != (pos = content.find("\\\"", pos, 2)))
        {
            content.erase(content.begin() + pos);
        }

        return content;
    }

}}}}
