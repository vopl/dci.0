#include <dci/gtest.hpp>
#include "../src/impl/links/local.hpp"
#include "../src/impl/links/local/linkId.cpp"

using namespace dci::couple::runtime;
using namespace streamer;
using namespace ::impl::links;
using namespace ::impl::links::local;

struct LinkStub
{
    ServiceHub::ServiceId _id = 0;

    void setId(ServiceHub::ServiceId id)
    {
        _id = id;
    }

    Future<Interface> shutdown()
    {
        delete this;
        return Future<Interface>(make_error_code(std::errc::address_not_available));
    }
};

TEST(LocalLinksContainer, Create)
{
    Local<LinkStub> c;
}

TEST(LocalLinksContainer, AddDel)
{
    Local<LinkStub> c;

    LinkStub *l = c.add();
    ASSERT_NE(l, nullptr);
    ASSERT_EQ(c.get(l->_id), l);
    c.del(l->_id);
}

TEST(LocalLinksContainer, AddDelStress)
{
    std::set<LinkStub *> links;
    std::map<LinkId, LinkStub *> id2link;

    {
        //(forward, back, random) x (add-del, get)
        Local<LinkStub> c;

        for(std::size_t i(0); i<10000; i++)
        {
            LinkStub *l = c.add();

            ASSERT_NE(l, nullptr);
            ASSERT_EQ(c.get(l->_id), l);
            ASSERT_TRUE(links.end() == links.find(l));
            ASSERT_TRUE(id2link.end() == id2link.find(l->_id));

            links.insert(l);
            id2link[l->_id] = l;
        }

        while(!links.empty())
        {
            LinkStub *l = *links.begin();
            links.erase(links.begin());
            id2link.erase(l->_id);

            c.del(l->_id);
        }
    }
}
