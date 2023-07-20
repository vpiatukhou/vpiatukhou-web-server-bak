#pragma once

#include "HttpConnection.h"

#include <boost/asio/ssl.hpp>

namespace WebServer {

    /**
     * An HTTPS connection.
     * 
     * @see HttpConnection
     */
    class HttpsConnection : public HttpConnection<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> {
    public:
        HttpsConnection(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>&& socket_,
                        ApplicationConfigPtr config_,
                        RequestDispatcherPtr requestDispatcher_);

        void doHandshake();
    };

}
