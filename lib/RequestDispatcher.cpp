#include "RequestDispatcher.h"
#include "MediaType.h"
#include "UrlUtil.h"

#include <boost/log/trivial.hpp>

#include <string>
#include <utility>

namespace WebServer {

    namespace {
        namespace http = boost::beast::http;

        const std::string INTERNAL_SERVER_ERROR_RESPONSE = "500 Internal Server Error";

        void setUpInternalServerError(HttpResponse& response) {
            response.result(http::status::internal_server_error);
            response.set(http::field::content_type, MEDIA_TYPE_TEXT_PLAIN);
            response.body() = INTERNAL_SERVER_ERROR_RESPONSE;
        }
    }

    RequestDispatcher::RequestDispatcher(StaticResouceControllerPtr staticResouceController_,
        std::vector<HttpControllerMapping>&& controllerMapping_,
        HttpErrorExceptionHandlerPtr httpErrorExceptionHandler_,
        std::unordered_map<int, HttpControllerExceptionHandlerPtr>&& exceptionHandlerById_)
            : staticResouceController(staticResouceController_),
              controllerMapping(std::move(controllerMapping_)),
              httpErrorExceptionHandler(httpErrorExceptionHandler_),
              exceptionHandlerById(std::move(exceptionHandlerById_)) {
    }

    void RequestDispatcher::dispatch(HttpRequestHolder& request, HttpResponse& response) {
        try {
            auto requestUri = request.getRequestUri();
            for (auto& mapping : controllerMapping) {
                if (mapping.requestUri == requestUri) {
                    mapping.controller->processRequest(request, response);
                    return;
                }
            }
            staticResouceController->processRequest(request, response);
        } catch (HttpErrorException& e) {
            httpErrorExceptionHandler->handle(e, response);
        } catch (HttpControllerException& e) {
            try {
                exceptionHandlerById.at(e.getErrorCode())->handle(e, response);
            } catch (std::out_of_range&) {
                BOOST_LOG_TRIVIAL(warning) << "There is no exception handler for the Id '" << e.getErrorCode() << "'.";
                setUpInternalServerError(response);
            }
        } catch (std::exception& e) {
            BOOST_LOG_TRIVIAL(error) << "Unexpected error occurred: " << e.what();
            setUpInternalServerError(response);
        }
    }

}
