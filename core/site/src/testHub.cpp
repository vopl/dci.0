#include <dci/site/testHub.hpp>
#include <dci/gtest.hpp>

namespace dci { namespace site { namespace testHub
{
    namespace
    {
        Manager *g_manager = nullptr;
    }

    Stage stage()
    {
        return Stage::DCI_SITE_TESTHUB_STAGE;
    }

    Manager *manager()
    {
        return g_manager;
    }

}}}

extern "C"
{
    int dciTestHubEntryPoint(int argc, char *argv[], dci::site::Manager *manager)
    {
        dci::site::testHub::g_manager = manager;
        testing::InitGoogleTest(&argc, argv);
        int res = RUN_ALL_TESTS();
        dci::site::testHub::g_manager = nullptr;

        return res;
    }
}
