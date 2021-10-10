# additional meta data
set(META_PROJECT_NAME qtforkawesomequickimageprovider)
set(META_PROJECT_VARNAME QT_FORK_AWESOME_QUICK_IMAGE_PROVIDER)
set(META_APP_NAME "QQuickImageProvider for ForkAwesome")
set(META_APP_DESCRIPTION "QQuickImageProvider for ForkAwesome")

# add project files
set(HEADER_FILES provider.h)
set(SRC_FILES provider.cpp)
set(DOC_FILES ../README.md)

# use headers and CMake modules from c++utilities and qtutilities
use_cpp_utilities(ONLY_HEADERS VISIBILITY PUBLIC)
use_qt_utilities(ONLY_HEADERS VISIBILITY PRIVATE)

if (NAMESPACE)
    set(NAMESPACE_PREFIX "${NAMESPACE}-")
endif ()

# use main qtforkawesome library
find_package(${NAMESPACE_PREFIX}qtforkawesome${CONFIGURATION_PACKAGE_SUFFIX_QTFORKAWESOME} ${META_APP_VERSION} REQUIRED)
use_qt_fork_awesome()

# use Qt Gui module
list(APPEND ADDITIONAL_QT_MODULES Gui Quick)

# include modules to apply configuration
include(BasicConfig)
include(QtConfig)
include(WindowsResources)
include(LibraryTarget)
include(Doxygen)
include(ConfigHeader)
