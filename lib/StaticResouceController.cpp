#include "StaticResouceController.h"
#include "exceptions/HttpErrorException.h"
#include "MediaType.h"
#include "UrlUtil.h"

#include <boost/log/trivial.hpp>

#include <utility>

namespace WebServer {

    namespace {
        namespace http = boost::beast::http;
    }

    StaticResouceController::StaticResouceController(ApplicationConfigPtr config_,
                                                     MediaTypeResolverPtr mediaTypeResolver_,
                                                     StaticResourceReaderPtr staticResourceReader_)
        : config(config_), mediaTypeResolver(std::move(mediaTypeResolver_)),
          staticResourceReader(staticResourceReader_) {
    }

    void StaticResouceController::processRequest(HttpRequestHolder& requestHolder,  HttpResponse& response) {
        if (requestHolder.getRequest().method() == http::verb::get) {
            processGetRequest(requestHolder.getRequestUri(), response);
        } else {
            BOOST_LOG_TRIVIAL(info) << "The method '" << requestHolder.getRequest().method() 
                << "' is not supported in StaticResouceController.";
            throw HttpErrorException(http::status::method_not_allowed);
        }
    }

    void StaticResouceController::processGetRequest(const std::string& requestUri, HttpResponse& response) const {
        auto filepath = config->getStaticResouceBaseDir();
        filepath += requestUri;
        filepath = filepath.lexically_normal();

        BOOST_LOG_TRIVIAL(trace) << "Getting the resource: " << filepath;

        std::string responseBody;
        if (checkIfPathStartsWithBase(filepath, config->getStaticResouceBaseDir())) {
            if (staticResourceReader->readResourceFromFile(filepath, responseBody)) {
                auto mediaType = mediaTypeResolver->getMediaTypeByFilename(filepath.filename().string());
                response.result(http::status::ok);
                response.set(http::field::content_type, std::move(mediaType));
                response.body() = std::move(responseBody);
            } else {
                BOOST_LOG_TRIVIAL(info) << "The resource " << filepath << " was not found.";
                throw HttpErrorException(http::status::not_found);
            }
        } else {
            BOOST_LOG_TRIVIAL(info) << "Access to " << filepath << " is forbidden.";
            throw HttpErrorException(http::status::forbidden);
        }
    }

}
