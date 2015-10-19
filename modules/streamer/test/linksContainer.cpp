#include <dci/gtest.hpp>
#include "../src/impl/links/container.hpp"

#include <deque>

using namespace dci::couple::runtime;
using namespace streamer;
using namespace ::impl::links;

#define AMOUNT 4000

struct LinkStub
    : public dci::mm::NewDelete<LinkStub>
{
    ServiceHub::ServiceId _id = 0;

    void setId(ServiceHub::ServiceId id)
    {
        _id = id;
    }
};

TEST(LinksContainer, Create)
{
    //создать-удалить пустой объект
    std::size_t mBefore = dci::mm::occupied();
    {
        Container<LinkStub> c;
    }
    ASSERT_EQ(mBefore, dci::mm::occupied());
}

TEST(LinksContainer, AddDel)
{
    //добавить-удалить один линк
    std::size_t mBefore = dci::mm::occupied();
    {
        Container<LinkStub> c;

        LinkStub *l = c.add();
        ASSERT_NE(l, nullptr);
        ASSERT_EQ(c.get(l->_id), l);
        c.del(l->_id);
    }
    ASSERT_EQ(mBefore, dci::mm::occupied());
}

TEST(LinksContainer, AddDelMax)
{
    //добавить-удалить до максимума
    std::size_t mBefore = dci::mm::occupied();

    auto f = [](auto max) {

        Container<LinkStub, max.value> c;


        ASSERT_TRUE(c.add(max.value-1));
        c.del(max.value-1);

        ASSERT_TRUE(!c.add(max.value));
        c.add(max.value);


        for(std::size_t i(0); i<max.value; ++i)
        {
            LinkStub *l = c.add();
            ASSERT_NE(l, nullptr);
            ASSERT_EQ(c.get(l->_id), l);
        }

        ASSERT_TRUE(!c.add());
        ASSERT_TRUE(!c.add(max.value-1));
        ASSERT_TRUE(!c.add(max.value));

        for(std::size_t i(0); i<max.value; ++i)
        {
            ASSERT_FALSE(!c.del(i));
        }
    };

    f(std::integral_constant<std::size_t, 3>());
    f(std::integral_constant<std::size_t, 7>());
    f(std::integral_constant<std::size_t, 24>());
    f(std::integral_constant<std::size_t, 81>());
    f(std::integral_constant<std::size_t, 123>());
    f(std::integral_constant<std::size_t, 513>());
    f(std::integral_constant<std::size_t, 1092>());
    f(std::integral_constant<std::size_t, 4039>());
    f(std::integral_constant<std::size_t, 10634>());

    f(std::integral_constant<std::size_t, 4>());
    f(std::integral_constant<std::size_t, 8>());
    f(std::integral_constant<std::size_t, 16>());
    f(std::integral_constant<std::size_t, 32>());
    f(std::integral_constant<std::size_t, 64>());
    f(std::integral_constant<std::size_t, 128>());
    f(std::integral_constant<std::size_t, 256>());
    f(std::integral_constant<std::size_t, 512>());
    f(std::integral_constant<std::size_t, 1024>());
    f(std::integral_constant<std::size_t, 2048>());
    f(std::integral_constant<std::size_t, 4096>());
    f(std::integral_constant<std::size_t, 8192>());

    ASSERT_EQ(mBefore, dci::mm::occupied());
}

TEST(LinksContainer, InvalidDel)
{
    //удалять неправильные линки
    std::size_t mBefore = dci::mm::occupied();
    {
        Container<LinkStub> c;

        for(std::size_t i(0); i<AMOUNT; ++i)
        {
            ASSERT_TRUE(!c.get(i));
            ASSERT_TRUE(!c.del(i));
        }
    }
    ASSERT_EQ(mBefore, dci::mm::occupied());
}


TEST(LinksContainer, AddDelMany)
{
    //добавить-удалить много линков

    std::set<LinkStub *> links;
    std::map<Id, LinkStub *> id2link;

    std::size_t mBefore = dci::mm::occupied();
    {
        Container<LinkStub> c;

        for(std::size_t i(0); i<AMOUNT; i++)
        {
            LinkStub *l = c.add();

            ASSERT_NE(l, nullptr);
            ASSERT_EQ(c.get(l->_id), l);
            ASSERT_TRUE(links.end() == links.find(l));
            ASSERT_TRUE(id2link.end() == id2link.find(l->_id));

            links.insert(l);
            id2link[l->_id] = l;


            //удалить неправильный линк
            {
                Id id = AMOUNT + 100500;
                ASSERT_TRUE(!c.get(id));
                ASSERT_TRUE(!c.del(id));
            }

        }

        while(!links.empty())
        {
            LinkStub *l = *links.begin();
            links.erase(links.begin());
            id2link.erase(l->_id);

            c.del(l->_id);
        }
    }
    ASSERT_EQ(mBefore, dci::mm::occupied());
}

