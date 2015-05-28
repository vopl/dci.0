#include "tokenizer.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{
    Tokenizer::Tokenizer()
        : ob        {'('}
        , cb        {')'}
        , ocb       {'{'}
        , ccb       {'}'}
        , oab       {'<'}
        , cab       {'>'}
        , semi      {';'}
        , comma     {','}
        , dcolon    {"\"::\""}
        , colon     {':'}
        , eq        {'='}
        , arrow     {"\"->\""}

        , quotedString {"\\\"((\\\\\\\")|[^\\\"])*\\\""}
        , uint      {"\\d+"}
        , nint      {"-\\d+"}
        , hint      {"(0x[0-9a-fA-F]*)"}

        , kwbool    {"\"bool\""}
        , kwstring  {"\"string\""}
        , kwint8    {"\"int8\""}
        , kwint16   {"\"int16\""}
        , kwint32   {"\"int32\""}
        , kwint64   {"\"int64\""}
        , kwuint8   {"\"uint8\""}
        , kwuint16  {"\"uint16\""}
        , kwuint32  {"\"uint32\""}
        , kwuint64  {"\"uint64\""}
        , kwreal32  {"\"real32\""}
        , kwreal64  {"\"real64\""}
        , kwbytes   {"\"Bytes\""}

        , kwlist    {"\"list\""}
        , kwset     {"\"set\""}
        , kwmap     {"\"map\""}
        , kwptr     {"\"ptr\""}
        , kwarray   {"\"array\""}

        , kwscope   {"\"scope\""}
        , kwinclude {"\"include\""}
        , kwalias   {"\"alias\""}
        , kwstruct  {"\"struct\""}
        , kwvariant {"\"variant\""}
        , kwiface   {"\"iface\""}
        , kwenum    {"\"enum\""}
        , kwerrc    {"\"errc\""}
        , kwin      {"\"in\""}
        , kwout     {"\"out\""}
        , kwnowait  {"\"nowait\""}

        , id        {"([a-zA-Z_]|{UTF8_2_4})+(\\w|{UTF8_2_4})*"}
    {
        this->self.add_pattern(
                    "ANY",
                    "[\\x01-\\xff]");

        this->self.add_pattern(
                    "UTF8_2_4",
                    "([\\xc0-\\xdf]{ANY}{1})|([\\xe0-\\xef]{ANY}{2})|([\\xf0-\\xf7]{ANY}{3})");

        this->self
            = TokenDef{"\\s"}                       [boost::spirit::lex::_pass = boost::spirit::lex::pass_flags::pass_ignore]
            | TokenDef{"\\/\\/[^\\r\\n]*\\r?\\n?"}  [boost::spirit::lex::_pass = boost::spirit::lex::pass_flags::pass_ignore]
            | TokenDef{"\\/\\*(.*?)\\*\\/"}         [boost::spirit::lex::_pass = boost::spirit::lex::pass_flags::pass_ignore]

            | ob
            | cb
            | ocb
            | ccb
            | oab
            | cab
            | semi
            | comma
            | dcolon
            | colon
            | eq
            | arrow

            | quotedString

            | uint
            | nint
            | hint

            | kwbool
            | kwstring
            | kwint8
            | kwint16
            | kwint32
            | kwint64
            | kwuint8
            | kwuint16
            | kwuint32
            | kwuint64
            | kwreal32
            | kwreal64
            | kwbytes

            | kwlist
            | kwset
            | kwmap
            | kwptr
            | kwarray

            | kwscope
            | kwinclude
            | kwalias
            | kwstruct
            | kwvariant
            | kwiface
            | kwenum
            | kwerrc
            | kwin
            | kwout
            | kwnowait

            | id

        ;
    }

    Tokenizer toks;

}}}}
