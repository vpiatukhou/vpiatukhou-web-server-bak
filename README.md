# web-server

## Overview

Web Server is a simple library which provides functionality of a HTTP server.

Features:
- asynchronous HTTP server;
- HTTPS support;
- the server can return static content (HTML, CSS etc.);
- possibility to register custom Controllers to manage dynamic content;
- simple API and configuration.

## Getting Started

1. Compile the library from sources and add it to your project.

2. Add the following code to the function main():

```
#include "Application.h"

int main(int argc, char* argv[]) {
    using namespace WebServer;

    try {
        Application application;
        application.start(argc, argv); //start the server
    } catch (const std::exception& e) {
        // exception handling
    }

    return 0;
}
```
The function Application::start() launches the server. This function blocks the current thread. In order to stop the running server, use the function Application::stop().

3. Place static resources (HTML, CSS, javascript, images etc) into the directory

/var/www/webserver/

Lets create a folder "pages" with a page "helloWorld.html" in this directory.

4. Thats it. Now run your application and try to access the resource:

http://localhost:8080/pages/helloWorld.html

You should see the content of helloWorld.html page.

## Configuration

The configuration is stored in a json file.

By default, the server is looking for the configuration file using the following path:

/etc/webserver/config/application.json

If you place a file in another directory or choose different name, provide the path to the file in command line arguments:

./<your application name> -c "/my/path/to/config.json"

The structore of the configuration file:
```
{
    "serverPort": 8080,
    "serverName": "Demo",
    "ssl": {
        "enabled": true,
        "certificateFilepath": "/etc/webserver/ssl/demo.crt",
        "privateKeyFilepath": "/etc/webserver/ssl/demo.key",
        "dhFilepath": "/etc/webserver/ssl/dh4096.pem",
        "password": "demo123"
    },
    "staticResourceMapping": {
        "baseDir": "/var/www/webserver/",
        "errorPageMapping": [
            {
                "status": 401,
                "page": "errors/forbidden.html"
            },
            {
                "status": 404,
                "page": "/errors/notFound.html"
            }
        ]
    },
    "mediaTypeMapping": [
        {
            "fileNameRegex": ".+[.]json$",
            "mediaType": "application/json"
        },
        {
            "fileNameRegex": ".+[.]jpeg$",
            "mediaType": "image/pjpeg"
        }
    ]
}
```

The configuration properties:
| Property                                      | Default                        | Description   |
| --------------------------------------------- | ------------------------------ | ------------- |
| serverPort                                    | 8080                           | The server will listen on this port.  | 
| serverName                                    | Web Server                     | The value of "Server" header in HTTP responses, returned by the server. |
| ssl.enabled                                   | false                          | Enables or disables HTTPS. |
| ssl.certificateFilepath                       |                                | The path to SSL certificate file |
| ssl.privateKeyFilepath                        |                                | The path to the private key  |
| ssl.dhFilepath                                |                                | The path to the Diffie-Hellman key file |
| ssl.password                                  |                                | The certificate's pass phrase |
| staticResourceMapping.baseDir                 | /var/www/webserver/            | The server will look for static content in this directory. |
| staticResourceMapping.errorPageMapping        |                                | Contains a mapping between HTTP status codes and error pages. |
| staticResourceMapping.errorPageMapping.status |                                | An HTTP status (positive integer number). |
| staticResourceMapping.errorPageMapping.page   |                                | A relative path to an error page. |
| mediaTypeMapping                              |                                | Contains a mapping between resource names and media types. May contain any number of elements       |
| mediaTypeMapping.fileNameRegex                |                                | The filename pattern. If the request target is "/target/url/foo.html", the filename is "foo".    |
| mediaTypeMapping.mediaType                    |                                | The media type which is mapped to the given filename. |

## Custom controllers

If you want to manage dynamic content, you need to create your own controller:

```
#include "HttpController.h"

class FooController : public HttpController {
public:
    void processRequest(HttpRequestHolder& requestHolder, HttpResponse& response) override {
        // add business logic here
    }

};
```

The controller is registered like below. You may add any number of controllers.

```
#include "Application.h"
#include "HttpController.h"

...

std::vector<HttpControllerMapping> controllerMapping;
controllerMapping.push_back({"/foo", std::make_unique<FooController>()});

Application application;
application.start(argc, argv, controllerMapping);
```

FooController::processRequest() will be called if the request target matches to "/foo".

## Exception handling

There are two common cases: 1. returning the static error case for the given HTTP status and 2. handling an exception.

### Return a static page for a given HTTP status

In this case just add a mapping to the configuration:
```
{
    "staticResourceMapping": {
        "baseDir": "/var/www/webserver/",
        "errorPageMapping": [
            {
                "status": 401,
                "page": "errors/forbidden.html"
            },
        ]
    }
}
```
Then throw an exception in your controller:
```
class YourController : public HttpController {

    void handle(HttpRequestHandeler& handler, HttpResponse& response) {

        ...
        throw HttpErrorException(401);
        ...
    }
}
```
In the example above the response will contain the page "/var/www/webserver/errors/forbidden.html" with the HTTP status 401.

Please take a note that the page is returned only if HttpErrorException has been thrown.

If you just write the status 401 in the response, the page won't be returned.

### Return handle an exception

If you need more complex excetpion handling, you can create your own handler.

First, create an exception class.

```
class YourException : public HttpControllerException {
public:

    ErrorCode getErrorCode() override {
        return 1;
    }

};
```

The method getErrorCode() returns the code which will be used to bind the exception with the handler.

Second, create the handler:

```
class YourExceptionHandler : public HttpControllerExceptionHandler {

    void handle(const HttpControllerException& exception, HttpResponse& response) override {
        const YourException& yourException = static_cast<const YourException&>(exception);

        // write the response
    }

};
```

Last, register the handler:
```
std::unordered_map<int, HttpControllerExceptionHandlerPtr> exceptionHandlerById;
exceptionHandlerById[1] = std::make_shared<YourExceptionHandler>();

Application application;
application.start(argc, argv, std::move(controllerMapping), std::move(exceptionHandlerById));
```
