cmake_minimum_required(VERSION 3.3.0 FATAL_ERROR)

# meta data
set(META_PROJECT_NAME qtforkawesome)
set(META_PROJECT_VARNAME QT_FORK_AWESOME)
set(META_APP_NAME "Qt ForkAwesome")
set(META_APP_AUTHOR "Martchus")
set(META_APP_URL "https://github.com/${META_APP_AUTHOR}/${META_PROJECT_NAME}")
set(META_APP_DESCRIPTION "Library that bundles ForkAwesome for use within Qt applications")
set(META_VERSION_MAJOR 0)
set(META_VERSION_MINOR 0)
set(META_VERSION_PATCH 1)
set(META_APP_VERSION ${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH})
set(META_PUBLIC_QT_MODULES Core Gui)
set(META_ADD_DEFAULT_CPP_UNIT_TEST_APPLICATION OFF)

project(${META_PROJECT_NAME})

# add project files
set(HEADER_FILES qtforkawesome.h)
set(SRC_FILES qtforkawesome.cpp)
set(QT_TESTS rendering)
set(DOC_FILES README.md)

# find/download ForkAwesome's font file and icon definitions
set(FORK_AWESOME_VERSION
    "1.2.0"
    CACHE STRING "specifies the version of ForkAwesome")
set(FORK_AWESOME_REPO_URL "https://raw.githubusercontent.com/ForkAwesome/Fork-Awesome/${FORK_AWESOME_VERSION}")
set(FORK_AWESOME_FONT_FILE
    "${FORK_AWESOME_REPO_URL}/fonts/forkawesome-webfont.woff2"
    CACHE STRING "specifies the location of ForkAwesome's font file")
set(FORK_AWESOME_ICON_DEFINITIONS
    "${FORK_AWESOME_REPO_URL}/src/icons/icons.yml"
    CACHE STRING "specifies the location of ForkAwesome's icon definitions file")
set(DOWNLOAD_DIR "${CMAKE_CURRENT_BINARY_DIR}/download")
file(MAKE_DIRECTORY "${DOWNLOAD_DIR}")
function (checkout_file VARIABLE)
    if ("${${VARIABLE}}" MATCHES "http(s)?:.+")
        get_filename_component(FILE_NAME "${${VARIABLE}}" NAME)
        if (NOT EXISTS "${DOWNLOAD_DIR}/${FILE_NAME}")
            message(STATUS "Downloading ${${VARIABLE}}")
            file(DOWNLOAD "${${VARIABLE}}" "${DOWNLOAD_DIR}/${FILE_NAME}" SHOW_PROGRESS)
        endif ()
        set("${VARIABLE}"
            "${DOWNLOAD_DIR}/${FILE_NAME}"
            PARENT_SCOPE)
    elseif (NOT EXISTS "${${VARIABLE}}")
        message(FATAL_ERROR "${VARIABLE} is set to \"${${VARIABLE}}\" which does not exist.")
    endif ()
endfunction ()
checkout_file(FORK_AWESOME_FONT_FILE)
checkout_file(FORK_AWESOME_ICON_DEFINITIONS)

# configure font file
get_filename_component(META_FONT_FILE_NAME "${FORK_AWESOME_FONT_FILE}" NAME)
set(META_CUSTOM_CONFIG
    "#define META_FONT_FILE_NAME \"${META_FONT_FILE_NAME}\"\n#define META_FORK_AWESOME_VERSION \"${FORK_AWESOME_VERSION}\"\n"
)
set(RES_FILE_PATH "${CMAKE_CURRENT_BINARY_DIR}/font.qrc")
set(RES_FILE_DATA "<RCC><qresource prefix=\"/\"><file>${META_FONT_FILE_NAME}</file></qresource></RCC>")
file(CREATE_LINK "${FORK_AWESOME_FONT_FILE}" "${CMAKE_CURRENT_BINARY_DIR}/${META_FONT_FILE_NAME}")
file(WRITE "${RES_FILE_PATH}" "${RES_FILE_DATA}")
list(APPEND RES_FILES "${RES_FILE_PATH}")

# create rule for generating header file
find_program(PERL perl)
set(GENERATED_HEADERS_PATH "${CMAKE_CURRENT_BINARY_DIR}/headers")
set(ICONS_HEADER_FILE "${GENERATED_HEADERS_PATH}/icons.h")
list(APPEND SRC_FILES "${ICONS_HEADER_FILE}")
file(MAKE_DIRECTORY "${GENERATED_HEADERS_PATH}")
add_custom_command(
    OUTPUT "${ICONS_HEADER_FILE}"
    COMMAND "${PERL}" "${CMAKE_CURRENT_SOURCE_DIR}/scripts/yaml2enum.pl" "${FORK_AWESOME_ICON_DEFINITIONS}"
            "${ICONS_HEADER_FILE}"
    DEPENDS "${FORK_AWESOME_ICON_DEFINITIONS}"
    COMMENT "Icon header")

# find c++utilities and qtutilities
set(CONFIGURATION_PACKAGE_SUFFIX
    ""
    CACHE STRING "sets the suffix for c++utilities")
set(CONFIGURATION_PACKAGE_SUFFIX_QTUTILITIES
    "${CONFIGURATION_PACKAGE_SUFFIX}"
    CACHE STRING "sets the suffix for qtutilities")
find_package(c++utilities${CONFIGURATION_PACKAGE_SUFFIX} 5.5.0 REQUIRED)
find_package(qtutilities${CONFIGURATION_PACKAGE_SUFFIX_QTUTILITIES} 6.3.0 REQUIRED)
use_cpp_utilities(ONLY_HEADERS VISIBILITY PUBLIC)
use_qt_utilities(ONLY_HEADERS VISIBILITY PRIVATE)

# use Qt Gui module
list(APPEND ADDITIONAL_QT_MODULES Gui)

# include modules to apply configuration
include(BasicConfig)
include(QtConfig)
include(WindowsResources)
include(LibraryTarget)
include(Doxygen)
include(ConfigHeader)

# configure test target
include(TestUtilities)
set(QT_TEST_LIBRARIES ${CPP_UTILITIES_LIB} ${META_TARGET_NAME})
use_qt_module(LIBRARIES_VARIABLE "QT_TEST_LIBRARIES" PREFIX "${QT_PACKAGE_PREFIX}" MODULE "Test")
foreach (TEST ${QT_TESTS})
    configure_test_target(TEST_NAME "${TEST}_tests" SRC_FILES "tests/${TEST}.cpp" LIBRARIES "${QT_TEST_LIBRARIES}")
endforeach ()

# ensure generated header can be found during build
target_include_directories(${META_TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${GENERATED_HEADERS_PATH}>)

# install generated header
install(
    FILES "${ICONS_HEADER_FILE}"
    DESTINATION "${INCLUDE_SUBDIR}/${META_PROJECT_NAME}"
    COMPONENT header)