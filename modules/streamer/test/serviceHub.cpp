#include <dci/gtest.hpp>
#include <dci/site.hpp>
#include <dci/site/manager.hpp>
#include <dci/site/test.hpp>

#include <dci/async.hpp>
#include <dci/couple/runtime.hpp>
#include <dci/couple/serialize.hpp>
#include "streamer.hpp"
#include "test.hpp"
#include "streamerHandlerSkel.hpp"
#include "streamerSerializer.hpp"


/////////////////////////////////////////////////////////////////////

using namespace dci;
using namespace dci::site;
using namespace dci::async;
using namespace dci::couple::runtime;

//using namespace streamer;
using namespace dci::couple;

namespace  dci { namespace  couple { namespace  serialize
{
            template <>
            struct ValueTraits< ::streamer::Channel>
            {
                static const ValueKind _kind = ValueKind::interface;
            };

}}}







/////////////////////////////////////////////////////////////////////
#include <dci/couple/serialize.hpp>
namespace xyz
{
    using namespace dci::couple::runtime;

    struct SinkSource
    {
        //sink
        template <class T>
        void writePod(const T &v) throw (std::system_error)
        {
            assert(0);
            std::cout<<(int)v<<std::endl;
        }

        void writeBinary(const char *data, std::size_t size) throw (std::system_error)
        {
            assert(0);
            (void)data;
            (void)size;
        }

        void writeBytes(Bytes &&data) throw (std::system_error)
        {
            assert(0);
            (void)data;
        }

        //source
        template <class T>
        void readPod(T &v)  throw (std::system_error)
        {
            assert(0);
            v = 220;
        }

        void readBinary(char *data, std::size_t size) throw (std::system_error)
        {
            assert(0);
            (void)data;
            (void)size;
        }

        Bytes readBytes(std::size_t size) throw (std::system_error)
        {
            assert(0);
            (void)size;

            return Bytes();
        }
    };

    struct Settings
    {
        static const std::size_t _maxSize = 1024*1024*1;

        static const serialize::Endianness _endianness = serialize::Endianness::little;

    };

    struct Engine
    {
        using OStream = serialize::Stream<Settings, Engine, SinkSource>;
        using IStream  = serialize::Stream<Settings, Engine, SinkSource>;

        template <class Service>
        std::uint32_t setService(Service &&);

        void pushWriter(
                dci::couple::serialize::Interface<Engine> *interface,
                dci::couple::serialize::WriterPtr<OStream> &&writerPtr,
                int cookie)
        {
            assert(0);
            (void)interface;
            (void)writerPtr;
            (void)cookie;
        }
    };

    struct Context
    {
      std::uint32_t setService(...);
    } context;

    int f()
    {

        {
            std::set<int> kin;
            kin.insert(0);

            SinkSource sink;

            serialize::Stream<Settings, Context, SinkSource> ostream(context, sink);

            ostream << 10.2;
            ostream << std::move(kin);
        }

        {
            std::set<int> kout;
            SinkSource source;

            serialize::Stream<Settings, Context, SinkSource> istream(context, source);

            istream >> kout;
        }


        return 0;
    }
}






/////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
class ServiceHub
    : public testing::Test
{
protected:
    virtual void SetUp() override
    {
        _manager = testManager();
        EXPECT_NE(_manager, nullptr);
    }

    virtual void TearDown() override
    {
        _manager = nullptr;
    }

    Manager *_manager {nullptr};
};



TEST_F(ServiceHub, Create)
{
    {
        ::streamer::ServiceHub sh = _manager->createService< ::streamer::ServiceHub>();
    }
}

TEST_F(ServiceHub, CreateAttach)
{
    {
        ::streamer::ServiceHub sh1 = _manager->createService< ::streamer::ServiceHub>();
        ::streamer::ServiceHub sh2 = _manager->createService< ::streamer::ServiceHub>();

        ::streamer::Channel ch1;
        ::streamer::Channel ch2 {ch1._output, ch1._input};

        auto f1 = sh1.attachChannel(std::move(ch1));
        auto f2 = sh2.attachChannel(std::move(ch2));

        ASSERT_TRUE(f1.hasValue());
        ASSERT_TRUE(f2.hasValue());
    }
}

TEST_F(ServiceHub, CreateAttachDetach)
{
    {
        ::streamer::ServiceHub sh1 = _manager->createService< ::streamer::ServiceHub>();
        ::streamer::ServiceHub sh2 = _manager->createService< ::streamer::ServiceHub>();


        {
            auto f1 = sh1.detachChannel();
            auto f2 = sh2.detachChannel();

            ASSERT_TRUE(f1.hasError());
            ASSERT_TRUE(f2.hasError());
        }


        {
            ::streamer::Channel ch1;
            ::streamer::Channel ch2 {ch1._output, ch1._input};

            auto f1 = sh1.attachChannel(std::move(ch1));
            auto f2 = sh2.attachChannel(std::move(ch2));

            ASSERT_TRUE(f1.hasValue());
            ASSERT_TRUE(f2.hasValue());
        }

        {
            auto f1 = sh1.detachChannel();
            auto f2 = sh2.detachChannel();

            ASSERT_TRUE(f1.hasValue());
            ASSERT_TRUE(f2.hasValue());
        }

        {
            auto f1 = sh1.detachChannel();
            auto f2 = sh2.detachChannel();

            ASSERT_TRUE(f1.hasError());
            ASSERT_TRUE(f2.hasError());
        }
    }
}

TEST_F(ServiceHub, SimpleExchange)
{
    if(0)
    {
        ::streamer::ServiceHub sh1 = _manager->createService< ::streamer::ServiceHub>();
        ::streamer::ServiceHub sh2 = _manager->createService< ::streamer::ServiceHub>();

        ::streamer::Channel ch1;
        ::streamer::Channel ch2 {ch1._output, ch1._input};

        auto f1 = sh1.attachChannel(std::move(ch1));
        auto f2 = sh2.attachChannel(std::move(ch2));

        ASSERT_TRUE(f1.hasValue());
        ASSERT_TRUE(f2.hasValue());

        streamer::X1 x1;
        streamer::X1Opposite x1Opposite(x1);

        bool handled = false;
        sh2.signal_serviceInjected().connect([&handled](Interface &&interface, std::uint32_t &&serviceId)->void{
            (void)interface;
            (void)serviceId;

            handled = true;

        });
        ASSERT_TRUE(sh1.injectService(std::move(x1)).hasValue());
        dci::async::yield();
        ASSERT_TRUE(handled);
    }
}