TEST(LinksContainer, AddDelRndGet)
{
    //добавить-удалить много линков, несколько раз рандомом

    std::deque<LinkStub *> links;

    std::size_t mBefore = dci::mm::occupied();
    {
        Container<LinkStub> c;

        for(std::size_t i(0); i<AMOUNT; i++)
        {
            LinkStub *l = c.add();

            ASSERT_NE(l, nullptr);
            ASSERT_EQ(c.get(l->_id), l);

            links.push_back(l);
        }

        for(std::size_t i(0); i<AMOUNT; i++)
        {
            LinkStub *l = links[static_cast<std::size_t>(rand())%links.size()];

            ASSERT_NE(l, nullptr);
            ASSERT_EQ(c.get(l->_id), l);
        }

        while(!links.empty())
        {
            auto iter = links.begin()+static_cast<std::ptrdiff_t>((static_cast<std::size_t>(rand())%links.size()));
            LinkStub *l = *iter;
            links.erase(iter);

            auto id = l->_id;
            ASSERT_EQ(c.get(id), l);
            c.del(id);

            {
                ASSERT_TRUE(!c.get(id));
                ASSERT_TRUE(!c.del(id));
            }
        }
    }
    ASSERT_EQ(mBefore, dci::mm::occupied());
}


TEST(LinksContainer, RndAddDelGet)
{
    //добавить-удалить много линков, несколько раз рандомом, получать их

    std::deque<LinkStub *> links;

    std::size_t mBefore = dci::mm::occupied();
    {
        Container<LinkStub> c;

        for(std::size_t i(0); i<AMOUNT; i++)
        {
            LinkStub *l = c.add();

            ASSERT_NE(l, nullptr);
            ASSERT_EQ(c.get(l->_id), l);

            links.push_back(l);
        }

        for(std::size_t i(0); i<10; i++)
        {
            std::size_t delAmount = rand() % (AMOUNT/10);
            for(std::size_t idel(0); idel<delAmount && links.size() > 0; idel++)
            {
                auto iter = links.begin()+static_cast<std::ptrdiff_t>(static_cast<std::size_t>(rand())%links.size());
                LinkStub *l = *iter;
                links.erase(iter);

                auto id = l->_id;
                ASSERT_EQ(c.get(id), l);
                c.del(id);

                {
                    ASSERT_TRUE(!c.get(id));
                    ASSERT_TRUE(!c.del(id));
                }

            }

            std::size_t addAmount = rand() % (AMOUNT/10);
            for(std::size_t iadd(0); iadd<addAmount && links.size() < AMOUNT; iadd++)
            {
                LinkStub *l = c.add();

                ASSERT_NE(l, nullptr);
                ASSERT_EQ(c.get(l->_id), l);

                links.push_back(l);
            }
        }

        while(!links.empty())
        {
            auto iter = links.begin()+static_cast<std::ptrdiff_t>(static_cast<std::size_t>(rand())%links.size());
            LinkStub *l = *iter;
            links.erase(iter);

            ASSERT_EQ(c.get(l->_id), l);
            c.del(l->_id);
        }
    }
    ASSERT_EQ(mBefore, dci::mm::occupied());
}

TEST(LinksContainer, RndAddIdDelGet)
{
    //добавитьПоID-удалить много линков, несколько раз рандомом, получать их

    std::deque<LinkStub *> links;
    std::set<LinkStub *> slinks;

    std::size_t mBefore = dci::mm::occupied();
    {
        Container<LinkStub> c;

        for(std::size_t i(0); i<AMOUNT; i++)
        {
            Id id = static_cast<Id>(rand());
            if(!c.get(id))
            {
                LinkStub *l = c.add(id);

                ASSERT_NE(l, nullptr);
                ASSERT_EQ(c.get(l->_id), l);

                links.push_back(l);
                slinks.insert(l);
            }
        }

        for(std::size_t i(0); i<10; i++)
        {
            std::size_t delAmount = rand() % (AMOUNT/10);
            for(std::size_t idel(0); idel<delAmount && links.size() > 0; idel++)
            {
                auto iter = links.begin()+static_cast<std::ptrdiff_t>(static_cast<std::size_t>(rand())%links.size());
                LinkStub *l = *iter;
                links.erase(iter);

                auto id = l->_id;
                ASSERT_EQ(c.get(id), l);
                c.del(id);

                {
                    ASSERT_TRUE(!c.get(id));
                    ASSERT_TRUE(!c.del(id));
                }

            }

            std::size_t addAmount = rand() % (AMOUNT/10);
            for(std::size_t iadd(0); iadd<addAmount && links.size() < AMOUNT; iadd++)
            {
                Id id = static_cast<Id>(rand());
                if(!c.get(id))
                {
                    LinkStub *l = c.add(id);

                    ASSERT_NE(l, nullptr);
                    ASSERT_EQ(c.get(l->_id), l);

                    links.push_back(l);
                }
            }
        }

        while(!links.empty())
        {
            auto iter = links.begin()+static_cast<std::ptrdiff_t>(static_cast<std::size_t>(rand())%links.size());
            LinkStub *l = *iter;
            links.erase(iter);

            ASSERT_EQ(c.get(l->_id), l);
            c.del(l->_id);
        }
    }
    ASSERT_EQ(mBefore, dci::mm::occupied());
}
