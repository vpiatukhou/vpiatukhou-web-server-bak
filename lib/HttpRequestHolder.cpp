#include "HttpRequestHolder.h"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

namespace WebServer {

    namespace {
        const char QUERY_STRING_START = '?';
        const std::string QUERY_STRING_PAIRS_DELIMITER = "&";
        const std::string QUERY_STRING_KEY_VALUE_DELIMITER = "=";
    }

    void HttpRequestHolder::init() {
        auto boostTarget = request.target();
        std::string target(boostTarget.data(), boostTarget.size());
        auto delimiterPos = target.find_first_of(QUERY_STRING_START);
        if (delimiterPos == std::string::npos) {
            requestUri = std::move(target);
        } else {
            requestUri = target.substr(0, delimiterPos);
            if (delimiterPos + 1 < target.size()) {
                queryString = target.substr(delimiterPos + 1);
            }
        }
    }

    const std::string& HttpRequestHolder::getRequestUri() const {
        return requestUri;
    }

    HttpRequest& HttpRequestHolder::getRequest() {
        return request;
    }

    const QueryParameters& HttpRequestHolder::getQueryParameters() {
        if (!queryParameters.has_value()) {
            queryParameters.emplace();

            if (!queryString.empty()) {
                std::vector<std::string> keyValues;
                boost::algorithm::split(keyValues, queryString, boost::is_any_of(QUERY_STRING_PAIRS_DELIMITER));
                for (auto& keyValue : keyValues) {
                    std::vector<std::string> tokens;
                    boost::algorithm::split(tokens, keyValue, boost::is_any_of(QUERY_STRING_KEY_VALUE_DELIMITER));
                    if (tokens.size() == 2) {
                        (*queryParameters)[tokens[0]] = tokens[1];
                    } else if (tokens.size() == 1) {
                        (*queryParameters)[tokens[0]] = std::string();
                    }
                }
            }
        }
        return *queryParameters;
    }

}
