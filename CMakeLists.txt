cmake_minimum_required(VERSION 3.17.0 FATAL_ERROR)

# meta data
set(META_PROJECT_NAME qtforkawesome)
set(META_APP_AUTHOR "Martchus")
set(META_APP_URL "https://github.com/${META_APP_AUTHOR}/${META_PROJECT_NAME}")
set(META_VERSION_MAJOR 0)
set(META_VERSION_MINOR 2)
set(META_VERSION_PATCH 3)
set(META_APP_VERSION ${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH})
set(META_SOVERSION 1)
set(META_PUBLIC_QT_MODULES Core Gui)
set(META_ADD_DEFAULT_CPP_UNIT_TEST_APPLICATION OFF)

project(${META_PROJECT_NAME})
enable_testing()

# find c++utilities and qtutilities
set(CONFIGURATION_PACKAGE_SUFFIX
    ""
    CACHE STRING "sets the suffix for c++utilities")
set(CONFIGURATION_PACKAGE_SUFFIX_QTUTILITIES
    "${CONFIGURATION_PACKAGE_SUFFIX}"
    CACHE STRING "sets the suffix for qtutilities")
set(CONFIGURATION_PACKAGE_SUFFIX_QTFORKAWESOME
    "${CONFIGURATION_PACKAGE_SUFFIX_QTUTILITIES}"
    CACHE STRING "sets the suffix for qtforkawesome")
set(PACKAGE_NAMESPACE
    ""
    CACHE STRING "sets the namespace (prefix) for find_package() calls to packages configured via c++utilities")
if (PACKAGE_NAMESPACE)
  set(PACKAGE_NAMESPACE_PREFIX "${PACKAGE_NAMESPACE}-")
endif ()
find_package(${PACKAGE_NAMESPACE_PREFIX}c++utilities${CONFIGURATION_PACKAGE_SUFFIX} 5.5.0 REQUIRED)
find_package(${PACKAGE_NAMESPACE_PREFIX}qtutilities${CONFIGURATION_PACKAGE_SUFFIX_QTUTILITIES} 6.3.0 REQUIRED)

add_subdirectory(${META_PROJECT_NAME})
set(${PACKAGE_NAMESPACE_PREFIX}${META_PROJECT_NAME}${CONFIGURATION_PACKAGE_SUFFIX_QTFORKAWESOME}_DIR "${CMAKE_CURRENT_BINARY_DIR}/${META_PROJECT_NAME}")
add_subdirectory(iconengineplugin)

option(ENABLE_QT_QUICK_LIBRARY "enables building the library for Qt Quick integration" ON)
if (ENABLE_QT_QUICK_LIBRARY)
    add_subdirectory(qtquickforkawesome)
endif ()
