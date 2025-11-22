cmake_minimum_required(VERSION "3.25" FATAL_ERROR)

set(CMAKE_UTIL_FILE "" CACHE STRING "...")
set(USE_CONAN "1" CACHE BOOL "...")

message(STATUS "USE_CONAN: '${USE_CONAN}'")

string(REPLACE "." ";" PRESET_NAME_ELEMENTS "${PRESET_NAME}")

if("${USE_CONAN}")
    if("windows" IN_LIST "PRESET_NAME_ELEMENTS")
        find_program(CONAN_COMMAND NAMES "conan.exe" PATHS ENV CONAN_PATH ENV PATH REQUIRED NO_CACHE NO_DEFAULT_PATH)
    else()
        find_program(CONAN_COMMAND NAMES "conan" PATHS ENV CONAN_PATH ENV PATH REQUIRED NO_CACHE NO_DEFAULT_PATH)
    endif()
endif()

if("${CMAKE_UTIL_FILE}" STREQUAL "")
    if("$ENV{CMAKE_UTIL_FILE}" STREQUAL "")
        if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/build/dependencies/direct_deploy/exqudens-cmake/cmake/util.cmake")
            if("${USE_CONAN}")
                #[[execute_process(
                    COMMAND "${CONAN_COMMAND}" "user" "$ENV{CONAN_LOGIN_USERNAME}" "-r" "artifactory" "-p"
                    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
                    ENCODING "UTF-8"
                    COMMAND_ERROR_IS_FATAL "ANY"
                )]]
                execute_process(
                    COMMAND "${CONAN_COMMAND}" "install" "--tool-requires=exqudens-cmake/0.0.1" "--deployer" "direct_deploy" "--deployer-folder" "${CMAKE_SOURCE_DIR}/build/dependencies"
                    COMMAND_ECHO "STDERR"
                    ENCODING "UTF-8"
                    COMMAND_ERROR_IS_FATAL "ANY"
                )
                set(CMAKE_UTIL_FILE "${CMAKE_CURRENT_SOURCE_DIR}/build/dependencies/direct_deploy/exqudens-cmake/cmake/util.cmake" CACHE STRING "..." FORCE)
            else()
                message(FATAL_ERROR "Unable to resolve CMAKE_UTIL_FILE: '${CMAKE_UTIL_FILE}'")
            endif()
        else()
            set(CMAKE_UTIL_FILE "${CMAKE_CURRENT_SOURCE_DIR}/build/dependencies/direct_deploy/exqudens-cmake/cmake/util.cmake" CACHE STRING "..." FORCE)
        endif()
    else()
        set(CMAKE_UTIL_FILE "$ENV{CMAKE_UTIL_FILE}" CACHE STRING "..." FORCE)
    endif()
endif()

include("${CMAKE_UTIL_FILE}")

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/cmake-install" CACHE PATH "..." FORCE)
set(CMAKE_VERBOSE_MAKEFILE "1" CACHE BOOL "..." FORCE)
set(CMAKE_EXPORT_COMPILE_COMMANDS "1" CACHE BOOL "..." FORCE)
set(SKIP_TEST "0" CACHE BOOL "" FORCE)
set(TEST_GROUP "all" CACHE STRING "...")
set(TEST_CASE "all" CACHE STRING "...")

if("msvc-x64-x64" IN_LIST "PRESET_NAME_ELEMENTS")
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/build/msvc-x64-x64-toolchain.cmake")
        set_msvc_toolchain(TOOLCHAIN_CONTENT
            PROCESSOR "x86_64"
            OS "${CMAKE_HOST_SYSTEM_NAME}"
            VERSION "17"
            HOST "x64"
            TARGET "x64"
            SET_CMAKE_SYSTEM_NAME "0"
            OUTPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/build/msvc-x64-x64-toolchain.cmake"
        )
    endif()
    find_program(CONAN_COMMAND NAMES "conan.exe" PATHS ENV CONAN_PATH ENV PATH REQUIRED NO_CACHE NO_DEFAULT_PATH)
