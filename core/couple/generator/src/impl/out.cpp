#include "out.hpp"
#include <iostream>
#include <cassert>

namespace dci { namespace couple { namespace generator
{
    Out &el(Out &out)
    {
        out._isNewLine = true;
        return out << std::endl;
    }

    Out &indent(Out &out)
    {
        ++out._indentLevel;
        return out;
    }

    Out &undent(Out &out)
    {
        assert(out._indentLevel);
        --out._indentLevel;
        return out;
    }

    Out::Out()
        : _fname()
        , _s()
        , _levels()
        , _currentStream(&_s)
        , _indentLevel(0)
        , _indentPad("    ")
    {
    }

    Out::~Out()
    {
    }

    void Out::open(const std::string &fname)
    {
        close();
        _fname = fname;

        _s.open(_fname);
        if(!_s)
        {
            throw std::system_error(std::error_code(errno, std::system_category()), "unable to open file "+_fname);
        }
    }

    void Out::close()
    {
        _s.close();
        _s.clear();
    }

    void Out::push()
    {
        _levels.emplace(std::ios_base::out);
        _currentStream = &_levels.top();
    }

    void Out::pop(bool commit)
    {
        assert(!_levels.empty());

        auto content = _levels.top().str();
        _levels.pop();

        if(_levels.empty())
        {
            _currentStream = &_s;
        }
        else
        {
            _currentStream = &_levels.top();
        }

        if(commit)
        {
            _currentStream->write(content.data(), content.size());
        }
    }

    Out::operator bool()
    {
        return static_cast<bool>(_s);
    }

    Out &Out::operator<<(Out &(*v)(Out &))
    {
        return v(*this);
    }

    Out &Out::operator<<(std::ostream&(*v)(std::ostream&))
    {
        (*_currentStream) << v;
        if(!_s)
        {
            throw std::system_error(std::error_code(errno, std::system_category()), "unable to write file "+_fname);
        }
        return *this;
    }

    void Out::flushNewLine()
    {
        if(_isNewLine)
        {
            for(std::size_t i(0); i<_indentLevel; ++i)
            {
                (*_currentStream) << _indentPad;
                if(!_s)
                {
                    throw std::system_error(std::error_code(errno, std::system_category()), "unable to write file "+_fname);
                }
            }
            _isNewLine = false;
        }
    }

}}}
