#pragma once

#include "tokenizer.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{
    class GrammarError
        : public std::runtime_error
    {

    public:
        GrammarError(const GrammarError &) = default;
        GrammarError(TokIterator pos, const std::string &msg);
        GrammarError(CharIterator pos, const std::string &msg);
        ~GrammarError();

        GrammarError &operator=(const GrammarError &) = default;

        const CharIterator &pos() const;

    private:
        CharIterator _pos;
    };

}}}}
