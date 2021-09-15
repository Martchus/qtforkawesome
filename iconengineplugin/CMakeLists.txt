# additional meta data
set(META_PROJECT_NAME qtforkawesomeiconengine)
set(META_PROJECT_VARNAME QT_FORK_AWESOME_ICON_ENGINE)
set(META_APP_NAME "QIconEngine for ForkAwesome")
set(META_APP_DESCRIPTION "QIconEngine for ForkAwesome")
set(META_PROJECT_TYPE qtplugin)
set(META_PLUGIN_CATEGORY iconengines)

set(HEADER_FILES iconengine.h)
set(SRC_FILES iconengine.cpp main.cpp)

# use headers and CMake modules from c++utilities and qtutilities
use_cpp_utilities(ONLY_HEADERS VISIBILITY PUBLIC)
use_qt_utilities(ONLY_HEADERS VISIBILITY PRIVATE)

# use main qtforkawesome library
find_package(qtforkawesome${CONFIGURATION_PACKAGE_SUFFIX_QTFORKAWESOME} ${META_APP_VERSION} REQUIRED)
use_qt_fork_awesome()

# use Qt Gui module
list(APPEND ADDITIONAL_QT_MODULES Gui)

# configure styling support
option(ENABLE_STYLE_SUPPORT "whether to apply icon mode via QStyle (pulls in Qt Widgets dependency)" OFF)
if (ENABLE_STYLE_SUPPORT)
    list(APPEND ADDITIONAL_QT_MODULES Widgets)
    set(META_CUSTOM_CONFIG "#define ${META_PROJECT_VARNAME}_ENABLE_STYLE_SUPPORT 1\n")
endif ()

include(BasicConfig)
include(QtGuiConfig)
include(QtConfig)
include(WindowsResources)
include(LibraryTarget)
include(ConfigHeader)
