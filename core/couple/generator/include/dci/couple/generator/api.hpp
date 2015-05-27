#pragma once
#include <dci/api.hpp>

#ifdef dci_couple_generator_EXPORTS
#   define APIDCI_COUPLE_GENERATOR DCI_API_EXPORT
#else
#   define APIDCI_COUPLE_GENERATOR DCI_API_IMPORT
#endif
