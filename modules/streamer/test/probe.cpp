#include <dci/gtest.hpp>
#include <dci/site.hpp>
#include <dci/async.hpp>
#include <dci/couple/runtime.hpp>
#include <dci/couple/serialize.hpp>
#include "streamer.hpp"


/////////////////////////////////////////////////////////////////////

using namespace streamer;
using namespace dci::couple;

namespace  dci { namespace  couple { namespace  serialize
{
            template <>
            struct ValueTraits< ::streamer::Channel>
            {
                static const ValueKind _kind = ValueKind::service;
            };

}}}


namespace tryIfaceSerialization
{
    using namespace dci::couple::serialize;

    //////////////////////////////////////////////////////////////////////
    // serializer for Hub
    template <class Engine>
    struct Hub
        : public ::streamer::HubOpposite
        , public ::dci::couple::serialize::InterfaceImplHelper<Hub<Engine>, Engine>
    {
        using ImplHelper = ::dci::couple::serialize::InterfaceImplHelper<Hub<Engine>, Engine>;
        using OStream = typename Engine::OStream;
        using IStream = typename Engine::IStream;

    public:
        //constructor
        Hub(Engine &engine, bool deleteSelfWhenUninvolved=true)
            : ImplHelper(engine)
        {
            (void)deleteSelfWhenUninvolved;

//            if(deleteSelfWhenUninvolved) wire()->listenUninvolve(true, [](void *userData){delete static_cast<Handler*>(userData);}, static_cast<Handler*>(this));

//            //connect 'in' methods
//            bool b; (void)b;
//            b = ::streamer::HubOpposite::signal_start().connect(&Handler::start, static_cast<Handler *>(this));
//            assert(b);
//            b = ::streamer::HubOpposite::signal_stop().connect(&Handler::stop, static_cast<Handler *>(this));
//            assert(b);
//            b = ::streamer::HubOpposite::signal_put().connect(&Handler::put, static_cast<Handler *>(this));
//            assert(b);
//            b = ::streamer::HubOpposite::signal_get().connect(&Handler::get, static_cast<Handler *>(this));
//            assert(b);
//            b = ::streamer::HubOpposite::signal_join().connect(&Handler::join, static_cast<Handler *>(this));
//            assert(b);
        }

        //destructor
        ~Hub()
        {
//            //disconnect 'in' methods
//            ::streamer::HubOpposite::signal_start().disconnect();
//            ::streamer::HubOpposite::signal_stop().disconnect();
//            ::streamer::HubOpposite::signal_put().disconnect();
//            ::streamer::HubOpposite::signal_get().disconnect();
//            ::streamer::HubOpposite::signal_join().disconnect();

        }

        //concrete
        void readRequest(IStream &)
        {
            assert(0);
            //read 'with result' methodId
//            switch(methodId)
//            {
//            case MethodId::get:
//                //read args

//                //fire signal
                  //res = ::streamer::HubOpposite::get(arg1)

//                //struct ConcreteResponseWriter ...
//                //return new ConcreteResponseWriter;

//                break;
//            }
        }

        void readMessage(IStream &)
        {
            assert(0);
            //read void methodId
//            switch(methodId)
//            {
//            case MethodId::get:
//                //read args
//                //fire signal
//                break;
//            }
        }

        //concrete
        ::dci::couple::runtime::Future< > start(::streamer::Channel &&unnamed_0)
        {
            /*
             *
             * generate callId, store callId -> promise
             * create pack (lambda)
             *  callId
             *  methodId
             *  args
             *
             * emit pack
             *
             * return future from promise
             */

            struct ConcreteRequestWriterResponseReader
                : public RequestWriterResponseReader<OStream, IStream>
            {
                std::tuple< ::streamer::Channel> _args;
                ::dci::couple::runtime::Promise< > _promise;

                ConcreteRequestWriterResponseReader(::streamer::Channel &&unnamed_0)
                    : _args(std::move(unnamed_0))
                {

                }

                ::dci::couple::runtime::Future< > result()
                {
                    return _promise.future();
                }

                virtual void write(OStream &w) override
                {
                    RequestWriterResponseReader<OStream, IStream>::write(w);

                    assert(0);
                    w << MethodId::start;
                    w << std::move(_args);
                }

                virtual void read(IStream &r) override
                {
                    RequestWriterResponseReader<OStream, IStream>::read(r);


                    assert(0);
                    //read type (error or success)
                    //empty because of no values in result
                    _promise.resolveValue();
                }
            };

            return this->pushRequestWriter(std::unique_ptr<ConcreteRequestWriterResponseReader>(new ConcreteRequestWriterResponseReader(std::move(unnamed_0))));
        }

        ::dci::couple::runtime::Future< > stop() = delete;
        //::dci::couple::runtime::void_ channelError();
        ::dci::couple::runtime::void_ put(::dci::couple::runtime::Interface &&unnamed_0) = delete;
        //::dci::couple::runtime::void_ put(::dci::couple::runtime::Interface &&unnamed_0);
        ::dci::couple::runtime::Future< ::dci::couple::runtime::Interface> get(::dci::couple::runtime::Iid &&unnamed_0) = delete;
        //::dci::couple::runtime::Future< ::dci::couple::runtime::Interface> get(::dci::couple::runtime::Iid &&unnamed_0);
        ::dci::couple::runtime::void_ join(::streamer::Channel &&unnamed_0) = delete;

    private:
        enum class MethodId
        {
            start,
            stop,
            //...
        };
    };
}





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


        {
            Engine e;

            tryIfaceSerialization::Hub<Engine> h(e);

            ::streamer::Channel ch;
            h.start(std::move(ch));
        }
        return 0;
    }
}










TEST(Streamer, Probe)
{
    //xyz::f();
    //return;

    using namespace dci::couple::runtime;

    {
        Future<Hub> hf = dci::site::testManager()->createService<Hub>();

        GTEST_ASSERT_EQ(hf.hasValue(), true);
    }


    Hub h1 = dci::site::testManager()->createService<Hub>().detachValue<0>();
    Hub h2 = dci::site::testManager()->createService<Hub>().detachValue<0>();

    {
        Hub h3(std::forward<runtime::Interface>(h1));
        if(h3)
        {
            std::cout << h3.stop().error().message() << std::endl;
        }
    }
}
