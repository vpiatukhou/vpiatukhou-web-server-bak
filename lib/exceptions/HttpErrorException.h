#pragma once

#include "../HttpRequestResponse.h"

#include <boost/beast/http.hpp>

#include <stdexcept>

namespace WebServer {

    /**
     * Thrown if the server must return a configured error page for the given HTTP status.
     */
    class HttpErrorException : public std::runtime_error {
    public:
        HttpErrorException(boost::beast::http::status httpStatus_) : std::runtime_error(""), httpStatus(httpStatus_) {
        }

        boost::beast::http::status getHttpStatus() const {
            return httpStatus;
        }
    private:
        boost::beast::http::status httpStatus;
    };

}
