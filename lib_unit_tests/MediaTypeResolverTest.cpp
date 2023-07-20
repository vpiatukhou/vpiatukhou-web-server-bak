#include "MediaTypeResolver.h"
#include "MediaType.h"

#include "gtest/gtest.h"

#include <utility>

using namespace WebServer;

namespace {
    const std::string MEDIA_TYPE_IMAGE_PJPEG = "image/pjpeg";
    const std::string MEDIA_TYPE_APPLICATION_JSON = "application/json";
}

using FileAndMediaType = std::pair<std::string, std::string>;

class MediaTypeResolverDefaultTypesOnlyTest : public testing::TestWithParam<FileAndMediaType> {
};

class MediaTypeByTargetReadFromConfigTest : public testing::TestWithParam<FileAndMediaType> {
};

INSTANTIATE_TEST_SUITE_P(MediaTypeResolverSuit, MediaTypeResolverDefaultTypesOnlyTest, testing::Values(
    FileAndMediaType("aaa.css", MEDIA_TYPE_TEXT_CSS),
    FileAndMediaType("aaa.html", MEDIA_TYPE_TEXT_HTML),
    FileAndMediaType("aaa.js", MEDIA_TYPE_TEXT_JAVASCRIPT),
    FileAndMediaType("aaa.jpeg", MEDIA_TYPE_IMAGE_JPEG),
    FileAndMediaType("aaa.png", MEDIA_TYPE_IMAGE_PNG),
    FileAndMediaType("aaa.svg", MEDIA_TYPE_IMAGE_SVG)
));

INSTANTIATE_TEST_SUITE_P(MediaTypeResolverSuit, MediaTypeByTargetReadFromConfigTest, testing::Values(
    //default types
    FileAndMediaType("aaa.css", MEDIA_TYPE_TEXT_CSS),
    FileAndMediaType("aaa.html", MEDIA_TYPE_TEXT_HTML),
    FileAndMediaType("aaa.js", MEDIA_TYPE_TEXT_JAVASCRIPT),
    FileAndMediaType("aaa.png", MEDIA_TYPE_IMAGE_PNG),
    FileAndMediaType("aaa.svg", MEDIA_TYPE_IMAGE_SVG),

    //new configured type
    FileAndMediaType("aaa.json", MEDIA_TYPE_APPLICATION_JSON),

    //overloaded type
    FileAndMediaType("aaa.jpeg", MEDIA_TYPE_IMAGE_PJPEG)
));

TEST_P(MediaTypeResolverDefaultTypesOnlyTest, getMediaTypeByTarget_defaultTypesOnly) {
    //when
    auto param = GetParam();

    std::vector<MediaTypeMapping> config;
    MediaTypeResolver resolver(config);

    //when
    auto result = resolver.getMediaTypeByFilename(param.first);

    //then
    ASSERT_EQ(param.second, result);
}

TEST_P(MediaTypeByTargetReadFromConfigTest, getMediaTypeByTarget_readFromConfig) {
    //when
    auto param = GetParam();

    std::vector<MediaTypeMapping> config;
    config.push_back({".+[.]json$", MEDIA_TYPE_APPLICATION_JSON});
    config.push_back({".+[.]jpeg$", MEDIA_TYPE_IMAGE_PJPEG});
    MediaTypeResolver resolver(config);

    //when
    auto result = resolver.getMediaTypeByFilename(param.first);

    //then
    ASSERT_EQ(param.second, result);
}
