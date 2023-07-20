#pragma once

#include "DemoExceptions.h"
#include "HttpController.h"
#include "MediaType.h"

namespace WebServer {

    class FooController : public HttpController {

        void processRequest(HttpRequestHolder& requestHolder, HttpResponse& response) override {
            namespace http = boost::beast::http;

            response.set(http::field::content_type, MEDIA_TYPE_TEXT_PLAIN);

            std::string body;

            switch (requestHolder.getRequest().method()) {
                case http::verb::get:
                    body = "Foo - GET. Query parameters:\n";
                    for (auto& keyValue : requestHolder.getQueryParameters()) {
                        body += "name=" + keyValue.first + ", value=" + keyValue.second + "\n";
                    }
                    break;
                case http::verb::post:
                    body = "Foo - POST";
                    throw InvalidInputException("abc");
                case http::verb::put:
                    body = "Foo - PUT";
                    throw InvalidStateException(123);
                default:
                    body = "Foo - method is not allowed";
                    break;
            }

            response.body() = body;
        }

    };

    class BarController : public HttpController {

        void processRequest(HttpRequestHolder& requestHolder, HttpResponse& response) override {
            namespace http = boost::beast::http;

            response.set(http::field::content_type, MEDIA_TYPE_TEXT_PLAIN);

            switch (requestHolder.getRequest().method()) {
                case http::verb::get:
                    response.body() = "Bar - GET";
                    break;
                default:
                    response.body() = "Bar - method is not allowed";
                    break;
            }
        }

    };

}
