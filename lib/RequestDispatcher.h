#pragma once

#include "HttpController.h"
#include "StaticResouceController.h"
#include "exceptions/HttpControllerExceptionHandler.h"
#include "exceptions/HttpErrorExceptionHandler.h"

#include <memory>
#include <vector>
#include <unordered_map>

namespace WebServer {

    /**
     * Dispatches the given request to the appropriate controller, based on HTTP 'target'.
     *
     * This is an entry point for all incoming HTTP requests.
     */
    class RequestDispatcher {
    public:
        RequestDispatcher(StaticResouceControllerPtr staticResouceController_, 
            std::vector<HttpControllerMapping>&& controllerMapping_,
            HttpErrorExceptionHandlerPtr httpErrorExceptionHandler_,
            std::unordered_map<int, HttpControllerExceptionHandlerPtr>&& exceptionHandlerById_);

        void dispatch(HttpRequestHolder& requestHolder, HttpResponse& response);

    private:
        StaticResouceControllerPtr staticResouceController;

        std::vector<HttpControllerMapping> controllerMapping;

        HttpErrorExceptionHandlerPtr httpErrorExceptionHandler;
        std::unordered_map<int, HttpControllerExceptionHandlerPtr> exceptionHandlerById;

    };

    using RequestDispatcherPtr = std::shared_ptr<RequestDispatcher>;
}
