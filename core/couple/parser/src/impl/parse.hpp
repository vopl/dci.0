#pragma once

#include <dci/couple/parser/config.hpp>
#include <dci/couple/parser/errorInfo.hpp>
#include "ast.hpp"
#include "parseState.hpp"


namespace  dci { namespace couple { namespace parser { namespace impl
{
    Scope parse(const std::string &fileName, ParseState &parseState);
    Scope parse(const std::vector<std::string> &fileNames, const Config &cfg, std::vector<ErrorInfo> &errs);


}}}}
