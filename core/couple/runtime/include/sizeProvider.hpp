#pragma once

#include <dci/himpl/sizeProvider.hpp>

namespace dci {namespace couple {namespace runtime {namespace impl {class Hub;}}}}

namespace dci { namespace himpl {
    template <> struct sizeProvider< ::dci::couple::runtime::impl::Hub > { static const std::size_t _value =  0;};
}}

