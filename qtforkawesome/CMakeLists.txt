# additional meta data
set(META_PROJECT_VARNAME QT_FORK_AWESOME)
set(META_APP_NAME "Qt ForkAwesome")
set(META_APP_DESCRIPTION "Library that bundles ForkAwesome for use within Qt applications")

# add project files
set(HEADER_FILES renderer.h icon.h iconfwd.h utils.h)
set(SRC_FILES renderer.cpp utils.cpp)
set(QT_TESTS renderer)
set(DOC_FILES ../README.md)

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
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.14")
    file(CREATE_LINK "${FORK_AWESOME_FONT_FILE}" "${CMAKE_CURRENT_BINARY_DIR}/${META_FONT_FILE_NAME}")
else ()
    file(COPY "${FORK_AWESOME_FONT_FILE}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endif ()
file(WRITE "${RES_FILE_PATH}" "${RES_FILE_DATA}")
list(APPEND RES_FILES "${RES_FILE_PATH}")

# create rule for generating header file
find_program(PERL perl)
set(GENERATED_HEADERS_PATH "${CMAKE_CURRENT_BINARY_DIR}/headers")
set(ICONS_HEADER_FILE "${GENERATED_HEADERS_PATH}/private/icons.h")
set(ID_MAPPING_HEADER_FILE "${GENERATED_HEADERS_PATH}/private/idmapping.h")
file(MAKE_DIRECTORY "${GENERATED_HEADERS_PATH}/private")
list(APPEND SRC_FILES "${ICONS_HEADER_FILE}" "${ID_MAPPING_HEADER_FILE}")
set(META_TIDY_EXCLUDE_REGEX ".*/headers/private.*")
add_custom_command(
    OUTPUT "${ICONS_HEADER_FILE}" "${ID_MAPPING_HEADER_FILE}"
    COMMAND "${PERL}" "${CMAKE_CURRENT_SOURCE_DIR}/scripts/yaml2enum.pl" "${FORK_AWESOME_ICON_DEFINITIONS}"
            "${ICONS_HEADER_FILE}" "${ID_MAPPING_HEADER_FILE}"
    DEPENDS "${FORK_AWESOME_ICON_DEFINITIONS}"
    COMMENT "Icon header")

# use headers and CMake modules from c++utilities and qtutilities
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
    DESTINATION "${INCLUDE_SUBDIR}/${META_PROJECT_NAME}/private"
    COMPONENT header)