#pragma once

#include "ApplicationConfig.h"
#include "HttpController.h"
#include "MediaTypeResolver.h"
#include "StaticResourceReader.h"

namespace WebServer {

    /**
     * Handles HTTP GET requests and returns static resources like HTML, javascript, images etc.
     */
    class StaticResouceController : public HttpController {
    public:
        StaticResouceController(ApplicationConfigPtr config_, MediaTypeResolverPtr mediaTypeResolver_,
            StaticResourceReaderPtr staticResourceReader_);

        /**
         * Returns the resource (HTML, JS, CSS etc.) which is specified in 'target' of the given HTTP request.
         * The resource is written to the response.
         * 
         * @param request   - HTTP request
         * @param response  - HTTP response which contains the requested resource
         * @throws HttpErrorException if:
         *         - the resource could not be not found;
         *         - the HTTP method is not GET;
         *         - the request target points to the directory outside of the base one
         *           (please see ApplicationConfig::getStaticResouceBaseDir())
         */
        void processRequest(HttpRequestHolder& requestHolder, HttpResponse& response) override;

    private:
        ApplicationConfigPtr config;
        MediaTypeResolverPtr mediaTypeResolver;
        StaticResourceReaderPtr staticResourceReader;

        void processGetRequest(const std::string& requestUri, HttpResponse& response) const;
    };

    using StaticResouceControllerPtr = std::shared_ptr<StaticResouceController>;
}
