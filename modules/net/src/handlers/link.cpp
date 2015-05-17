#include "link.hpp"

#include "../impl/link.hpp"

namespace handlers
{
    Link::Link(::impl::Link *impl)
        : _impl(impl)
    {
        if(_impl)
        {
            _impl->registerHandler(this);
        }
    }

    Link::~Link()
    {
        if(_impl)
        {
            _impl->unregisterHandler(this);
        }
    }

    void Link::dropImpl()
    {
        if(_impl)
        {
            removed();
            _impl = nullptr;
        }
    }

    Future< uint32> Link::id()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        return _impl->id();
    }

    Future< string> Link::name()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        return string(_impl->name());
    }

    Future< uint32> Link::flags()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        return _impl->flags();
    }

    Future< uint32> Link::mtu()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        return _impl->mtu();
    }

    Future< list< ip4::LinkAddress>> Link::ip4()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        return list< ip4::LinkAddress>(_impl->ip4());
    }

    Future< list< ip6::LinkAddress>> Link::ip6()
    {
        if(!_impl)
        {
            return std::error_code(::net::error::general::implementation_down);
        }

        return list< ip6::LinkAddress>(_impl->ip6());
    }

}
