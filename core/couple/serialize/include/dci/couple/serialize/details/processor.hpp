#pragma once

#include <dci/couple/runtime/error.hpp>

#include <system_error>
#include <type_traits>

namespace dci { namespace couple { namespace serialize { namespace details
{
    template <class Settings, class Context, class SinkSource> struct Processor
    {
        static void checkTotalSize(std::size_t totalSize)
        {
            if(Settings::_maxSize > totalSize)
            {
                throw std::system_error(err_general::quote_exhausted);
            }
        }

        template <class Value>
        static Value fixEndianness(const Value &src)
        {
            //TODO: implement this
            assert(!"not implemented");
            return src;
        }

//        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
//        template <class Value> static std::size_t save(Context &, SinkSource &, const Value &, std::size_t)
//        {
//            assert(!"never here");
//            abort();
//            return 0;
//        }
//        template <class Value> static std::size_t load(Context &, SinkSource &, Value &, std::size_t)
//        {
//            assert(!"never here");
//            abort();
//            return 0;
//        }

        /*
         * bool
         * integer
         * real
         *
         * string
         * Bytes
         *
         * array
         *
         * vector
         * map
         * set
         * ptr
         *
         * variant
         *
         * struct (TypeTraits)
         * enum (TypeTraits)
         *
         * service (interface instance)
         * iid
         *
         */

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        template <class Value>
        static std::enable_if_t<std::is_integral<Value>::value || std::is_floating_point<Value>::value, std::size_t> save(Context &, SinkSource &sink, const Value &value, std::size_t totalSize)
        {
            totalSize += sink.writePod(fixEndianness(value));
            checkTotalSize(totalSize);

            return totalSize;
        }

        template <class Value>
        static std::enable_if_t<std::is_integral<Value>::value || std::is_floating_point<Value>::value, std::size_t> load(Context &, SinkSource &source, Value &value, std::size_t totalSize)
        {
            Value tmp;

            totalSize += source.readPod(tmp);
            checkTotalSize(totalSize);

            value = fixEndianness(tmp);

            return totalSize;
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        static std::size_t save(Context &context, SinkSource &sink, const std::string &value, std::size_t totalSize)
        {
            totalSize = save(context, sink, static_cast<std::uint32_t>(value.size()), totalSize);
            checkTotalSize(totalSize);

            totalSize += sink.writeBinary(value.data(), value.size());
            checkTotalSize(totalSize);

            return totalSize;
        }

        static std::size_t load(Context &, SinkSource &source, std::string &value, std::size_t totalSize)
        {
//            Value tmp;
//            totalSize = checkTotalSize(source.readPod(tmp), totalSize);

//            value = fixEndianness(tmp);

//            return totalSize;
        }

};
}}}}
