#pragma once

#include "HttpRequestResponse.h"

#include <optional>
#include <string>
#include <unordered_map>

namespace WebServer {

    /**
     * Contains parameters from the query string of the URL.
     */
    using QueryParameters = std::unordered_map<std::string, std::string>;

    /**
     * Holds a Boost request object.
     */
    class HttpRequestHolder {
    public:
        HttpRequestHolder() = default;
        HttpRequestHolder(const HttpRequestHolder&) = delete;
        HttpRequestHolder(HttpRequestHolder&&) = delete;
        ~HttpRequestHolder() = default;

        HttpRequestHolder& operator=(const HttpRequestHolder&) = delete;
        HttpRequestHolder& operator=(HttpRequestHolder&&) = delete;

        /**
         * Initializes the request holder.
         * 
         * This method must be called after the incoming request is set to HttpRequestHolder::request.
         */
        void init();

        /**
         * Return a part of URL after the host and without the query string.
         * 
         * E.g. if a URL is 'https://localhost:8080/foo/bar?key=value'
         * 
         * The request URI is '/foo/bar'.
         * 
         * @return the request URI
         */
        const std::string& getRequestUri() const;

        /**
         * @return a Boost Beast HTTP request object.
         */
        HttpRequest& getRequest();

        /**
         * Parses the query string and returns a map of query parameter names and values.
         * 
         * The query string is parsed only when the method is called at the first time.
         * Following calls use cached values.
         * 
         * @return a map of query parameters names and values
         */
        const QueryParameters& getQueryParameters();

    private:
        std::string requestUri;
        std::string queryString;
        HttpRequest request;
        std::optional<QueryParameters> queryParameters;
    };

}