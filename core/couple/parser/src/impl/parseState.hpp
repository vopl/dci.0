#pragma once

#include <config.hpp>
#include <errorInfo.hpp>

#include "tokenizer.hpp"

#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <memory>

namespace  dci { namespace couple { namespace parser { namespace impl
{
    class Grammar;

    struct ParseState
    {
        const Config &          _cfg;
        std::vector<ErrorInfo> &_errors;

        Grammar                                 *_grammar;
        CharIterator                            _currentPos;
        std::set<std::string>                   _currentFiles;

        std::map<std::string, std::shared_ptr<std::ifstream>>    _processedFiles;
    };


}}}}
