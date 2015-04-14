#include "out.hpp"
#include <iostream>

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
        --out._indentLevel;
        return out;
    }

    Out::Out()
        : _fname()
        , _s()
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
        _s << v;
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
                _s << _indentPad;
                if(!_s)
                {
                    throw std::system_error(std::error_code(errno, std::system_category()), "unable to write file "+_fname);
                }
            }
            _isNewLine = false;
        }
    }

}}}
