// Created via CMake from template global.h.in
// WARNING! Any changes to this file will be overwritten by the next CMake run!

#ifndef QT_FORK_AWESOME_GLOBAL
#define QT_FORK_AWESOME_GLOBAL

#include <c++utilities/application/global.h>

#ifdef QT_FORK_AWESOME_STATIC
#define QT_FORK_AWESOME_EXPORT
#define QT_FORK_AWESOME_IMPORT
#else
#define QT_FORK_AWESOME_EXPORT CPP_UTILITIES_GENERIC_LIB_EXPORT
#define QT_FORK_AWESOME_IMPORT CPP_UTILITIES_GENERIC_LIB_IMPORT
#endif

/*!
 * \def QT_FORK_AWESOME_EXPORT
 * \brief Marks the symbol to be exported by the qtforkawesome library.
 */

/*!
 * \def QT_FORK_AWESOME_IMPORT
 * \brief Marks the symbol to be imported from the qtforkawesome library.
 */

#endif // QT_FORK_AWESOME_GLOBAL
