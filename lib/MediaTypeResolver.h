#pragma once

#include "ApplicationConfig.h"

#include <memory>
#include <regex>
#include <string>
#include <vector>
#include <utility>

namespace WebServer {

    /**
     * Finds a media type for the given resouce (e.g. 'text/html' for HTML page).
     */
    class MediaTypeResolver {
    public:
        MediaTypeResolver(const std::vector<MediaTypeMapping>& mediaTypeMapping);
        MediaTypeResolver(const MediaTypeResolver&) = delete;
        MediaTypeResolver(MediaTypeResolver&&) = delete;
        ~MediaTypeResolver() = default;

        MediaTypeResolver& operator=(const MediaTypeResolver&) = delete;
        MediaTypeResolver& operator=(MediaTypeResolver&&) = delete;

        /**
         * @return a media type for the given filename
         * @see MediaTypes.h
         */
        std::string getMediaTypeByFilename(const std::string& filename);

    private:
        std::vector<std::pair<std::regex, std::string>> mediaTypesMapping;
    };

    using MediaTypeResolverPtr = std::shared_ptr<MediaTypeResolver>;

}