elseif("gcc" IN_LIST PRESET_NAME_ELEMENTS)
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/build/gcc-toolchain.cmake")
        find_file(compilerFile NAMES "gcc" PATHS ENV GCC_PATH ENV PATH REQUIRED NO_CACHE NO_DEFAULT_PATH)
        set_gnu_toolchain(TOOLCHAIN_CONTENT
            PROCESSOR "x86_64"
            OS "${CMAKE_HOST_SYSTEM_NAME}"
            PATH "${compilerFile}"
            SET_CMAKE_SYSTEM_NAME "0"
            OUTPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/build/gcc-toolchain.cmake"
        )
    endif()
    find_program(CONAN_COMMAND NAMES "conan" PATHS ENV CONAN_PATH ENV PATH REQUIRED NO_CACHE NO_DEFAULT_PATH)
endif()

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/name-version.txt" INFO)
string(REPLACE ":" ";" INFO_LIST "${INFO}")
list(GET INFO_LIST 0 RAW_NAME)
list(GET INFO_LIST 1 RAW_VERSION)
string(STRIP "${RAW_NAME}" NAME)
string(STRIP "${RAW_VERSION}" VERSION)
project("${NAME}" VERSION "${VERSION}" LANGUAGES "NONE")

message(STATUS "project: '${PROJECT_NAME}-${PROJECT_VERSION}'")

string(TOLOWER "${PROJECT_NAME}" PROJECT_NAME_LOWER)
string(TOUPPER "${PROJECT_NAME}" PROJECT_NAME_UPPER)
set(PROJECT_VERSION_NOTWEAK "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

message(STATUS "PROJECT_NAME_LOWER: '${PROJECT_NAME_LOWER}'")
message(STATUS "PROJECT_VERSION_NOTWEAK: '${PROJECT_VERSION_NOTWEAK}'")

enable_language("C")
enable_language("CXX")
#include("GenerateExportHeader")
enable_testing()

set(CMAKE_OBJECT_PATH_MAX 1000)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_MODULE_PREFIX "")
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_IMPORT_LIBRARY_PREFIX "")
set(CMAKE_STAGING_PREFIX "")
set(CMAKE_FIND_LIBRARY_PREFIXES "" "lib")
set(CMAKE_FIND_USE_CMAKE_ENVIRONMENT_PATH "0")
set(CMAKE_FIND_USE_CMAKE_SYSTEM_PATH "0")
set(CMAKE_FIND_USE_SYSTEM_ENVIRONMENT_PATH "1")
set(CMAKE_FIND_USE_SYSTEM_PACKAGE_REGISTRY "0")
set(CMAKE_FIND_USE_PACKAGE_REGISTRY "0")
set(CMAKE_FIND_USE_PACKAGE_ROOT_PATH "0")
set(CMAKE_FIND_USE_CMAKE_PATH "1")
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG "0")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS "1")
set(CMAKE_BUILD_RPATH "\$ORIGIN")
set(CMAKE_INSTALL_RPATH "\$ORIGIN")
set(CMAKE_SKIP_BUILD_RPATH "1")
set(CMAKE_BUILD_RPATH_USE_ORIGIN "1")
set(CMAKE_BUILD_WITH_INSTALL_RPATH "1")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH "0")
set(CMAKE_CXX_VISIBILITY_PRESET "default")
set(CMAKE_VISIBILITY_INLINES_HIDDEN "0")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED "1")
set(CMAKE_C_EXTENSIONS "0")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED "1")
set(CMAKE_CXX_EXTENSIONS "0")

separate_arguments(CMAKE_CXX_FLAGS NATIVE_COMMAND "${CMAKE_CXX_FLAGS}")
if(MSVC)
    if(NOT "/EHa" IN_LIST CMAKE_CXX_FLAGS AND "/EHsc" IN_LIST CMAKE_CXX_FLAGS)
        list(REMOVE_ITEM CMAKE_CXX_FLAGS "/EHsc")
        list(APPEND CMAKE_CXX_FLAGS "/EHa")
    endif()
