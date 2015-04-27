#include "tokenizer.hpp"
#include "parseState.hpp"

namespace  dci { namespace couple { namespace parser { namespace impl
{
    ////////////////////////////////////////////////////////////////////////////////
    struct CurentPosRaii
    {
        CharIterator _old;
        CharIterator _cur;
        ParseState &_parseState;

        CurentPosRaii(CharIterator cur, ParseState &parseState)
            : _old(parseState._currentPos)
            , _cur(cur)
            , _parseState(parseState)
        {
            _parseState._currentPos = _cur;
            _parseState._currentFiles.insert(_cur.file());
        }

        ~CurentPosRaii()
        {
            _parseState._currentFiles.erase(_cur.file());
            _parseState._currentPos = _old;
        }
    };

}}}}
