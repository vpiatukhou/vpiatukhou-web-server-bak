/**
 * This header contains exceptions and handler which are used to demonstrate how the exception handling mechanism works.
 */
#pragma once

#include "MediaType.h"
#include "exceptions/HttpControllerException.h"
#include "exceptions/HttpControllerExceptionHandler.h"

#include <string>

namespace WebServer {

    enum Error {
        
        INVALID_INPUT = 1,
        INVALID_STATE = 2

    };

    class InvalidInputException : public HttpControllerException {
    public:

        InvalidInputException(const std::string& input_) : input(input_), HttpControllerException("invalid input") {
        }

        ErrorCode getErrorCode() override {
            return Error::INVALID_INPUT;
        }

        const std::string& getInput() const {
            return input;
        }

    private:

        std::string input;

    };

    class InvalidStateException : public HttpControllerException {
    public:

        InvalidStateException(int state_) : state(state_), HttpControllerException("invalid state") {
        }

        ErrorCode getErrorCode() override {
            return Error::INVALID_STATE;
        }

        int getState() const {
            return state;
        }

    private:

        int state;

    };

    class InvalidInputExceptionHandler : public HttpControllerExceptionHandler {

        void handle(const HttpControllerException& exception, HttpResponse& response) override {
            namespace http = boost::beast::http;

            const InvalidInputException& invalidInputException = static_cast<const InvalidInputException&>(exception);

            response.set(http::field::content_type, MEDIA_TYPE_TEXT_PLAIN);
            response.result(http::status::bad_request);
            response.body() = "Invalid input: " + invalidInputException.getInput();
        }

    };

    class InvalidStateExceptionHandler : public HttpControllerExceptionHandler {

        void handle(const HttpControllerException& exception, HttpResponse& response) override {
            namespace http = boost::beast::http;

            const InvalidStateException& invalidStateException = static_cast<const InvalidStateException&>(exception);

            response.set(http::field::content_type, MEDIA_TYPE_TEXT_PLAIN);
            response.result(http::status::bad_request);
            response.body() = "Invalid state: " + std::to_string(invalidStateException.getState());
        }

    };

}
