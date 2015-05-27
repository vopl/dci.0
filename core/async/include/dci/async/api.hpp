#pragma once
#include <dci/api.hpp>

#ifdef dci_async_EXPORTS
#   define APIDCI_ASYNC DCI_API_EXPORT
#else
#   define APIDCI_ASYNC DCI_API_IMPORT
#endif
