#include "HttpErrorExceptionHandler.h"

#include <boost/beast/http.hpp>
#include <boost/log/trivial.hpp>

#include <filesystem>
#include <utility>

namespace WebServer {

    namespace {
        namespace fs = std::filesystem;
        namespace http = boost::beast::http;

        bool tryToGetErrorPagePath(const ApplicationConfigPtr& config, boost::beast::http::status status, 
            fs::path& pageFilepath) {
            try {
                pageFilepath = config->getErrorPageMapping().at(static_cast<HttpStatus>(status));
            } catch (std::out_of_range& e) {
                return false;
            }
            return true;
        }
    }

    HttpErrorExceptionHandler::HttpErrorExceptionHandler(ApplicationConfigPtr config_,
        MediaTypeResolverPtr mediaTypeResolver_, StaticResourceReaderPtr staticResourceReader_) 
        : config(config_), mediaTypeResolver(mediaTypeResolver_), staticResourceReader(staticResourceReader_) {
    }

    void HttpErrorExceptionHandler::handle(const HttpErrorException& exception, HttpResponse& response) {
        auto httpStatus = exception.getHttpStatus();

        fs::path pageFilepath;
        if (tryToGetErrorPagePath(config, httpStatus, pageFilepath)) {
            auto mediaType = mediaTypeResolver->getMediaTypeByFilename(pageFilepath.filename().string());
            std::string responseBody;
            staticResourceReader->readResourceFromFile(pageFilepath, responseBody);
            response.set(http::field::content_type, std::move(mediaType));
            response.result(httpStatus);
            response.body() = std::move(responseBody);
        } else {
            BOOST_LOG_TRIVIAL(warning) << "There is no an error page for HTTP status '" << httpStatus << "'.";
            response.result(httpStatus);
            response.body() = "";
        }
    }

}
