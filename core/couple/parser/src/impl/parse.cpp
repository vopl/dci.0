#include "ast/oppositeInterfacesMaker.hpp"
#include "ast/scopeMerger.hpp"
#include "ast/namesChecker.hpp"
#include "ast/ownerIndexer.hpp"
#include "ast/scopedNamesResolver.hpp"
#include "ast/basesChecker.hpp"

#include "parse.hpp"
#include "grammar.hpp"
#include "grammarError.hpp"
#include "currentPosRaii.hpp"

#include <boost/filesystem.hpp>

#include <fstream>

namespace  dci { namespace couple { namespace parser { namespace impl
{
    namespace
    {
        ////////////////////////////////////////////////////////////////////////////////
        std::string resolveFileName(const std::string &in, const ParseState &parseState, std::string &errorMessage)
        {
            try
            {

                boost::filesystem::path p(in);
                boost::filesystem::path candidate;
                if(p.is_absolute())
                {
                    candidate = p;
                    if(!boost::filesystem::exists(candidate))
                    {
                        errorMessage = "not found";
                        return std::string();
                    }
                }
                else
                {
                    boost::filesystem::path base;

                    if(!parseState._currentPos)
                    {
                        base = boost::filesystem::current_path();
                    }
                    else
                    {
                        base = boost::filesystem::absolute(parseState._currentPos.file()).remove_filename();
                    }

                    if(boost::filesystem::exists(base / p))
                    {
                        candidate = base / p;
                    }
                    else
                    {
                        for(const std::string &b : parseState._cfg._includeDirectories)
                        {
                            if(boost::filesystem::exists(b / p))
                            {
                                candidate = b / p;
                                break;
                            }
                        }
                    }
                }

                if(!boost::filesystem::exists(candidate))
                {
                    errorMessage = "not found";
                    return std::string();
                }

                if(boost::filesystem::is_symlink(candidate))
                {
                    candidate = boost::filesystem::read_symlink(candidate);
                }

                if(boost::filesystem::is_regular_file(candidate))
                {
                    candidate.normalize();
                    return candidate.generic_string();
                }

                errorMessage = "is not a regular file";
                return std::string();
            }
            catch(const boost::filesystem::filesystem_error &err)
            {
                errorMessage = err.code().message();
                return std::string();
            }

            assert(0&&"never here");
//            errorMessage = "not found";
            return std::string();
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    Scope parse(const std::string &fileNameUnresolved, ParseState &parseState)
    {
        std::string resolverErrorMessage;
        std::string fileName = resolveFileName(fileNameUnresolved, parseState, resolverErrorMessage);
        if(fileName.empty())
        {
            parseState._errors.emplace_back(ErrorInfo {
                                                parseState._currentPos.file(),
                                                parseState._currentPos.line(),
                                                parseState._currentPos.column(),
                                                fileNameUnresolved + ": " + resolverErrorMessage});
            return Scope();
        }

        if(parseState._cfg._printSourceFiles)
        {
            std::cout << "source file: "<<fileName<<std::endl;
        }

        //check cyclic
        if(parseState._currentFiles.end() != parseState._currentFiles.find(fileName))
        {
            parseState._errors.emplace_back(ErrorInfo {
                                                parseState._currentPos.file(),
                                                parseState._currentPos.line(),
                                                parseState._currentPos.column(),
                                                fileNameUnresolved + ": cyclic inclusion"});
            return Scope();
        }

        //load content
        std::shared_ptr<std::ifstream> reader{new std::ifstream{fileName}};
        if(!*reader)
        {
            parseState._errors.emplace_back(ErrorInfo {fileName, -1, -1, strerror(errno)});
            return Scope();
        }


        CharIterator lexBegin{boost::spirit::multi_pass<std::istreambuf_iterator<char>>{std::istreambuf_iterator<char>{*reader}}, fileName};
        CharIterator lexEnd{boost::spirit::multi_pass<std::istreambuf_iterator<char>>{std::istreambuf_iterator<char>{}}, fileName};

        auto pushError = [&](const CharIterator &pos, const std::string &msg)
        {
            parseState._errors.emplace_back(ErrorInfo {
                                  pos.file(),
                                  static_cast<int>(pos.line()),
                                  static_cast<int>(pos.column()),
                                  msg});
        };

        parseState._processedFiles.emplace(std::make_pair(fileName, reader));
        CurentPosRaii fnr(lexBegin, parseState);

        //tokenize
        TokIterator tokBegin;
        TokIterator tokEnd;

        {
            CharIterator lexIter{lexBegin};
            tokBegin = toks.begin(lexIter, lexEnd);
            tokEnd = toks.end();

            TokIterator tokIter = tokBegin;
            for(; tokIter != tokEnd; ++tokIter)
            {
                if(!token_is_valid(*tokIter))
                {
                    pushError(lexIter, "invalid input sequence");
                    return Scope();
                }
            }
        }

        //parse
        TokIterator tokIter{tokBegin};

        Scope res;

        bool r;
        try
        {
            r = qi::parse(
                tokIter,
                tokEnd,
                *parseState._grammar,
                res);
        }
        catch(const GrammarError &e)
        {
            pushError(e.pos(), std::string("error: ") + e.what());
            return Scope();
        }

        if(!r)
        {
            pushError(tokIter->value().begin(), std::string("fail"));
            return Scope();
        }

        if(tokIter != tokEnd)
        {
            pushError(tokIter->value().begin(), std::string("unexpected extra input"));
            return Scope();
        }

        return res;
    }

    ////////////////////////////////////////////////////////////////////////////////
    Scope parse(const std::vector<std::string> &fileNames, const Config &cfg, std::vector<ErrorInfo> &errs)
    {
        Scope res;
        {
            ParseState parseState = {cfg, errs, nullptr, {}, {}, {}};
            Grammar g(parseState);
            parseState._grammar = &g;

            for(const std::string &fileName : fileNames)
            {
                Scope localRes = parse(fileName, parseState);
                if(!localRes)
                {
                    return Scope();
                }

                if(!res)
                {
                    res = localRes;
                }
                else
                {
                    res->decls.insert(res->decls.end(), localRes->decls.begin(), localRes->decls.end());
                }
            }
        }

        //make opposite interfaces
        ast::OppositeInterfacesMaker().exec(res);

        //merge scopes
        ast::ScopeMerger().exec(res);

        //check names uniqueness
        if(!ast::NamesChecker(errs).exec(res))
        {
            return Scope();
        }

        //index owners
        ast::OwnerIndexer(errs).exec(res);
        if(!errs.empty())
        {
            return Scope();
        }

        //resolve typeUse.scopedName
        if(!ast::ScopedNamesResolver(errs).exec(res))
        {
            return Scope();
        }

        //check bases for cycles
        if(!ast::BasesChecker(errs).exec(res))
        {
            return Scope();
        }

        return res;
    }

}}}}
