#include "hub.hpp"
#include <dci/couple/runtime.hpp>

namespace handlers
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Hub::Hub()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Hub::~Hub()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< Hub::SubchannelId> Hub::attach(Channel &&unnamed_0)
    {
        assert(0);
        (void)unnamed_0;
        return Future< Hub::SubchannelId>(std::make_error_code(std::errc::address_family_not_supported));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< > Hub::detach(Hub::SubchannelId &&unnamed_0)
    {
        assert(0);
        (void)unnamed_0;
        return Future< >(std::make_error_code(std::errc::address_family_not_supported));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< > Hub::start(Channel &&unnamed_0)
    {
        assert(0);
        (void)unnamed_0;
        return Future< >(std::make_error_code(std::errc::address_family_not_supported));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Future< > Hub::stop()
    {
        assert(0);
        return Future< >(std::make_error_code(std::errc::address_family_not_supported));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void_ Hub::flow(Channel::Traffic &&unnamed_0)
    {
        assert(0);
        (void)unnamed_0;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void HubHandlerFactory::createService(void *outFuture)
    {
        auto *res = static_cast<dci::async::Future<std::error_code, ::streamer::Hub> *>(outFuture);
        *res = ::streamer::Hub(* new Hub);
    }

}
