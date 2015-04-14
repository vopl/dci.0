#pragma once

#include <dci/himpl/sizeProvider.hpp>

#include "predecl.hpp"

namespace dci { namespace himpl {
    template <> struct sizeProvider< ::dci::couple::meta::impl::Alias > { static const std::size_t _value=32; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Array > { static const std::size_t _value=24; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Attribute > { static const std::size_t _value=16; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Compound< ::dci::couple::meta::Attribute> > { static const std::size_t _value=24; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Compound< ::dci::couple::meta::EnumValue> > { static const std::size_t _value=24; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Compound< ::dci::couple::meta::Method> > { static const std::size_t _value=24; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Compound< ::dci::couple::meta::Struct> > { static const std::size_t _value=24; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Compound< ::dci::couple::meta::Variant> > { static const std::size_t _value=24; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Compound< ::dci::couple::meta::Enum> > { static const std::size_t _value=24; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Compound< ::dci::couple::meta::Alias> > { static const std::size_t _value=24; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Compound< ::dci::couple::meta::Iface> > { static const std::size_t _value=24; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Compound< ::dci::couple::meta::Scope> > { static const std::size_t _value=24; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Container1 > { static const std::size_t _value=16; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Container2 > { static const std::size_t _value=24; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Enum > { static const std::size_t _value=72; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::EnumValue > { static const std::size_t _value=8; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Iface > { static const std::size_t _value=216; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Inheritable< ::dci::couple::meta::Enum> > { static const std::size_t _value=24; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Inheritable< ::dci::couple::meta::Iface> > { static const std::size_t _value=24; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Inheritable< ::dci::couple::meta::Struct> > { static const std::size_t _value=24; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Inheritable< ::dci::couple::meta::Variant> > { static const std::size_t _value=24; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::List > { static const std::size_t _value=16; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Map > { static const std::size_t _value=24; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Method > { static const std::size_t _value=48; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Ptr > { static const std::size_t _value=16; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Primitive > { static const std::size_t _value=8; static const std::size_t _align=4; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Scope > { static const std::size_t _value=160; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Set > { static const std::size_t _value=16; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Struct > { static const std::size_t _value=216; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Named > { static const std::size_t _value=8; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Scoped > { static const std::size_t _value=8; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Type > { static const std::size_t _value=4; static const std::size_t _align=4; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Variant > { static const std::size_t _value=216; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::Library > { static const std::size_t _value=792; static const std::size_t _align=8; };
    template <> struct sizeProvider< ::dci::couple::meta::impl::LibraryBuilder > { static const std::size_t _value=792; static const std::size_t _align=8; };
}}

