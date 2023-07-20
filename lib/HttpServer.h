#pragma once

#include "Port.h"
#include "RequestDispatcher.h"

#include <boost/asio.hpp>

namespace WebServer {

    /**
     * An asynchronous HTTP server.
     */
    class HttpServer {
    public:
        HttpServer(boost::asio::io_context& ioContext_, 
                   Port port_,
                   ApplicationConfigPtr config_,
                   RequestDispatcherPtr requestDispatcher_);

    private:
        boost::asio::ip::tcp::acceptor acceptor;

        ApplicationConfigPtr config;
        RequestDispatcherPtr requestDispatcher;

        void accept();
    };
}
