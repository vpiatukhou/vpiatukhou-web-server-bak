#pragma once

#include "RequestDispatcher.h"
#include "HttpRequestHolder.h"

#include <boost/asio.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/log/trivial.hpp>

#include <memory>
#include <utility>

namespace WebServer {

    /**
     * An HTTP connection. Receives HTTP requests and writes the responses.
     * The instance of this class is created when a client sets up a new connection 
     * and destroyed after the connection is closed.
     */
    template<class Socket>
    class HttpConnection : public std::enable_shared_from_this<HttpConnection<Socket>> {
    public:
        HttpConnection(Socket& socket_, ApplicationConfigPtr config_, RequestDispatcherPtr requestDispatcher_)
                       : socket(std::move(socket_)), config(config_), requestDispatcher(requestDispatcher_) {
        }

        /**
         * Reads the current HTTP request asynchronously. Calls RequestDispatcher. Writes the response.
         * 
         * The method doesn't block the current thread.
         */
        void listen() {
            namespace http = boost::beast::http;

            //pass "self" to the callback in order to keep the instance alive while the connection exists
            auto self = this->shared_from_this();
            auto requestHandler = [this, self](boost::system::error_code const& readError, std::size_t) {
                if (readError) {
                    BOOST_LOG_TRIVIAL(error) << "Error reading the request. The error: " 
                        << readError << ". The message: " << readError.message();
                } else {
                    BOOST_LOG_TRIVIAL(trace) << "The request was received.";

                    requestHolder.init();

                    response.version(HTTP_VERSION);
                    response.result(http::status::ok);
                    response.set(http::field::server, config->getServerName());

                    requestDispatcher->dispatch(requestHolder, response);

                    response.prepare_payload();

                    sendResponse();
                }
            };

            http::async_read(socket, buffer, requestHolder.getRequest(), requestHandler);
        }

    protected:
        Socket socket;

    private:
        const unsigned int HTTP_VERSION = 11;

        boost::beast::flat_buffer buffer;
        HttpRequestHolder requestHolder;
        HttpResponse response;

        ApplicationConfigPtr config;
        RequestDispatcherPtr requestDispatcher;

        void sendResponse() {
            //pass "self" to the callback in order to keep the instance alive while the connection exists
            auto self = this->shared_from_this();
            auto writeHandler = [this, self](boost::system::error_code const& writeError, std::size_t) {
                if (writeError) {
                    BOOST_LOG_TRIVIAL(error) << "Error sending response. The error: "
                        << writeError << ". The message: " << writeError.message();
                } else {
                    BOOST_LOG_TRIVIAL(trace) << "The response was sent.";
                }
            };

            boost::beast::http::async_write(socket, response, writeHandler);
        }
    };

}
