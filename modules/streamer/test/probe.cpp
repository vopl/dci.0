#include <dci/gtest.hpp>
#include <dci/site.hpp>
#include <dci/async.hpp>
#include <dci/couple/runtime.hpp>
#include "streamer.hpp"


/////////////////////////////////////////////////////////////////////

using namespace streamer;
using namespace dci::couple;








/////////////////////////////////////////////////////////////////////
#include <dci/couple/serialize.hpp>
namespace xyz
{
    using namespace dci::couple::runtime;

    struct SinkSource
    {
        //sink
        template <class T>
        void writePod(const T &) throw (std::system_error)
        {
            //assert(0);
        }

        void writeBinary(const char *data, std::size_t size) throw (std::system_error)
        {
            //assert(0);
            (void)data;
            (void)size;
        }

        void writeBytes(Bytes &&data) throw (std::system_error)
        {
            //assert(0);
            (void)data;
        }

        //source
        template <class T>
        void readPod(T &)  throw (std::system_error)
        {
            //assert(0);
        }

        void readBinary(char *data, std::size_t size) throw (std::system_error)
        {
            //assert(0);
            (void)data;
            (void)size;
        }

        Bytes readBytes(std::size_t size) throw (std::system_error)
        {
            //assert(0);
            (void)size;

            return Bytes();
        }
    };

    struct Settings
    {
        static const std::size_t _maxSize = 1024*1024*1;

    };

    int f()
    {

        {
            std::array<Bytes, 0> kin;

            SinkSource sink;
            struct {} context;

            std::error_code ec = serialize::save<Settings>(context, std::move(kin), sink);//ok, writerError, badInput
            (void)ec;
        }

        {
            Bytes kout;
            SinkSource source;
            struct {} context;

            std::error_code ec = serialize::load<Settings>(context, source, kout);//ok, readerError, badInput
            (void)ec;
        }

        return 0;
    }
}










TEST(Streamer, Probe)
{
    xyz::f();
    return;

    using namespace dci::couple::runtime;

    {
        Future<Hub> hf = dci::site::testManager()->createService<Hub>();

        GTEST_ASSERT_EQ(hf.hasValue(), true);
    }


    Hub h1 = dci::site::testManager()->createService<Hub>().detachValue<0>();
    Hub h2 = dci::site::testManager()->createService<Hub>().detachValue<0>();

    Channel c1;
    Channel c2;

    c1.signal_flow().connect(&Channel::flow, &c2);
    c2.signal_flow().connect(&Channel::flow, &c1);

    h1.start(std::move(c1));
    h2.start(std::move(c2));


    dci::async::Event e1, e2;

    dci::async::spawn([&]()
    {
        Interface i = h1.get(Iid(Channel::_iid));

        e1.set();
    });

    dci::async::spawn([&]()
    {
        h2.signal_get().connect([](const Iid &iid) -> runtime::Future< Interface>
        {
            (void)iid;

            runtime::Promise< Interface> p;
            p.resolveError(std::make_error_code(std::errc::address_family_not_supported));

            return p.future();
        });

        e2.set();
    });


    dci::async::waitAll(e1, e2);
}
