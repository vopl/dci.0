#pragma once
#include <dci/api.hpp>

#ifdef dci_couple_serialize_EXPORTS
#   define APIDCI_COUPLE_SERIALIZE DCI_API_EXPORT
#else
#   define APIDCI_COUPLE_SERIALIZE DCI_API_IMPORT
#endif
