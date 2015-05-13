#include "interface.hpp"

#include "../impl/interface.hpp"

namespace handlers
{
    Interface::Interface(::impl::Interface *impl)
        : _impl(impl)
    {
        if(_impl)
        {
            _impl->registerHandler(this);
        }
    }

    Interface::~Interface()
    {
        if(_impl)
        {
            _impl->unregisterHandler(this);
        }
    }

    void Interface::dropImpl()
    {
        if(_impl)
        {
            _impl = nullptr;
        }
    }

    Future< uint32> Interface::flags()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        return _impl->flags();
    }

    Future< uint32> Interface::mtu()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        return _impl->mtu();
    }

    Future< string> Interface::name()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        return string(_impl->name());
    }

    Future< list< ip4::Net>> Interface::ip4Nets()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        return list< ip4::Net>(_impl->ip4Nets());
    }

    Future< list< ip6::Net>> Interface::ip6Nets()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        return list< ip6::Net>(_impl->ip6Nets());
    }

}