endif()
string(JOIN " " CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

if("${USE_CONAN}")
    set_conan_architecture(CONAN_ARCH "${CMAKE_SYSTEM_PROCESSOR}")
    set_conan_compiler(CONAN_COMPILER "${CMAKE_CXX_COMPILER_ID}")
    set_conan_compiler_version(CONAN_COMPILER_VERSION "${CMAKE_CXX_COMPILER_ID}" "${CMAKE_CXX_COMPILER_VERSION}" MAX_ELEMENTS "1")
    set_conan_compiler_runtime(CONAN_COMPILER_RUNTIME "${CMAKE_MSVC_RUNTIME_LIBRARY}")
    set_conan_compiler_runtime_type(CONAN_COMPILER_RUNTIME_TYPE "${CMAKE_MSVC_RUNTIME_LIBRARY}")

    set_python_boolean(CONAN_INTERFACE_ONLY "${INTERFACE_ONLY}")
    set_python_boolean(CONAN_BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS}")

    set_conan_settings(CONAN_SETTINGS
        "arch=${CONAN_ARCH}"
        "os=${CMAKE_SYSTEM_NAME}"
        "compiler=${CONAN_COMPILER}"
        "compiler.version=${CONAN_COMPILER_VERSION}"
        "compiler.cstd=${CMAKE_C_STANDARD}"
        "compiler.cppstd=${CMAKE_CXX_STANDARD}"
        "compiler.runtime=${CONAN_COMPILER_RUNTIME}"
        "compiler.runtime_type=${CONAN_COMPILER_RUNTIME_TYPE}"
        "compiler.libcxx=${CONAN_COMPILER_LIBCXX}"
        "build_type=${CMAKE_BUILD_TYPE}"
    )
    set_conan_options(CONAN_OPTIONS
        "&:shared=${CONAN_BUILD_SHARED_LIBS}"
        "*:shared=${CONAN_BUILD_SHARED_LIBS}"
    )
endif()

cmake_path(CONVERT "$ENV{CMAKE_USER_HOME}" TO_CMAKE_PATH_LIST CMAKE_USER_HOME NORMALIZE)
if("" STREQUAL "${CMAKE_USER_HOME}")
    cmake_path(CONVERT "$ENV{HOME}" TO_CMAKE_PATH_LIST HOME NORMALIZE)
else()
    set(HOME "${CMAKE_USER_HOME}")
endif()
if(NOT EXISTS "${HOME}" OR NOT IS_DIRECTORY "${HOME}")
    message(FATAL_ERROR "HOME: '${HOME}' not exists or not directory.")
endif()
set(CMAKE_HOME "${HOME}/.cmake")
if(NOT EXISTS "${CMAKE_HOME}")
    file(MAKE_DIRECTORY "${CMAKE_HOME}")
endif()
if(NOT EXISTS "${CMAKE_HOME}" OR NOT IS_DIRECTORY "${CMAKE_HOME}")
    message(FATAL_ERROR "CMAKE_HOME: '${CMAKE_HOME}' not exists or not directory.")
endif()
set(CMAKE_DOWNLOADS "${CMAKE_HOME}/downloads")
if(NOT EXISTS "${CMAKE_DOWNLOADS}")
    file(MAKE_DIRECTORY "${CMAKE_DOWNLOADS}")
endif()
if(NOT EXISTS "${CMAKE_DOWNLOADS}" OR NOT IS_DIRECTORY "${CMAKE_DOWNLOADS}")
    message(FATAL_ERROR "CMAKE_DOWNLOADS: '${CMAKE_DOWNLOADS}' is not directory.")
endif()
if(NOT EXISTS "${CMAKE_DOWNLOADS}/${PROJECT_NAME_LOWER}-${PROJECT_VERSION_NOTWEAK}.zip")
    message(STATUS "download ...")
    file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/build/tmp")
    file(DOWNLOAD
        "https://github.com/${PROJECT_NAME_LOWER}/${PROJECT_NAME_LOWER}/archive/refs/tags/v${PROJECT_VERSION_NOTWEAK}.zip"
        "${PROJECT_SOURCE_DIR}/build/tmp/${PROJECT_NAME_LOWER}-${PROJECT_VERSION_NOTWEAK}.zip"
        EXPECTED_MD5 "f929f80f6af30a0570a289c7c9b09399"
        STATUS downloadStatus
    )
    if(NOT "${downloadStatus}" STREQUAL "0;\"No error\"")
        message(FATAL_ERROR "downloadStatus: '${downloadStatus}'")
    endif()
    file(COPY "${PROJECT_SOURCE_DIR}/build/tmp/${PROJECT_NAME_LOWER}-${PROJECT_VERSION_NOTWEAK}.zip" DESTINATION "${CMAKE_DOWNLOADS}")
    message(STATUS "... done")
endif()
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME_LOWER}-${PROJECT_VERSION_NOTWEAK}.zip")
    message(STATUS "copy from downloads ...")
    file(COPY "${CMAKE_DOWNLOADS}/${PROJECT_NAME_LOWER}-${PROJECT_VERSION_NOTWEAK}.zip" DESTINATION "${PROJECT_SOURCE_DIR}/build")
    message(STATUS "... done")
