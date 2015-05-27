#pragma once

#ifdef __GNUC__
#   ifdef _WIN32
#       define DCI_API_EXPORT __declspec(dllexport)
#       define DCI_API_IMPORT __declspec(dllimport)
#   else
#       define DCI_API_EXPORT __attribute__((visibility("default")))
#       define DCI_API_IMPORT __attribute__((visibility("default")))
#   endif
#elif _MSC_VER
#   define DCI_API_EXPORT __declspec(dllexport)
#   define DCI_API_IMPORT __declspec(dllimport)
#else
#   pragma message "unsupported compiler for binary exports"
#   define DCI_API_EXPORT
#   define DCI_API_IMPORT
#endif
