#pragma once

#include <dci/himpl/layoutProvider.hpp>

#include "dci/couple/meta/predecl.hpp"

namespace dci { namespace himpl {
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Alias > { static const std::size_t _size=56; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Array > { static const std::size_t _size=48; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Attribute > { static const std::size_t _size=40; static const std::size_t _align=8; static const bool _polymorphic=false; };
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
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Enum > { static const std::size_t _size=72; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::EnumValue > { static const std::size_t _size=40; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Errc > { static const std::size_t _size=72; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::ErrcValue > { static const std::size_t _size=40; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Interface > { static const std::size_t _size=280; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Inheritable< ::dci::couple::meta::Interface> > { static const std::size_t _size=24; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Inheritable< ::dci::couple::meta::Struct> > { static const std::size_t _size=24; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::List > { static const std::size_t _size=40; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Map > { static const std::size_t _size=48; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Method > { static const std::size_t _size=88; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Ptr > { static const std::size_t _size=40; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Primitive > { static const std::size_t _size=28; static const std::size_t _align=4; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Scope > { static const std::size_t _size=208; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Set > { static const std::size_t _size=40; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Struct > { static const std::size_t _size=264; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Named > { static const std::size_t _size=8; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Scoped > { static const std::size_t _size=32; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Signed > { static const std::size_t _size=20; static const std::size_t _align=4; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Type > { static const std::size_t _size=4; static const std::size_t _align=4; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Variant > { static const std::size_t _size=240; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::Library > { static const std::size_t _size=912; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::meta::impl::LibraryBuilder > { static const std::size_t _size=888; static const std::size_t _align=8; static const bool _polymorphic=false; };
}}

