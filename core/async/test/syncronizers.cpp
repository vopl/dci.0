#include <dci/gtest.hpp>
#include <dci/async.hpp>

using namespace dci::async;

/////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
TEST(async, event)
{
    Event e;
    int step = 0;

    spawn([&]()
    {
        ASSERT_EQ(0, step);
        EXPECT_FALSE(e.canAcquire());
        ASSERT_EQ(0, step);
        EXPECT_FALSE(e.tryAcquire());
        ASSERT_EQ(0, step);

        step = 1;

        e.acquire();

        ASSERT_EQ(3, step);
        EXPECT_TRUE(e.canAcquire());
        ASSERT_EQ(3, step);
        EXPECT_TRUE(e.tryAcquire());
        ASSERT_EQ(3, step);

        step = 4;
        e.reset();
        e.acquire();
        ASSERT_EQ(5, step);

        step = 6;
        yield();

        ASSERT_EQ(6, step);
        step = 7;
    });

    ASSERT_EQ(0, step);
    dci::async::executeReadyCoros();
    ASSERT_EQ(1, step);

    spawn([&]()
    {
        ASSERT_EQ(1, step);
        step = 2;

        e.set();
        ASSERT_EQ(2, step);
        step = 3;

        yield();
        ASSERT_EQ(4, step);

        step = 5;
        yield();
        yield();
        yield();
        ASSERT_EQ(5, step);

        e.set();
        yield();
        ASSERT_EQ(6, step);

    });

    ASSERT_EQ(1, step);
    dci::async::executeReadyCoros();
    ASSERT_EQ(7, step);
}
