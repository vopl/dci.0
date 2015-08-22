#pragma once

#include <dci/couple/runtime/error.hpp>
#include <dci/couple/runtime/bytes.hpp>
#include "../error.hpp"
#include "../endianness.hpp"
#include "../valueTraits.hpp"

#include <system_error>
#include <type_traits>

namespace dci { namespace couple { namespace serialize { namespace details
{
    template <class Settings, class Context, class SinkSource> class Processor
    {
    public:
        Processor(Context &ctx, SinkSource &ss)
            : _ctx(ctx)
            , _ss(ss)
            , _processedSize(0)
        {
        }

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
        //integral
        template <class Value> std::enable_if_t<std::is_integral<Value>::value || std::is_floating_point<Value>::value, void> save(const Value &value)
        {
            meterProcessedSize(sizeof(Value));
            _ss.writePod(fixEndianness(value));
        }

        template <class Value> std::enable_if_t<std::is_integral<Value>::value || std::is_floating_point<Value>::value, void> load(Value &value)
        {
            Value tmp;
            meterProcessedSize(sizeof(Value));
            _ss.readPod(tmp);
            value = fixEndianness(tmp);
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        //string
        void save(const std::string &value)
        {
            save(static_cast<std::uint32_t>(value.size()));
            meterProcessedSize(value.size());
            _ss.writeBinary(value.data(), value.size());
        }

        void load(std::string &value)
        {
            std::uint32_t size;
            load(size);
            meterProcessedSize(size);
            value.resize(size);
            _ss.readBinary(&value.front(), size);
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        //Bytes
        void save(runtime::Bytes &&value)
        {
            save(static_cast<std::uint32_t>(value.size()));
            meterProcessedSize(value.size());
            _ss.writeBytes(std::forward<runtime::Bytes>(value));
        }

        void save(const runtime::Bytes &value)
        {
            save(value.clone());
        }

        void load(runtime::Bytes &value)
        {
            std::uint32_t size;
            load(size);
            meterProcessedSize(size);
            value = _ss.readBytes(size);
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        //pair
        template <class First, class Second> void save(std::pair<First, Second> &&value)
        {
            save(std::forward<First>(value.first));
            save(std::forward<Second>(value.second));
        }

        template <class First, class Second> void save(const std::pair<First, Second> &value)
        {
            save(value.first);
            save(value.second);
        }

        template <class First, class Second> void load(std::pair<First, Second> &value)
        {
            load(value.first);
            load(value.second);
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        //vector
        template <class Element> void save(const std::vector<Element> &value)
        {
            save(static_cast<std::uint32_t>(value.size()));
            saveArray(&value.front(), value.size());
        }

        template <class Element> void save(std::vector<Element> &&value)
        {
            save(static_cast<std::uint32_t>(value.size()));
            saveArray(&value.front(), value.size());
        }

        template <class Element> void load(std::vector<Element> &value)
        {
            std::uint32_t size;
            load(size);
            //это не точная проверка, лишь бы не был заявлен очень большой размер
            checkProcessedSize(_processedSize + size * sizeof(Element));
            value.resize(size);
            loadArray(&value.front(), value.size());
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        //array
        template <class Element, std::size_t N> void save(const std::array<Element, N> &value)
        {
            saveArray(&value.front(), N);
        }

        template <class Element, std::size_t N> void save(std::array<Element, N> &&value)
        {
            saveArray(&value.front(), N);
        }

        template <class Element, std::size_t N> void load(std::array<Element, N> &value)
        {
            loadArray(&value.front(), N);
        }


        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        //map
        template <class Key, class Value, class... OtherParams> void save(const std::map<Key, Value, OtherParams...> &value)
        {
            saveSequence(value.begin(), value.size());
        }

        template <class Key, class Value, class... OtherParams> void save(std::map<Key, Value, OtherParams...> &&value)
        {
            saveSequence(value.begin(), value.size());
        }

        template <class Key, class Value, class... OtherParams> void load(std::map<Key, Value, OtherParams...> &value)
        {
            loadSequence<std::pair<Key, Value>>([&](std::pair<Key, Value> &&element){
                value.insert(std::forward<std::pair<Key, Value>>(element));
            });
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        //set
        template <class Element, class... OtherParams> void save(const std::set<Element, OtherParams...> &value)
        {
            saveSequence(value.begin(), value.size());
        }

        template <class Element, class... OtherParams> void save(std::set<Element, OtherParams...> &&value)
        {
            saveSequence(value.begin(), value.size());
        }

        template <class Element, class... OtherParams> void load(std::set<Element, OtherParams...> &value)
        {
            loadSequence<Element>([&](Element &&element){
                value.insert(std::forward<Element>(element));
            });
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        //ptr
        //TODO

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        //variant
        template <class Value> std::enable_if_t<ValueKind::variant == ValueTraits<Value>::_kind> save(const Value &value)
        {
            save(static_cast<std::uint32_t>(value.typeId()));
            value.visit([](const auto &subval){
                save(subval);
            });
        }

        template <class Value> std::enable_if_t<ValueKind::variant == ValueTraits<Value>::_kind> save(Value &&value)
        {
            save(static_cast<std::uint32_t>(value.typeId()));
            value.visit([](auto &&subval){
                save(std::forward<std::remove_reference_t<decltype(subval)>>(subval));
            });
        }

        template <class Value> std::enable_if_t<ValueKind::variant == ValueTraits<Value>::_kind> load(Value &value) throw (std::system_error)
        {
            std::uint32_t typeId;
            load(typeId);
            if(!ValueTraits<Value>::isTypeIdValid(typeId))
            {
                throw std::system_error(err_general::bad_input);
            }
            value.ensureType(typeId);
            value.visit([](auto &subval){
                load(subval);
            });
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        //struct
        template <class Value> std::enable_if_t<ValueKind::struct_ == ValueTraits<Value>::_kind> save(const Value &value)
        {
            ValueTraits<Value>::enumerateFields([](const auto &subval){
                save(subval);
            });
        }

        template <class Value> std::enable_if_t<ValueKind::struct_ == ValueTraits<Value>::_kind> save(Value &&value)
        {
            ValueTraits<Value>::enumerateFields([](auto &&subval){
                save(std::forward<std::remove_reference_t<decltype(subval)>>(subval));
            });
        }

        template <class Value> std::enable_if_t<ValueKind::struct_ == ValueTraits<Value>::_kind> load(Value &value)
        {
            ValueTraits<Value>::enumerateFields([](auto &subval){
                load(subval);
            });
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        //enum
        template <class Value> std::enable_if_t<ValueKind::enum_ == ValueTraits<Value>::_kind> save(const Value &value)
        {
            save(static_cast<std::underlying_type_t<Value>>(value));
        }

        template <class Value> std::enable_if_t<ValueKind::enum_ == ValueTraits<Value>::_kind> load(Value &value)
        {
            std::underlying_type_t<Value> tmp;
            load(tmp);
            value = static_cast<Value>(tmp);
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        //service
        template <class Value> std::enable_if_t<ValueKind::service == ValueTraits<Value>::_kind> save(Value &&value)
        {
            std::uint32_t serviceId = _ctx.setService(value);
            save(serviceId);
        }

        template <class Value> std::enable_if_t<ValueKind::service == ValueTraits<Value>::_kind> load(Value &value)
        {
            std::uint32_t serviceId;
            load(serviceId);
            value = _ctx.getService(serviceId);
        }





    private:
        //array, vector
        //is_trivially_copyable - writeBinary
        //otherwise - save by elements

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        //TODO: relax is_trivial to is_trivially_copyable
        template <class Element> std::enable_if_t<std::is_trivial<Element>::value, void> saveArray(Element *begin, std::uint32_t amount)
        {
            if(amount)
            {
                meterProcessedSize(sizeof(Element)*amount);
                _ss.writeBinary(begin, sizeof(Element)*amount);
            }
        }

        template <class Element> std::enable_if_t<!std::is_trivial<Element>::value, void> saveArray(Element *begin, std::uint32_t amount)
        {
            for(std::size_t i(0); i<amount; i++)
            {
                save(std::forward<Element>(begin[i]));
            }
        }

        //TODO: relax is_trivial to is_trivially_copyable
        template <class Element> std::enable_if_t<std::is_trivial<Element>::value, void> loadArray(Element *begin, std::uint32_t amount)
        {
            if(amount)
            {
                meterProcessedSize(sizeof(Element)*amount);
                _ss.readBinary(begin, sizeof(Element)*amount);
            }
        }

        template <class Element> std::enable_if_t<!std::is_trivial<Element>::value, void> loadArray(Element *begin, std::uint32_t amount)
        {
            for(std::size_t i(0); i<amount; i++)
            {
                load(begin[i]);
            }
        }

        template <class Iterator> void saveSequence(Iterator begin, std::uint32_t amount)
        {
            save(amount);
            for(std::uint32_t i(0); i<amount; ++begin, ++i)
            {
                save(std::forward<std::remove_reference_t<decltype(*begin)>>(*begin));
            }

        }

        template <class Element, class Inserter> void loadSequence(Inserter inserter)
        {
            std::uint32_t size;
            load(size);

            for(std::size_t i(0); i<size; ++i)
            {
                Element element;
                load(element);
                inserter(std::move(element));
            }
        }

    private:
        void checkProcessedSize(std::size_t size) const throw (std::system_error)
        {
            if(Settings::_maxSize > _processedSize + size)
            {
                return;
            }

            throw std::system_error(err_general::quote_exhausted);
        }

        void meterProcessedSize(std::size_t size) throw (std::system_error)
        {
            _processedSize += size;
            if(Settings::_maxSize > _processedSize)
            {
                return;
            }

            throw std::system_error(err_general::quote_exhausted);
        }

        template <class Value>
        static Value fixEndianness(const Value &src)
        {
            if(Settings::_endianness == Endianness::unknown || _currentEndianness == Endianness::unknown)
            {
                assert(!"unknown endianness");
                abort();
                return src;
            }

            if(Settings::_endianness == _currentEndianness)
            {
                return src;
            }

            static const Endianness a = Settings::_endianness < _currentEndianness ? Settings::_endianness : _currentEndianness;
            static const Endianness b = Settings::_endianness < _currentEndianness ? _currentEndianness : Settings::_endianness;

            if(Endianness::little == a)
            {
                assert(Endianness::little != b);

                if(Endianness::big == b)
                {
                    //little - big

                    if(2 == sizeof(Value))
                    {
                        return   ((src & 0x00FF) << 8)
                               | ((src & 0xFF00) >> 8);
                    }
                    else if(4 == sizeof(Value))
                    {
                        return   ((src & 0x000000FF) << 24)
                               | ((src & 0x0000FF00) <<  8)
                               | ((src & 0x00FF0000) >>  8)
                               | ((src & 0xFF000000) >> 24);
                    }
                    else if(8 == sizeof(Value))
                    {
                        return   ((src & 0xFF00000000000000ull) >> 56)
                               | ((src & 0x00FF000000000000ull) >> 40)
                               | ((src & 0x0000FF0000000000ull) >> 24)
                               | ((src & 0x000000FF00000000ull) >>  8)
                               | ((src & 0x00000000FF000000ull) <<  8)
                               | ((src & 0x0000000000FF0000ull) << 24)
                               | ((src & 0x000000000000FF00ull) << 40)
                               | ((src & 0x00000000000000FFull) << 56);
                    }
                    else
                    {
                        assert(!"massive value");
                    }
                }
                else
                {
                    assert(Endianness::pdp == b);
                    //little - pdp

                    if(2 == sizeof(Value))
                    {
                        return   src;
                    }
                    else if(4 == sizeof(Value))
                    {
                        return   ((src & 0x0000FFFF) << 16)
                               | ((src & 0xFFFF0000) >> 16);
                    }
                    else if(8 == sizeof(Value))
                    {
                        return   ((src & 0xFFFF000000000000ull) >> 48)
                               | ((src & 0x0000FFFF00000000ull) >> 16)
                               | ((src & 0x00000000FFFF0000ull) << 16)
                               | ((src & 0x000000000000FFFFull) << 48);
                    }
                    else
                    {
                        assert(!"massive value");
                    }
                }
            }
            else if(Endianness::big == a)
            {
                assert(Endianness::pdp == b);
                //big - pdp

                if(2 == sizeof(Value))
                {
                    return   ((src & 0x00FF) << 8)
                           | ((src & 0xFF00) >> 8);
                }
                else if(4 == sizeof(Value))
                {
                    return   ((src & 0x000000FF) <<  8)
                           | ((src & 0x0000FF00) >>  8)
                           | ((src & 0x00FF0000) <<  8)
                           | ((src & 0xFF000000) >>  8);
                }
                else if(8 == sizeof(Value))
                {
                    return   ((src & 0xFF00000000000000ull) >>  8)
                           | ((src & 0x00FF000000000000ull) <<  8)
                           | ((src & 0x0000FF0000000000ull) >>  8)
                           | ((src & 0x000000FF00000000ull) <<  8)
                           | ((src & 0x00000000FF000000ull) >>  8)
                           | ((src & 0x0000000000FF0000ull) <<  8)
                           | ((src & 0x000000000000FF00ull) >>  8)
                           | ((src & 0x00000000000000FFull) <<  8);
                }
                else
                {
                    assert(!"massive value");
                }
            }

            return src;
        }

    private:
        Context &       _ctx;
        SinkSource &    _ss;
        std::size_t     _processedSize;
    };

}}}}