endif()
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME_LOWER}-${PROJECT_VERSION_NOTWEAK}")
    message(STATUS "extract ...")
    file(ARCHIVE_EXTRACT INPUT "${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME_LOWER}-${PROJECT_VERSION_NOTWEAK}.zip" DESTINATION "${PROJECT_SOURCE_DIR}/build")
    message(STATUS "... done")
endif()
set(BASE_DIR "${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME_LOWER}-${PROJECT_VERSION_NOTWEAK}")
cmake_path(RELATIVE_PATH "BASE_DIR" BASE_DIRECTORY "${PROJECT_SOURCE_DIR}" OUTPUT_VARIABLE "BASE_DIR")

message(STATUS "BASE_DIR: '${BASE_DIR}'")

set(TARGET_CMAKE_INSTALL_DEPENDS_ON "${PROJECT_NAME}")

set(POSIX_PLATFORM_SRC
    "${BASE_DIR}/${PROJECT_NAME}/os/events_posix.h"
    "${BASE_DIR}/${PROJECT_NAME}/os/events_posix.c"
    "${BASE_DIR}/${PROJECT_NAME}/os/threads_posix.h"
    "${BASE_DIR}/${PROJECT_NAME}/os/threads_posix.c"
)

set(WINDOWS_PLATFORM_SRC
    "${BASE_DIR}/${PROJECT_NAME}/os/events_windows.h"
    "${BASE_DIR}/${PROJECT_NAME}/os/events_windows.c"
    "${BASE_DIR}/${PROJECT_NAME}/os/threads_windows.h"
    "${BASE_DIR}/${PROJECT_NAME}/os/threads_windows.c"
)

set(OS_DARWIN_SRC
    "${BASE_DIR}/${PROJECT_NAME}/os/darwin_usb.h"
    "${BASE_DIR}/${PROJECT_NAME}/os/darwin_usb.c"
)

set(OS_HAIKU_SRC
    "${BASE_DIR}/${PROJECT_NAME}/os/haiku_usb.h"
    "${BASE_DIR}/${PROJECT_NAME}/os/haiku_usb_backend.cpp"
    "${BASE_DIR}/${PROJECT_NAME}/os/haiku_pollfs.cpp"
    "${BASE_DIR}/${PROJECT_NAME}/os/haiku_usb_raw.h"
    "${BASE_DIR}/${PROJECT_NAME}/os/haiku_usb_raw.cpp"
)

set(OS_LINUX_SRC
    "${BASE_DIR}/${PROJECT_NAME}/os/linux_usbfs.h"
    "${BASE_DIR}/${PROJECT_NAME}/os/linux_usbfs.c"
)

set(OS_EMSCRIPTEN_SRC
    "${BASE_DIR}/${PROJECT_NAME}/os/emscripten_webusb.cpp"
)

