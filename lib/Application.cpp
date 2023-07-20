#include "Application.h"
#include "ApplicationConfig.h"
#include "HttpServer.h"
#include "HttpsServer.h"
#include "MediaTypeResolver.h"
#include "ProgramOptions.h"
#include "RequestDispatcher.h"
#include "StaticResourceReader.h"

#include <memory>
#include <vector>
#include <utility>

namespace WebServer {

    class Application::Impl {
    public:
        void start(int argc, char* argv[], std::vector<HttpControllerMapping>&& controllerMapping, 
            std::unordered_map<int, HttpControllerExceptionHandlerPtr>&& exceptionHandlerById) {

            ProgramOptions options;
            options.parse(argc, argv);

            StaticResourceReaderPtr staticResourceReader = std::make_shared<StaticResourceReader>();
            ApplicationConfigPtr config = std::make_shared<ApplicationConfig>(options.getConfigFilepath());
            MediaTypeResolverPtr mediaTypeResolver = std::make_shared<MediaTypeResolver>(
                config->getMediaTypeMapping());
            StaticResouceControllerPtr staticResouceController = std::make_shared<StaticResouceController>(
                config, mediaTypeResolver, staticResourceReader);
            HttpErrorExceptionHandlerPtr httpErrorExceptionHandler = std::make_shared<HttpErrorExceptionHandler>(
                config, mediaTypeResolver, staticResourceReader);
            RequestDispatcherPtr requestDispatcher = std::make_shared<RequestDispatcher>(staticResouceController,
                std::move(controllerMapping), httpErrorExceptionHandler, std::move(exceptionHandlerById));

            if (config->isSslEnabled()) {
                HttpsServer server(ioContext, config->getServerPort(), config, requestDispatcher);
                ioContext.run();
            } else {
                HttpServer server(ioContext, config->getServerPort(), config, requestDispatcher);
                ioContext.run();
            }
        }

        void stop() {
            ioContext.stop();
        }

    private:
        boost::asio::io_context ioContext;
    };

    Application::Application() : impl(std::make_unique<Impl>()) {
    }

    Application::~Application() {
    }

    void Application::start(int argc, char* argv[], std::vector<HttpControllerMapping>&& controllerMapping,
        std::unordered_map<int, HttpControllerExceptionHandlerPtr>&& exceptionHandlerById) {

        impl->start(argc, argv, std::move(controllerMapping), std::move(exceptionHandlerById));
    }

    void Application::stop() {
        impl->stop();
    }

}
