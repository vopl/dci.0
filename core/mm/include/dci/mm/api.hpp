#pragma once
#include <dci/api.hpp>

#ifdef dci_mm_EXPORTS
#   define APIDCI_MM DCI_API_EXPORT
#else
#   define APIDCI_MM DCI_API_IMPORT
#endif
