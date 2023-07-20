set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

file(COPY resources DESTINATION .)
configure_file(Resources.h.in generated_sources/Resources.h @ONLY)
configure_file(resources/config/application.json resources/config/application.json @ONLY)

add_executable(webserver_it HttpServerApplicationTest.cpp)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

target_include_directories(webserver_it PRIVATE ${CMAKE_SOURCE_DIR}/lib ${CMAKE_CURRENT_BINARY_DIR}/generated_sources)
target_link_libraries(webserver_it webserver GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(webserver_it)
