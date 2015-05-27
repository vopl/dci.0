#pragma once
#include <dci/api.hpp>

#ifdef dci_poll_EXPORTS
#   define APIDCI_POLL DCI_API_EXPORT
#else
#   define APIDCI_POLL DCI_API_IMPORT
#endif
