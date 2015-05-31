#include <dci/gtest.hpp>
#include <dci/async.hpp>
#include <dci/site/test.hpp>
#include <dci/site/manager.hpp>
#include <dci/couple/runtime.hpp>

#include <chrono>

#include <net.hpp>

using namespace dci;
using namespace dci::site;
using namespace dci::async;
using namespace dci::couple::runtime;

using namespace net;


/////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
class Net
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

/////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
TEST_F(Net, Host)
{
    async::Future<std::error_code, Host> netHost = _manager->createService<Host>();

    ASSERT_FALSE(netHost.hasError());

    Host nh = netHost.detachValue<0>();

    map<uint32, Link> links = nh.links().detachValue<0>();
    ASSERT_FALSE(links.empty());

    //links
    for(auto & linkp : links)
    {
        Link &link = linkp.second;

        {
            auto v = link.id();
            ASSERT_FALSE(v.hasError());
            EXPECT_EQ(v.value<0>(), linkp.first);
        }

        {
            auto v = link.name();
            ASSERT_FALSE(v.hasError());
        }

        {
            auto v = link.flags();
            ASSERT_FALSE(v.hasError());
        }

        {
            auto v = link.mtu();
            ASSERT_FALSE(v.hasError());
        }

        {
            auto v = link.ip4();
            ASSERT_FALSE(v.hasError());
        }

        {
            auto v = link.ip6();
            ASSERT_FALSE(v.hasError());
        }
    }

    //ip4
    {
        auto v = nh.ip4StreamServer();
        ASSERT_FALSE(v.hasError());
    }
    {
        auto v = nh.ip4StreamClient();
        ASSERT_FALSE(v.hasError());
    }
    {
        auto v = nh.ip4DatagramChannel();
        ASSERT_FALSE(v.hasError());
    }

    //ip6
    {
        auto v = nh.ip6StreamServer();
        ASSERT_FALSE(v.hasError());
    }
    {
        auto v = nh.ip6StreamClient();
        ASSERT_FALSE(v.hasError());
    }
    {
        auto v = nh.ip6DatagramChannel();
        ASSERT_FALSE(v.hasError());
    }

    //local
    {
        auto v = nh.localStreamServer();
        ASSERT_FALSE(v.hasError());
    }
    {
        auto v = nh.localStreamClient();
        ASSERT_FALSE(v.hasError());
    }
}

TEST_F(Net, StreamIp4)
{
    async::Future<std::error_code, Host> netHost = _manager->createService<Host>();
    ASSERT_FALSE(netHost.hasError());
    Host nh = netHost.detachValue<0>();

    ip4::Address addr;
    addr.octets[0] = 127;
    addr.octets[1] = 0;
    addr.octets[2] = 0;
    addr.octets[3] = 1;

    Event sdone, cdone;

    //server
    spawn([&](){
        auto fs = nh.ip4StreamServer();
        ASSERT_FALSE(fs.hasError());
        ip4::stream::Server s = fs;

        ASSERT_FALSE(s.bind(ip4::Address(addr)).hasError());
        ASSERT_FALSE(s.listen().hasError());

        auto fch = s.accept();
        ASSERT_FALSE(fch.hasError());

        ip4::stream::Channel ch = fch;
        ch.write(ch.read().detachValue<0>());

        sdone.set();
    });

    //client
    spawn([&](){
        auto fc = nh.ip4StreamClient();
        ASSERT_FALSE(fc.hasError());
        ip4::stream::Client c = fc;

        auto fch = c.connect(ip4::Address(addr));
        ASSERT_FALSE(fch.hasError());

        ip4::stream::Channel ch = fch;

        Bytes bs;
        bs.append("tratata data");
        ch.write(std::move(bs));
        bs = ch.read();

        cdone.set();
    });

    async::acquireAll(sdone, cdone);
}

TEST_F(Net, SpamerBench)
{
    async::Future<std::error_code, Host> netHost = _manager->createService<Host>();
    ASSERT_FALSE(netHost.hasError());
    Host nh = netHost.detachValue<0>();

    ip4::Address addr;
    addr.octets[0] = 127;
    addr.octets[1] = 0;
    addr.octets[2] = 0;
    addr.octets[3] = 1;

    addr.port = 1234;

    Event cdone;

    //client
    spawn([&](){
        auto fc = nh.ip4StreamClient();
        ASSERT_FALSE(fc.hasError());
        ip4::stream::Client c = fc;

        auto fch = c.connect(ip4::Address(addr));
        ASSERT_FALSE(fch.hasError());

        ip4::stream::Channel ch = fch;



        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

        for(std::size_t i(1); i<=1000000; i++)
        {

            Bytes bs;
            bs.append("tratata data");
            ch.write(std::move(bs));

            bs = ch.read();

            if(!(i%100000))
            {
                std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();

                std::cout << "Reply is: ";
                std::cout<<bs.toString()<<std::endl;
                std::cout << ", "<<std::chrono::duration_cast<std::chrono::duration<double>>((stop - start)).count();
                std::cout << "\n";

                start = std::chrono::high_resolution_clock::now();

            }
        }

        cdone.set();
    });

    async::acquireAll(cdone);
}
