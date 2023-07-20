#include "HttpServer.h"
#include "HttpConnection.h"

#include <boost/log/trivial.hpp>

namespace WebServer {

    namespace {
        using namespace boost::asio::ip;
    }

    HttpServer::HttpServer(boost::asio::io_context& ioContext_,
                           Port port_,
                           ApplicationConfigPtr config_,
                           RequestDispatcherPtr requestDispatcher_) :
        acceptor(ioContext_, tcp::endpoint(tcp::v4(), port_)), 
        config(config_), 
        requestDispatcher(requestDispatcher_) {

        BOOST_LOG_TRIVIAL(info) << "The HTTP server is up and running.";

        accept();
    }

    void HttpServer::accept() {
        acceptor.async_accept([this](boost::system::error_code acceptionError, tcp::socket socket) {
            if (acceptionError) {
                BOOST_LOG_TRIVIAL(error) << "Error accepting a connection. The error code: " 
                    << acceptionError << ". The message: " << acceptionError.message();
            } else {
                BOOST_LOG_TRIVIAL(trace) << "A new connection was accepted.";
                //Make a shared pointer in order to allow usage enable_shared_from_this in HttpConnection.
                //The instance of HttpConnection won't be deleted after "accept()" returns control because
                //the pointer to it will be "captured" in HttpConnection::listen().
                std::make_shared<HttpConnection<tcp::socket>>(socket, config, requestDispatcher)->listen();

                //start listening for a next incoming connection
                accept();
            }

            });
    }

}