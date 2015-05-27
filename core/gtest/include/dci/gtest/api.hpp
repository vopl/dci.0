#pragma once
#include <dci/api.hpp>

#ifdef dci_gtest_EXPORTS
#   define APIDCI_GTEST DCI_API_EXPORT
#else
#   define APIDCI_GTEST DCI_API_IMPORT
#endif
