cmake_minimum_required(VERSION 3.23)

# TODO: do this only on rpi
set(ENV{VCPKG_FORCE_SYSTEM_BINARIES} ON)

set(USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(USE_UNITY_BUILDS "Enable unity build to improve build times" OFF)
option(TRACE_BUILD_TIME "Use -ftime-trace to generate build time trace" OFF)
option(ENABLE_SANITIZERS "Enable address and undefined behavior sanitizers" OFF)

project(sender)

# Enable ASAN
add_compile_options($<$<OR:$<BOOL:${ENABLE_SANITIZERS}>,$<CONFIG:Debug>>:-fsanitize=address>)
add_link_options($<$<OR:$<BOOL:${ENABLE_SANITIZERS}>,$<CONFIG:Debug>>:-fsanitize=address>)

# undefined behavior sanitizer is not available on MSVC
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options("$<$<OR:$<BOOL:${ENABLE_SANITIZERS}>,$<CONFIG:Debug>>:-fno-omit-frame-pointer;-fsanitize=undefined>")
    add_link_options("$<$<OR:$<BOOL:${ENABLE_SANITIZERS}>,$<CONFIG:Debug>>:-fno-omit-frame-pointer;-fsanitize=undefined>")
endif ()

# Set warning levels
if (MSVC)
    # level 4 as the base
    add_compile_options(/W4)
    # but without some annoying warnings
    add_compile_options(/wd4244 /wd4267)
else ()
    # set the base
    add_compile_options(-Wall -Wextra -Wpedantic)
    # and remove annoying warnings
    add_compile_options(-Wno-error=sign-compare)
endif ()

# And treat them as error
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

if (${TRACE_BUILD_TIME})
    add_compile_options(-ftime-trace)
endif ()

add_compile_definitions(DATA_DIR="${CMAKE_SOURCE_DIR}/data")
add_compile_definitions(LOG_DIR="${CMAKE_SOURCE_DIR}/log")

find_package(immortals-common CONFIG REQUIRED)

option(USE_MOCK_RF24 "Use a mock version of RF24. Useful when developing on platforms other than rpi." OFF)

if (NOT ${USE_MOCK_RF24})
    find_library(RF24 rf24 REQUIRED)
    message(STATUS "using RF24 library: ${RF24}")
endif()

set(HEADER_FILES
        source/pch.h)

set(SOURCE_FILES
        source/main.cpp)

if (${USE_MOCK_RF24})
    # TODO: create and add mock rf24
endif ()

add_executable(${PROJECT_NAME}
        ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(${PROJECT_NAME}
        immortals::common
        ${RF24}
        pthread)

set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD ${USE_UNITY_BUILDS})

target_precompile_headers(${PROJECT_NAME} PRIVATE source/pch.h)

install(TARGETS ${PROJECT_NAME})

set(CPACK_GENERATOR ZIP)
set(CPACK_OUTPUT_FILE_PREFIX ${CMAKE_BINARY_DIR}/dist)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
include(CPack)