set(OS_NETBSD_SRC
    "${BASE_DIR}/${PROJECT_NAME}/os/netbsd_usb.c"
)

set(OS_NULL_SRC
    "${BASE_DIR}/${PROJECT_NAME}/os/null_usb.c"
)

set(OS_OPENBSD_SRC
    "${BASE_DIR}/${PROJECT_NAME}/os/openbsd_usb.c"
)

set(OS_SUNOS_SRC
    "${BASE_DIR}/${PROJECT_NAME}/os/sunos_usb.h"
    "${BASE_DIR}/${PROJECT_NAME}/os/sunos_usb.c"
)

set(OS_WINDOWS_SRC
    #"${BASE_DIR}/${PROJECT_NAME}/libusb-1.0.def"
    #"${BASE_DIR}/${PROJECT_NAME}/libusb-1.0.rc"
    "${BASE_DIR}/${PROJECT_NAME}/os/windows_common.h"
    "${BASE_DIR}/${PROJECT_NAME}/os/windows_common.c"
    "${BASE_DIR}/${PROJECT_NAME}/os/windows_usbdk.h"
    "${BASE_DIR}/${PROJECT_NAME}/os/windows_usbdk.c"
    "${BASE_DIR}/${PROJECT_NAME}/os/windows_winusb.h"
    "${BASE_DIR}/${PROJECT_NAME}/os/windows_winusb.c"
)

if("windows" IN_LIST "PRESET_NAME_ELEMENTS")
    add_library("${PROJECT_NAME}"
        "${BASE_DIR}/${PROJECT_NAME}/libusbi.h"
        "${BASE_DIR}/${PROJECT_NAME}/version.h"
        "${BASE_DIR}/${PROJECT_NAME}/version_nano.h"
        "${BASE_DIR}/${PROJECT_NAME}/core.c"
        "${BASE_DIR}/${PROJECT_NAME}/descriptor.c"
        "${BASE_DIR}/${PROJECT_NAME}/hotplug.c"
        "${BASE_DIR}/${PROJECT_NAME}/io.c"
        "${BASE_DIR}/${PROJECT_NAME}/strerror.c"
        "${BASE_DIR}/${PROJECT_NAME}/sync.c"
        "${BASE_DIR}/msvc/config.h"
        "${BASE_DIR}/msvc/getopt/getopt.h"
        "${BASE_DIR}/msvc/getopt/getopt.c"
        "${BASE_DIR}/msvc/getopt/getopt1.c"
        ${WINDOWS_PLATFORM_SRC}
        ${OS_WINDOWS_SRC}
    )
    #[[generate_export_header("${PROJECT_NAME}"
        BASE_NAME "${PROJECT_NAME_UPPER}"
        EXPORT_FILE_NAME "generated/${PROJECT_NAME}/export.hpp"
    )]]
    target_include_directories("${PROJECT_NAME}" PUBLIC
        #"$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/generated/${PROJECT_NAME}>"
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${BASE_DIR}/${PROJECT_NAME}>"
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${BASE_DIR}/${PROJECT_NAME}/os>"
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${BASE_DIR}/msvc>"
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${BASE_DIR}/msvc/getopt>"
        "$<INSTALL_INTERFACE:include>"
    )
    target_compile_definitions("${PROJECT_NAME}" PUBLIC
        "HAVE_STRING_H"
    )
else()
    message(FATAL_ERROR "Unsupported PRESET_NAME: '${PRESET_NAME}'")
