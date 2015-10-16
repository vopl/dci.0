#include <dci/gtest.hpp>

extern "C"
{
    DCI_API_EXPORT int dciTestHubEntryPoint(int argc, char *argv[]);

    DCI_API_EXPORT int dciTestHubEntryPoint(int argc, char *argv[])
    {
        testing::InitGoogleTest(&argc, argv);
        int res = RUN_ALL_TESTS();

        return res;
    }
}
