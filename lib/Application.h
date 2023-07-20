#pragma once

#include "HttpController.h"
#include "exceptions/HttpControllerExceptionHandler.h"

#include <memory>
#include <vector>
#include <unordered_map>

namespace WebServer {

    /**
     * A class is used to set up and launch the server application.
     * 
     * Normally it is used in main() method like this:
     * 
     *   Application application;
     *   application.exec(argc, argv);
     */
    class Application {
    public:
        Application();
        Application(const Application&) = delete;
        Application(Application&&) = default;
        ~Application();

        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&) = delete;

        /**
         * Starts the server. This method doesn't return control until "stop()" is invoked.
         * 
         * @param argc              - a number of arguments. It is the first argument of the function 'int main(argc, argv)'.
         * @param argv              - the program arguments. It is the second argument of the function 'int main(argc, argv)'.
         * @param controllerMapping - (optional) user-defined controllers. This vector will be MOVED inside of this method.
         * @see stop()
         */
        void start(int argc, char* argv[], std::vector<HttpControllerMapping>&& controllerMapping = std::vector<HttpControllerMapping>(),
            std::unordered_map<int, HttpControllerExceptionHandlerPtr>&& exceptionHandlerById = std::unordered_map<int, HttpControllerExceptionHandlerPtr>());

        /**
         * Stops the server. The method is thread-safe.
         */
        void stop();

    private:
        class Impl;
        std::unique_ptr<Impl> impl;
    };

}
