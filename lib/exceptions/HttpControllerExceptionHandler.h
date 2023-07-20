#pragma once

#include "HttpControllerException.h"
#include "../HttpRequestResponse.h"

#include <memory>

namespace WebServer {

    /**
     * The base class for all user-defined exception handlers.
     */
    class HttpControllerExceptionHandler {
    public:

        virtual ~HttpControllerExceptionHandler() {
        }

        virtual void handle(const HttpControllerException& exception, HttpResponse& response) = 0;

    };

    using HttpControllerExceptionHandlerPtr = std::shared_ptr<HttpControllerExceptionHandler>;
}
