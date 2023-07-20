set(Boost_USE_STATIC_LIBS ON)
set(OPENSSL_USE_STATIC_LIBS ON)

find_package(Threads REQUIRED)
find_package(Boost 1.74.0 EXACT REQUIRED COMPONENTS log program_options)
find_package(OpenSSL REQUIRED)

add_library(webserver STATIC Application.cpp
                             ApplicationConfig.cpp
                             exceptions/HttpErrorExceptionHandler.cpp
                             HttpRequestHolder.cpp
                             HttpServer.cpp
                             HttpsConnection.cpp
                             HttpsServer.cpp
                             MediaTypeResolver.cpp
                             ProgramOptions.cpp
                             RequestDispatcher.cpp
                             StaticResourceReader.cpp
                             StaticResouceController.cpp
                             UrlUtil.cpp)

target_include_directories(webserver PUBLIC ${Boost_INCLUDE_DIR})
target_link_libraries(webserver Boost::log Boost::program_options OpenSSL::Crypto OpenSSL::SSL ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
