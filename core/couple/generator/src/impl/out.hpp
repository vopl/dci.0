#pragma once

#include <fstream>
#include <system_error>
#include <stack>
#include <sstream>

namespace dci { namespace couple { namespace generator
{
    class Out;
    Out &el(Out &);
    Out &indent(Out &);
    Out &undent(Out &);


    class Out
    {
    public:
        Out();
        ~Out();

        void open(const std::string &fname);
        void close();

        void push();
        void pop(bool commit);

        operator bool();

        Out &operator<<(Out &(*)(Out &));
        Out &operator<<(std::ostream&(*v)(std::ostream&));

        template <class T>
        Out &operator<<(T v);

    private:
        void flushNewLine();

    private:
        std::string                     _fname;
        std::ofstream                   _s;
        std::stack<std::stringstream>   _levels;

        std::ostream *_currentStream;

        std::size_t     _indentLevel;
        std::string     _indentPad;
        bool            _isNewLine;

        friend Out &el(Out &);
        friend Out &indent(Out &);
        friend Out &undent(Out &);
    };


    template <class T>
    Out &Out::operator<<(T v)
    {
        flushNewLine();

        (*_currentStream) << v;
        if(!_s)
        {
            throw std::system_error(std::error_code(errno, std::system_category()), "unable to write file "+_fname);
        }

        return *this;
    }

}}}
