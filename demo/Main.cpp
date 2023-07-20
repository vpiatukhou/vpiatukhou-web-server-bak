#include "Application.h"
#include "DemoControllers.h"
#include "DemoExceptions.h"
#include "HttpController.h"

#include <boost/log/trivial.hpp>

#include <utility>

namespace WebServer {

    const int INTERNAL_ERROR = -1;
}

int main(int argc, char* argv[]) {
    using namespace WebServer;

    std::vector<HttpControllerMapping> controllerMapping;
    controllerMapping.push_back({"/bar", std::make_shared<BarController>()});
    controllerMapping.push_back({"/foo", std::make_shared<FooController>()});

    std::unordered_map<int, HttpControllerExceptionHandlerPtr> exceptionHandlerById;
    exceptionHandlerById[Error::INVALID_INPUT] = std::make_shared<InvalidInputExceptionHandler>();
    exceptionHandlerById[Error::INVALID_STATE] = std::make_shared<InvalidStateExceptionHandler>();

    try {
        Application application;
        application.start(argc, argv, std::move(controllerMapping), std::move(exceptionHandlerById));
    } catch (const std::exception& e) {
        BOOST_LOG_TRIVIAL(fatal) << "An internal error occurred. The application wasn't start: " << e.what();
        return INTERNAL_ERROR;
    }

    return 0;
}
