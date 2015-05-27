#pragma once
#include <dci/api.hpp>

#ifdef dci_couple_runtime_EXPORTS
#   define APIDCI_COUPLE_RUNTIME DCI_API_EXPORT
#else
#   define APIDCI_COUPLE_RUNTIME DCI_API_IMPORT
#endif
