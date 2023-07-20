#include "MediaTypeResolver.h"
#include "MediaType.h"

namespace WebServer {

    namespace {
        const std::regex CSS_PATTERN(".+[.]css$");
        const std::regex HTML_PATTERN(".+[.]html$");
        const std::regex JS_PATTERN(".+[.]js$");
        const std::regex JPEG_PATTERN(".+[.](jpeg|jpg)$");
        const std::regex PNG_PATTERN(".+[.]png$");
        const std::regex SVG_PATTERN(".+[.]svg$");
    }

    MediaTypeResolver::MediaTypeResolver(const std::vector<MediaTypeMapping>& mediaTypeMapping) {
        for (auto& config : mediaTypeMapping) {
            mediaTypesMapping.push_back({ std::regex(config.filenameRegExp), config.mediaType });
        }

        mediaTypesMapping.push_back({ CSS_PATTERN, MEDIA_TYPE_TEXT_CSS });
        mediaTypesMapping.push_back({ HTML_PATTERN, MEDIA_TYPE_TEXT_HTML });
        mediaTypesMapping.push_back({ JS_PATTERN, MEDIA_TYPE_TEXT_JAVASCRIPT });
        mediaTypesMapping.push_back({ JPEG_PATTERN, MEDIA_TYPE_IMAGE_JPEG });
        mediaTypesMapping.push_back({ PNG_PATTERN, MEDIA_TYPE_IMAGE_PNG });
        mediaTypesMapping.push_back({ SVG_PATTERN, MEDIA_TYPE_IMAGE_SVG });
    }

    std::string MediaTypeResolver::getMediaTypeByFilename(const std::string& filename) {
        for (auto& mediaType : mediaTypesMapping) {
            std::smatch match;
            if (std::regex_match(filename, match, mediaType.first)) {
                return mediaType.second;
            }
        }
        return MEDIA_TYPE_APPLICATION_OCTET_STREAM;
    }

}
