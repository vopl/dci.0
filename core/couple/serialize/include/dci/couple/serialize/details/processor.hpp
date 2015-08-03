#pragma once

#include <dci/couple/runtime/error.hpp>
#include <dci/couple/runtime/bytes.hpp>

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
                return;
            }

            throw std::system_error(runtime::err_general::quote_exhausted);
        }

        template <class Value>
        static Value fixEndianness(const Value &src)
        {
            //TODO: implement this
            //assert(!"not implemented");
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
            totalSize += sizeof(Value);
            checkTotalSize(totalSize);

            sink.writePod(fixEndianness(value));

            return totalSize;
        }

        template <class Value>
        static std::enable_if_t<std::is_integral<Value>::value || std::is_floating_point<Value>::value, std::size_t> load(Context &, SinkSource &source, Value &value, std::size_t totalSize)
        {
            Value tmp;

            totalSize += sizeof(Value);
            checkTotalSize(totalSize);

            source.readPod(tmp);

            value = fixEndianness(tmp);

            return totalSize;
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        static std::size_t save(Context &context, SinkSource &sink, const std::string &value, std::size_t totalSize)
        {
            totalSize = save(context, sink, static_cast<std::uint32_t>(value.size()), totalSize);

            totalSize += value.size();
            checkTotalSize(totalSize);

            sink.writeBinary(value.data(), value.size());

            return totalSize;
        }

        static std::size_t load(Context &context, SinkSource &source, std::string &value, std::size_t totalSize)
        {
            std::uint32_t size;
            totalSize = load(context, source, size, totalSize);

            checkTotalSize(size);
            totalSize += size;
            checkTotalSize(totalSize);

            value.resize(size);
            source.readBinary(&value.front(), size);

            return totalSize;
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        static std::size_t save(Context &context, SinkSource &sink, runtime::Bytes &&value, std::size_t totalSize)
        {
            totalSize = save(context, sink, static_cast<std::uint32_t>(value.size()), totalSize);

            totalSize += value.size();
            checkTotalSize(totalSize);

            sink.writeBytes(std::forward<runtime::Bytes>(value));

            return totalSize;
        }

        static std::size_t save(Context &context, SinkSource &sink, const runtime::Bytes &value, std::size_t totalSize)
        {
            return save(context, sink, value.clone(), totalSize);
        }

        static std::size_t load(Context &context, SinkSource &source, runtime::Bytes &value, std::size_t totalSize)
        {
            std::uint32_t size;
            totalSize = load(context, source, size, totalSize);

            checkTotalSize(size);
            totalSize += size;
            checkTotalSize(totalSize);

            value = source.readBytes(size);

            return totalSize;
        }

        //array, vector
        //is_trivially_copyable - writeBinary
        //otherwise - save by elements

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        //TODO: relax is_trivial to is_trivially_copyable
        template <class Element>
        static std::enable_if_t<std::is_trivial<Element>::value, std::size_t> saveArray(Context &context, SinkSource &sink, Element &&first, std::uint32_t amount, std::size_t totalSize)
        {
            if(amount)
            {
                totalSize += sizeof(Element)*amount;
                checkTotalSize(totalSize);

                sink.writeBinary(&first, sizeof(Element)*amount);
            }

            return totalSize;
        }

        template <class Element>
        static std::enable_if_t<!std::is_trivial<Element>::value, std::size_t> saveArray(Context &context, SinkSource &sink, Element &&first, std::uint32_t amount, std::size_t totalSize)
        {
            for(std::size_t i(0); i<amount; i++)
            {
                totalSize = save(context, sink, std::forward<Element>(*(&first+i)), totalSize);
            }

            return totalSize;
        }

        template <class Element>
        static std::size_t save(Context &context, SinkSource &sink, const std::vector<Element> &value, std::size_t totalSize)
        {
            totalSize = save(context, sink, static_cast<std::uint32_t>(value.size()), totalSize);
            return saveArray(context, sink, value.front(), value.size(), totalSize);
        }

        template <class Element>
        static std::size_t save(Context &context, SinkSource &sink, std::vector<Element> &&value, std::size_t totalSize)
        {
            totalSize = save(context, sink, static_cast<std::uint32_t>(value.size()), totalSize);
            return saveArray(context, sink, std::move(value.front()), value.size(), totalSize);
        }

        template <class Element, std::size_t N>
        static std::size_t save(Context &context, SinkSource &sink, const std::array<Element, N> &value, std::size_t totalSize)
        {
            return saveArray(context, sink, value.front(), N, totalSize);
        }

        template <class Element, std::size_t N>
        static std::size_t save(Context &context, SinkSource &sink, std::array<Element, N> &&value, std::size_t totalSize)
        {
            return saveArray(context, sink, std::move(value.front()), N, totalSize);
        }

    };



}}}}
