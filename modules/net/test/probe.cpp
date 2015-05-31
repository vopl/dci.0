#include <dci/gtest.hpp>
#include <dci/async.hpp>
#include <dci/site/test.hpp>
#include <dci/site/manager.hpp>
#include <dci/couple/runtime.hpp>

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

    EXPECT_FALSE(netHost.hasError());

    Host nh = netHost.detachValue<0>();

    map<uint32, Link> links = nh.links().detachValue<0>();
    EXPECT_FALSE(links.empty());

    //links
    for(auto & linkp : links)
    {
        Link &link = linkp.second;

        {
            auto v = link.id();
            EXPECT_FALSE(v.hasError());
            EXPECT_EQ(v.value<0>(), linkp.first);
        }

        {
            auto v = link.name();
            EXPECT_FALSE(v.hasError());
        }

        {
            auto v = link.flags();
            EXPECT_FALSE(v.hasError());
        }

        {
            auto v = link.mtu();
            EXPECT_FALSE(v.hasError());
        }

        {
            auto v = link.ip4();
            EXPECT_FALSE(v.hasError());
        }

        {
            auto v = link.ip6();
            EXPECT_FALSE(v.hasError());
        }
    }

    //ip4
    {
        auto v = nh.ip4StreamServer();
        EXPECT_FALSE(v.hasError());
    }
    {
        auto v = nh.ip4StreamClient();
        EXPECT_FALSE(v.hasError());
    }
    {
        auto v = nh.ip4DatagramChannel();
        EXPECT_FALSE(v.hasError());
    }

    //ip6
    {
        auto v = nh.ip6StreamServer();
        EXPECT_FALSE(v.hasError());
    }
    {
        auto v = nh.ip6StreamClient();
        EXPECT_FALSE(v.hasError());
    }
    {
        auto v = nh.ip6DatagramChannel();
        EXPECT_FALSE(v.hasError());
    }

    //local
    {
        auto v = nh.localStreamServer();
        EXPECT_FALSE(v.hasError());
    }
    {
        auto v = nh.localStreamClient();
        EXPECT_FALSE(v.hasError());
    }
}

TEST_F(Net, StreamIp4)
{
    async::Future<std::error_code, Host> netHost = _manager->createService<Host>();
    EXPECT_FALSE(netHost.hasError());
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
        EXPECT_FALSE(fs.hasError());
        ip4::stream::Server s = fs;

        EXPECT_FALSE(s.bind(ip4::Address(addr)).hasError());
        EXPECT_FALSE(s.listen().hasError());

        auto fch = s.accept();
        EXPECT_FALSE(fch.hasError());

        ip4::stream::Channel ch = fch;
        ch.write(ch.read().detachValue<0>());

        sdone.set();
    });

    //client
    spawn([&](){
        auto fc = nh.ip4StreamClient();
        EXPECT_FALSE(fc.hasError());
        ip4::stream::Client c = fc;

        auto fch = c.connect(ip4::Address(addr));
        EXPECT_FALSE(fch.hasError());

        ip4::stream::Channel ch = fch;

        Bytes bs;
        bs.append("tratata data");
        ch.write(std::move(bs));
        bs = ch.read();

        cdone.set();
    });

    async::acquireAll(sdone, cdone);
}
