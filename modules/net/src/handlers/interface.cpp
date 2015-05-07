#include "interface.hpp"

namespace handlers
{
    Future< uint32> InterfaceHandler::flags()
    {
        assert(0);
    }

    Future< uint32> InterfaceHandler::mtu()
    {
        assert(0);
    }

    Future< string> InterfaceHandler::name()
    {
        assert(0);
    }

    Future< list< ip4::Net>> InterfaceHandler::ip4Nets()
    {
        assert(0);
    }

    Future< list< ip6::Net>> InterfaceHandler::ip6Nets()
    {
        assert(0);
    }

}
