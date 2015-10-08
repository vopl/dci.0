#pragma once

#include <dci/couple/runtime.hpp>
#include "message.hpp"
#include "streamer.hpp"

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
        std::error_code process(Bytes &&input, F &&resultReceiver);

        const std::error_code &lastError() const;

    private:
        Bytes           _accumuler;

        bool            _headerReaded {false};

        Message         _currentMessage;
        std::uint32_t   _currentBodyLength {0};

        std::error_code _lastError;

    };



    template <class F>
    std::error_code InputFlowParser::process(Bytes &&input, F &&resultReceiver)
    {
        if(_lastError)
        {
            return _lastError;
        }

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
                    return _lastError;
                }
            }
            else
            {
                std::error_code ec = deserializeHeader(_accumuler, _currentMessage, _currentBodyLength);

                if(ec)
                {
                    if(::streamer::error::lowData == ec)
                    {
                        return _lastError;
                    }

                    _lastError = ec;
                    return _lastError;
                }

                _headerReaded = true;
                continue;
            }
        }

        return _lastError;
    }

}}
