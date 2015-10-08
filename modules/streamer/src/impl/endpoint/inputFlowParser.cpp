#include "inputFlowParser.hpp"

namespace impl { namespace endpoint
{
    InputFlowParser::InputFlowParser()
    {
    }

    InputFlowParser::~InputFlowParser()
    {
    }

    void InputFlowParser::reset()
    {
        _accumuler.clear();

        _headerReaded = false;
        _currentMessage._body.clear();
        _currentBodyLength = 0;
        _lastError = std::error_code();
    }

    const std::error_code &InputFlowParser::lastError() const
    {
        return _lastError;
    }

}}
