#pragma once

#include <dci/himpl/layoutProvider.hpp>

#include "dci/couple/meta/predecl.hpp"

namespace dci { namespace himpl {
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Alias > { static const std::size_t _size=48; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Array > { static const std::size_t _size=40; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Attribute > { static const std::size_t _size=32; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Compound< ::dci::couple::meta::Type> > { static const std::size_t _size=24; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Compound< ::dci::couple::meta::Attribute> > { static const std::size_t _size=24; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Compound< ::dci::couple::meta::EnumValue> > { static const std::size_t _size=24; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Compound< ::dci::couple::meta::ErrcValue> > { static const std::size_t _size=24; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Compound< ::dci::couple::meta::Method> > { static const std::size_t _size=24; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Compound< ::dci::couple::meta::Struct> > { static const std::size_t _size=24; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Compound< ::dci::couple::meta::Variant> > { static const std::size_t _size=24; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Compound< ::dci::couple::meta::Enum> > { static const std::size_t _size=24; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Compound< ::dci::couple::meta::Errc> > { static const std::size_t _size=24; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Compound< ::dci::couple::meta::Alias> > { static const std::size_t _size=24; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Compound< ::dci::couple::meta::Interface> > { static const std::size_t _size=24; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Compound< ::dci::couple::meta::Scope> > { static const std::size_t _size=24; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Container1 > { static const std::size_t _size=16; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Container2 > { static const std::size_t _size=24; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Enum > { static const std::size_t _size=64; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::EnumValue > { static const std::size_t _size=32; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Errc > { static const std::size_t _size=64; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::ErrcValue > { static const std::size_t _size=32; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Interface > { static const std::size_t _size=272; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Inheritable< ::dci::couple::meta::Interface> > { static const std::size_t _size=24; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Inheritable< ::dci::couple::meta::Struct> > { static const std::size_t _size=24; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::List > { static const std::size_t _size=32; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Map > { static const std::size_t _size=40; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Method > { static const std::size_t _size=80; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Ptr > { static const std::size_t _size=32; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Primitive > { static const std::size_t _size=24; static const std::size_t _align=4; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Scope > { static const std::size_t _size=200; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Set > { static const std::size_t _size=32; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Struct > { static const std::size_t _size=256; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Named > { static const std::size_t _size=8; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Scoped > { static const std::size_t _size=24; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Signed > { static const std::size_t _size=16; static const std::size_t _align=1; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Type > { static const std::size_t _size=4; static const std::size_t _align=4; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Variant > { static const std::size_t _size=232; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Library > { static const std::size_t _size=904; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::LibraryBuilder > { static const std::size_t _size=888; static const std::size_t _align=8; static const bool _polymorphic=false; };
}}

