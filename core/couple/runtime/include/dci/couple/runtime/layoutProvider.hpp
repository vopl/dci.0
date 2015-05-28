#pragma once

#include <dci/himpl/layoutProvider.hpp>

namespace dci {namespace couple {namespace runtime {namespace impl {class Sign;}}}}
namespace dci {namespace couple {namespace runtime {namespace impl {class SignBuilder;}}}}
namespace dci {namespace couple {namespace runtime {namespace impl {class Bytes;}}}}

namespace dci { namespace himpl {
    template <> struct LayoutProvider< ::dci::couple::runtime::impl::Sign > { static const std::size_t _size=16; static const std::size_t _align=1; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::runtime::impl::SignBuilder > { static const std::size_t _size=104; static const std::size_t _align=8; static const bool _polymorphic=false; };
    template <> struct LayoutProvider< ::dci::couple::runtime::impl::Bytes > { static const std::size_t _size=24; static const std::size_t _align=8; static const bool _polymorphic=false; };
}}

