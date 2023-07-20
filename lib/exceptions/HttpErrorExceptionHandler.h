#pragma once

#include "HttpErrorException.h"
#include "../ApplicationConfig.h"
#include "../HttpRequestHolder.h"
#include "../MediaTypeResolver.h"
#include "../StaticResourceReader.h"

#include <memory>

namespace WebServer {

    class HttpErrorExceptionHandler  {
    public:
        HttpErrorExceptionHandler(ApplicationConfigPtr config_, MediaTypeResolverPtr mediaTypeResolver_,
            StaticResourceReaderPtr staticResourceReader_);

        void handle(const HttpErrorException& exception, HttpResponse& response);

    private:
        ApplicationConfigPtr config;
        MediaTypeResolverPtr mediaTypeResolver;
        StaticResourceReaderPtr staticResourceReader;
    };

    using HttpErrorExceptionHandlerPtr = std::shared_ptr<HttpErrorExceptionHandler>;

}
