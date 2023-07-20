cmake_minimum_required(VERSION 3.4.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

option(BUILD_TESTS "If 'ON', the unit tests will be build." OFF)
option(BUILD_INTEGRATION_TESTS "If 'ON', the integration tests will be build." OFF)
option(BUILD_DEMO "If 'ON', the demo application will be build." OFF)

project(web_server VERSION ${PROJECT_VERSION})

add_subdirectory(lib)

if(${BUILD_TESTS})
    add_subdirectory(lib_unit_tests)
    enable_testing()
    add_test(NAME all_unit_tests COMMAND webserver_test)
endif()

if(${BUILD_INTEGRATION_TESTS})
    add_subdirectory(lib_integration_tests)
endif()

if(${BUILD_DEMO})
    add_subdirectory(demo)
endif()
