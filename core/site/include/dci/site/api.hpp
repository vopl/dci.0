#pragma once
#include <dci/api.hpp>

#ifdef dci_site_EXPORTS
#   define APIDCI_SITE DCI_API_EXPORT
#else
#   define APIDCI_SITE DCI_API_IMPORT
#endif
