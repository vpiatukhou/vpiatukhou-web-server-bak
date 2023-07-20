set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

configure_file(Resources.h.in ${CMAKE_CURRENT_BINARY_DIR}/generated_sources/Resources.h @ONLY)

add_executable(webserver_test ApplicationConfigTest.cpp
                              MediaTypeResolverTest.cpp
                              HttpRequestHolderTest.cpp
                              UrlUtilTest.cpp)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

target_include_directories(webserver_test PRIVATE ${CMAKE_SOURCE_DIR}/lib ${CMAKE_CURRENT_BINARY_DIR}/generated_sources)
target_link_libraries(webserver_test webserver GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(webserver_test)

file(COPY "resources" DESTINATION .)
