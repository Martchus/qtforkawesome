// Created via CMake from template global.h.in
// WARNING! Any changes to this file will be overwritten by the next CMake run!

#ifndef QT_FORK_AWESOME_QUICK_IMAGE_PROVIDER_GLOBAL
#define QT_FORK_AWESOME_QUICK_IMAGE_PROVIDER_GLOBAL

#include <c++utilities/application/global.h>

#ifdef QT_FORK_AWESOME_QUICK_IMAGE_PROVIDER_STATIC
#define QT_FORK_AWESOME_QUICK_IMAGE_PROVIDER_EXPORT
#define QT_FORK_AWESOME_QUICK_IMAGE_PROVIDER_IMPORT
#else
#define QT_FORK_AWESOME_QUICK_IMAGE_PROVIDER_EXPORT CPP_UTILITIES_GENERIC_LIB_EXPORT
#define QT_FORK_AWESOME_QUICK_IMAGE_PROVIDER_IMPORT CPP_UTILITIES_GENERIC_LIB_IMPORT
#endif

/*!
 * \def QT_FORK_AWESOME_QUICK_IMAGE_PROVIDER_EXPORT
 * \brief Marks the symbol to be exported by the qtforkawesomequickimageprovider library.
 */

/*!
 * \def QT_FORK_AWESOME_QUICK_IMAGE_PROVIDER_IMPORT
 * \brief Marks the symbol to be imported from the qtforkawesomequickimageprovider library.
 */

#endif // QT_FORK_AWESOME_QUICK_IMAGE_PROVIDER_GLOBAL
