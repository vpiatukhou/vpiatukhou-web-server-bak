#pragma once

#include <stdexcept>
#include <string>

namespace WebServer {

    using ErrorCode = int;

    /**
     * The base class for all user-defined exceptions which are thrown from HttpController.
     */
    class HttpControllerException : public std::runtime_error {
    public:
        HttpControllerException(const std::string& message) : std::runtime_error(message) {
        }

        virtual ~HttpControllerException() {
        }

        /**
         * @return the code which is used to map the exception to HttpControllerExceptionHandler.
         */
        virtual ErrorCode getErrorCode() {
            return 0;
        }
    };

}