endif()
set_target_properties("${PROJECT_NAME}" PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/main/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/main/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/main/bin"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/main/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/main/bin"

    ARCHIVE_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/main/lib"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/main/lib"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/main/lib"
    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/main/lib"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/main/lib"

    LIBRARY_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/main/lib"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/main/lib"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/main/lib"
    LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/main/lib"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/main/lib"
)
#set_property(TARGET "${PROJECT_NAME}" PROPERTY "VERSION" "${PROJECT_VERSION}")
#set_property(TARGET "${PROJECT_NAME}" PROPERTY "SOVERSION" "${PROJECT_VERSION}")
set_property(TARGET "${PROJECT_NAME}" PROPERTY "INTERFACE_${PROJECT_NAME}_MAJOR_VERSION" "${PROJECT_VERSION}")
set_property(TARGET "${PROJECT_NAME}" APPEND PROPERTY "COMPATIBLE_INTERFACE_STRING" "${PROJECT_NAME}_MAJOR_VERSION")
install(
    EXPORT "${PROJECT_NAME}-targets"
    DESTINATION "cmake"
    NAMESPACE "${PROJECT_NAME}::"
    FILE "${PROJECT_NAME}-targets.cmake"
)
include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/package-config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    INSTALL_DESTINATION "cmake"
)
write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY "ExactVersion"
)
install(
    FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    DESTINATION "cmake"
)
install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/${BASE_DIR}/${PROJECT_NAME}/"
              #"${PROJECT_SOURCE_DIR}/${BASE_DIR}/${PROJECT_NAME}/os/"
              #"${PROJECT_SOURCE_DIR}/${BASE_DIR}/msvc/"
              #"${PROJECT_SOURCE_DIR}/${BASE_DIR}/msvc/getopt/"
    DESTINATION "include"
    FILES_MATCHING
    #PATTERN "*.h"
    #PATTERN "*.hpp"
    PATTERN "libusb.h"
)
install(
    TARGETS "${PROJECT_NAME}"
    EXPORT "${PROJECT_NAME}-targets"
    INCLUDES DESTINATION "include"
    RUNTIME DESTINATION "bin"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
)

if(NOT "${SKIP_TEST}")
    set(TARGET_CMAKE_INSTALL_DEPENDS_ON "cmake-test")
    if("${TEST_GROUP}" STREQUAL "all" AND "${TEST_CASE}" STREQUAL "all")
        set(TEST_REGEXP "^.+\\..+$")
    elseif(NOT "${TEST_GROUP}" STREQUAL "all" AND "${TEST_CASE}" STREQUAL "all")
        set(TEST_REGEXP "^${TEST_GROUP}\\..+$")
    elseif("${TEST_GROUP}" STREQUAL "all" AND NOT "${TEST_CASE}" STREQUAL "all")
        set(TEST_REGEXP "^.+\\.${TEST_CASE}$")
    elseif(NOT "${TEST_GROUP}" STREQUAL "all" AND NOT "${TEST_CASE}" STREQUAL "all")
        set(TEST_REGEXP "^${TEST_GROUP}\\.${TEST_CASE}$")
    endif()

    message(STATUS "TEST_GROUP: '${TEST_GROUP}'")
    message(STATUS "TEST_CASE: '${TEST_CASE}'")
    message(STATUS "TEST_REGEXP: '${TEST_REGEXP}'")

    add_library("test-lib" STATIC
        "${BASE_DIR}/tests/libusb_testlib.h"
        "${BASE_DIR}/tests/testlib.c"
    )
    target_include_directories("test-lib" PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${BASE_DIR}/tests>"
        "$<INSTALL_INTERFACE:include>"
    )
    target_link_libraries("test-lib" PUBLIC
        "${PROJECT_NAME}"
    )
    set_target_properties("test-lib" PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/test/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/test/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/test/bin"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/test/bin"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/test/bin"

        ARCHIVE_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/test/lib"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/test/lib"
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/test/lib"
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/test/lib"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/test/lib"

        LIBRARY_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/test/lib"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/test/lib"
        LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/test/lib"
        LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/test/lib"
        LIBRARY_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/test/lib"
    )

    add_executable("test-app-stress"
        "${BASE_DIR}/tests/stress.c"
    )
    target_link_libraries("test-app-stress"
        "test-lib"
    )
    set_target_properties("test-app-stress" PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/test/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/test/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/test/bin"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/test/bin"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/test/bin"

        ARCHIVE_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/test/lib"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/test/lib"
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/test/lib"
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/test/lib"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/test/lib"

        LIBRARY_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/test/lib"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/test/lib"
        LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/test/lib"
        LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/test/lib"
        LIBRARY_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/test/lib"
    )
    if("${BUILD_SHARED_LIBS}")
        if("windows" IN_LIST "PRESET_NAME_ELEMENTS")
            add_custom_command(TARGET "test-app-stress"
                PRE_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy_directory "$<TARGET_PROPERTY:${PROJECT_NAME},RUNTIME_OUTPUT_DIRECTORY>" "$<TARGET_PROPERTY:test-app-stress,RUNTIME_OUTPUT_DIRECTORY>"
                WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
                USES_TERMINAL
                VERBATIM
            )
        else()
            add_custom_command(TARGET "test-app-stress"
                PRE_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy_directory "$<TARGET_PROPERTY:${PROJECT_NAME},LIBRARY_OUTPUT_DIRECTORY>" "$<TARGET_PROPERTY:test-app-stress,RUNTIME_OUTPUT_DIRECTORY>"
                WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
                USES_TERMINAL
                VERBATIM
            )
        endif()
    endif()

    add_test(
        NAME "test.stress"
        COMMAND "test-app-stress"
        WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/test/bin"
    )

    add_custom_command(
        OUTPUT "${PROJECT_BINARY_DIR}/junit.xml"
        COMMAND "${CMAKE_CTEST_COMMAND}" "--build-nocmake" "--output-junit" "${PROJECT_BINARY_DIR}/junit.xml" "--preset" "${PRESET_NAME}" "-R" "${TEST_REGEXP}"
        DEPENDS "test-app-stress"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        USES_TERMINAL
        VERBATIM
    )

    add_custom_target("cmake-test"
        DEPENDS "${PROJECT_BINARY_DIR}/junit.xml"
    )
