#pragma once

#include "Port.h"
#include "ApplicationConfig.h"
#include "RequestDispatcher.h"

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

namespace WebServer {

    /**
     * An asynchronous HTTPS server.
     */
    class HttpsServer {
    public:
        HttpsServer(boost::asio::io_context& ioContext_,
                    Port port_,
                    ApplicationConfigPtr config_,
                    RequestDispatcherPtr requestDispatcher_);

    private:
        boost::asio::ssl::context sslContext;
        boost::asio::ip::tcp::acceptor acceptor;

        ApplicationConfigPtr config;
        RequestDispatcherPtr requestDispatcher;

        void accept();
    };
}
