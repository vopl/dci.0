#pragma once
#include <dci/api.hpp>

#ifdef dci_couple_meta_EXPORTS
#   define APIDCI_COUPLE_META DCI_API_EXPORT
#else
#   define APIDCI_COUPLE_META DCI_API_IMPORT
#endif
