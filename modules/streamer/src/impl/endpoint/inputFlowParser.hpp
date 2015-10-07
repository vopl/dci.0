#pragma once

#include <dci/couple/runtime.hpp>
#include "message.hpp"

namespace impl { namespace endpoint
{
    using namespace dci::couple::runtime;


    class InputFlowParser
    {
    public:
        InputFlowParser();
        ~InputFlowParser();

        void reset();

        template <class F>
        void process(Bytes &&input, F &&resultReceiver);

    private:
        Bytes           _accumuler;

        bool            _headerReaded {false};

        Message         _currentMessage;
        std::uint32_t   _currentBodyLength {0};

    };



    template <class F>
    void InputFlowParser::process(Bytes &&input, F &&resultReceiver)
    {
        _accumuler.append(std::move(input));

        while(!_accumuler.empty())
        {
            if(_headerReaded)
            {
                assert(_currentBodyLength > _currentMessage._body.size());

                std::size_t need = _currentMessage._body.size() - _currentBodyLength;

                if(need >= _accumuler.size())
                {
                    _currentMessage._body.append(_accumuler.detachFirst(need));
                    resultReceiver(std::move(_currentMessage));
                    _headerReaded = false;
                    _currentMessage._body.clear();
                    _currentBodyLength = 0;
                    continue;
                }
                else
                {
                    _currentMessage._body.append(std::move(_accumuler));
                    return;
                }
            }
            else
            {
                if(deserializeHeader(_accumuler, _currentMessage, _currentBodyLength))
                {
                    _headerReaded = true;
                    continue;
                }
                else
                {
                    return;
                }
            }
        }
    }

}}
