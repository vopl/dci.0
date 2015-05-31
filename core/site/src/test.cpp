#include <dci/site/test.hpp>
#include <dci/gtest.hpp>

namespace dci { namespace site
{
    TestStage g_testStage = TestStage::null;
    Manager *g_testManager = nullptr;

    TestStage testStage()
    {
        return g_testStage;
    }

    Manager *testManager()
    {
        return g_testManager;
    }

}}
