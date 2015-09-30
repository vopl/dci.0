#include <dci/gtest.hpp>
#include <dci/site.hpp>
#include <dci/async.hpp>
#include <dci/couple/runtime.hpp>
#include <dci/couple/serialize.hpp>
#include "streamer.hpp"
#include "streamerHandlerSkel.hpp"
#include "streamerSerializer.hpp"


/////////////////////////////////////////////////////////////////////

using namespace streamer;
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










TEST(Streamer, Probe)
{
    {
        xyz::Engine e;

        ::streamer::Channel h;

        ::streamer::Stream::Traffic tr;
        h._out.flow(std::move(tr));

    }

}
