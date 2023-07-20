#pragma once

#include "HttpRequestHolder.h"
#include "HttpRequestResponse.h"

#include <memory>
#include <string>

namespace WebServer {

    /**
     * The base class for all controllers which process HTTP requests.
     */
    class HttpController {
    public:
        virtual ~HttpController() { }

        /**
         * Process the given request and writes the response.
         * 
         * @param request  - the incoming HTTP request
         * @param response - the outgoing HTTP response
         */
        virtual void processRequest(HttpRequestHolder& request, HttpResponse& response) = 0;

    };

    /**
     * Mapping an requestUri to a controller.
     */
    struct HttpControllerMapping {
        std::string requestUri;
        std::shared_ptr<HttpController> controller;
    };

}