endif()

add_custom_command(
    OUTPUT "${CMAKE_INSTALL_PREFIX}/cmake/${PROJECT_NAME}-config.cmake"
    COMMAND "${CMAKE_COMMAND}" --install "${PROJECT_BINARY_DIR}" --prefix "${CMAKE_INSTALL_PREFIX}"
    DEPENDS ${TARGET_CMAKE_INSTALL_DEPENDS_ON}
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    USES_TERMINAL
    VERBATIM
)

add_custom_target("cmake-install" ALL
    DEPENDS "${CMAKE_INSTALL_PREFIX}/cmake/${PROJECT_NAME}-config.cmake"
)

if("${USE_CONAN}")
    add_custom_target("conan-export"
        COMMAND "${CONAN_COMMAND}" "export-pkg"
                "--output-folder" "${CMAKE_INSTALL_PREFIX}"
                ${CONAN_SETTINGS}
                ${CONAN_OPTIONS}
                #"--user" "gitlab-group+gitlab-sub-group+${PROJECT_NAME}"
                #"--channel" "stable"
                "${PROJECT_SOURCE_DIR}/conanfile.py"
        DEPENDS "cmake-install"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        USES_TERMINAL
        VERBATIM
    )

    add_custom_target("conan-remove"
        COMMAND "${CONAN_COMMAND}" "remove" "-c" "${PROJECT_NAME}/${PROJECT_VERSION}"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        USES_TERMINAL
        VERBATIM
    )

    add_custom_target("conan-list"
        COMMAND "${CONAN_COMMAND}" "list" "${PROJECT_NAME}/${PROJECT_VERSION}:*"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        USES_TERMINAL
        VERBATIM
    )
endif()

set_target_names(ALL_TARGETS "${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "all-targets:")
foreach(t IN LISTS ALL_TARGETS)
    message(STATUS "${t}")
endforeach()

set("${PROJECT_NAME}_ADDITIONAL_CLEAN_FILES"
    "${CMAKE_INSTALL_PREFIX}"
)
set_directory_properties(PROPERTIES
    ADDITIONAL_CLEAN_FILES "${${PROJECT_NAME}_ADDITIONAL_CLEAN_FILES}"
)
