#include "Application.h"
#include "HttpRequestResponse.h"
#include "MediaType.h"
#include "Resources.h"

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include "gtest/gtest.h"

#include <chrono>
#include <fstream>
#include <string>
#include <thread>

namespace {
    using namespace WebServer;
    using namespace std::chrono_literals;
    using namespace std::string_literals;
    namespace http = boost::beast::http;
    namespace beast = boost::beast;
    using tcp = boost::asio::ip::tcp;

    const std::string TEST_DATA_DIR = BINARY_DIR + "/resources/"s;
    const auto BASE_DIR = TEST_DATA_DIR + "web_content/base_dir/"s;

    const auto APPLICATION_CONFIG_ARG = "-c" + TEST_DATA_DIR + "config/application.json"s;
    const auto REQUEST_TARGET = "/foo.html"s;
    const auto NOT_EXISTING_TARGET = "/notExisting.html"s;
    const auto NOT_FOUND_PAGE = "notFound.html"s;
    const auto WAIT_FOR_SERVER_START = 1s; 

    const auto SERVER_HOST = "localhost"s;
    const auto SERVER_PORT = "8080"s;
    const int HTTP_VERSION = 11;
    const char END_OF_STRING = '\0';

    /**
     * Starts the server before and stops it after the test.
     */
    class HttpServerApplicationTest : public ::testing::Test {
    public:
        HttpServerApplicationTest() : serverThread([this](){ application.start(argv.size(), argv.data()); }) {
        }

        ~HttpServerApplicationTest() {
            application.stop();
            serverThread.join();
        }

    private:
        std::string configArg = APPLICATION_CONFIG_ARG;
        std::vector<char*> argv = { nullptr, configArg.data() };

        Application application;
        std::thread serverThread;
    };

    HttpRequest createRequest(const std::string& targetRequest) {
        HttpRequest request{http::verb::get, targetRequest, HTTP_VERSION};
        request.set(http::field::host, SERVER_HOST);
        request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        return request;
    }

    /**
     * Creates a client, sends HTTP request and receives the response from the server.
     */
    void sendRequest(const HttpRequest& request, HttpResponse& response) {
        boost::asio::io_context ioContext;

        tcp::resolver resolver(ioContext);
        beast::tcp_stream stream(ioContext);

        tcp::resolver::query query(SERVER_HOST, SERVER_PORT);
        auto endpoints = resolver.resolve(query);
        stream.connect(endpoints);

        //send request
        http::write(stream, request);

        //receive response
        beast::flat_buffer buffer;
        http::read(stream, buffer, response);

        stream.socket().shutdown(tcp::socket::shutdown_both);
    }

    std::string getTestResourceContent(const std::string& filepath) {
        std::string resource;
        std::ifstream file(filepath, std::ios::binary | std::ios::ate);
        auto fileSize = file.tellg();
        resource.resize(fileSize, END_OF_STRING);
        file.seekg(0);
        file.read(resource.data(), fileSize);
        return resource;
    }
}

TEST_F(HttpServerApplicationTest, positive) {
    auto request = createRequest(REQUEST_TARGET);

    //sleep for one sec to make sure that the server is up and running
    std::this_thread::sleep_for(WAIT_FOR_SERVER_START);

    //when
    HttpResponse response;
    sendRequest(request, response);

    //then
    auto expected = getTestResourceContent(BASE_DIR + REQUEST_TARGET);
    ASSERT_EQ(http::status::ok, response.result());
    ASSERT_EQ(expected, response.body());
}

TEST_F(HttpServerApplicationTest, errorPage) {
    auto request = createRequest(NOT_EXISTING_TARGET);

    //sleep for one sec to make sure that the server is up and running
    std::this_thread::sleep_for(WAIT_FOR_SERVER_START);

    HttpResponse response;
    sendRequest(request, response);

    //then
    auto expected = getTestResourceContent(BASE_DIR + NOT_FOUND_PAGE);
    ASSERT_EQ(http::status::not_found, response.result());
    ASSERT_EQ(expected, response.body());
}
