#include <dci/site/moduleEntry.hpp>
#include <dci/logger/logger.hpp>

#include "net.hpp"

using namespace dci::couple::runtime;
using namespace dci::site;
using namespace net;

struct Info
    : ModuleInfo
{
    Info()
    {
        _provider = "dci";
        _id.fromHex("874ca4fbf31985170b2ce0efbb20a5a5");
        _serviceIds.push_back(Host::_iid);

        _revision = 1;
        _name = "net";
        _description = "simple network module";
        //_tags;

        //_requiredServiceIds;
        //_requiredModuleIds;
    }

} info;


struct Entry
    : ModuleEntry
{
    Entry()
    {
    }

    ~Entry()
    {
    }

    const ModuleInfo &getInfo() override
    {
        return info;
    }

    dci::async::Future<std::error_code> install(const ModulePlace &place) override
    {
        (void)place;
        return dci::async::Future<std::error_code>();
    }

    dci::async::Future<std::error_code> uninstall(const ModulePlace &place) override
    {
        (void)place;
        return dci::async::Future<std::error_code>();
    }

    dci::async::Future<std::error_code> load(const ModulePlace &place) override
    {
        (void)place;
        return dci::async::Future<std::error_code>();
    }

    dci::async::Future<std::error_code> unload(const ModulePlace &place) override
    {
        (void)place;
        return dci::async::Future<std::error_code>();
    }

    dci::async::Future<std::error_code> start(Manager &manager, const ModulePlace &place) override
    {
        (void)manager;
        (void)place;
        return dci::async::Future<std::error_code>();
    }

    dci::async::Future<std::error_code> stop(const ModulePlace &place) override
    {
        (void)place;
        return dci::async::Future<std::error_code>();
    }

    //TODO интерфейсам автоматическое время жизни по ссылкам из сетки
    template <class Service>
    struct NetHostServiceHelper
        : public HostOpposite
    {
    protected:
        NetHostServiceHelper()
        {
            //connect
            bool b;
            b = wire()->interfaces.connect(&Service::interfaces, static_cast<Service *>(this));
            assert(b);
        }

        ~NetHostServiceHelper()
        {
            //disconnect
            wire()->interfaces.disconnect();
        }

    private://in
        Future<list< Interface>> interfaces();

    private://out
//        idl::Future< > interfaceAdded(Interface v)
//        {
//            return _iface.interfaceAdded(v);
//        }



    };

    struct NetHostService
        : NetHostServiceHelper<NetHostService>
    {
        Future<list<Interface>> interfaces()
        {
            //do impl
            //assert(0);

            list<Interface> l;

            return Future<list<Interface>>(std::move(l));
        }

    };

    struct NetHostFactory
        : ServiceFactory
    {
        void createService(void *outFuture) override
        {
            dci::async::Future<std::error_code, Host> *res = static_cast<dci::async::Future<std::error_code, Host> *>(outFuture);
            *res = dci::async::Future<std::error_code, Host>(Host(* new NetHostService));
        }
    };

    ServiceFactory *allocServiceFactory(const Iid &iid) override
    {
        assert(iid == info._serviceIds[0]);
        return new NetHostFactory();
    }

    void freeServiceFactory(const Iid &iid, ServiceFactory *factory) override
    {
        assert(iid == info._serviceIds[0]);
        delete factory;
    }

} entry;

extern "C"
{
    ModuleEntry *dciModuleEntry = &entry;
}
