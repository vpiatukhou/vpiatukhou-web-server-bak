#include "HttpRequestHolder.h"

#include "gtest/gtest.h"

namespace {
    using namespace WebServer;
}

TEST(HttpRequestHolderTest, getQueryParameters_emptyTarget) {
    //given
    HttpRequestHolder requestHolder;
    requestHolder.getRequest().target("");
    requestHolder.init();

    //when
    auto result = requestHolder.getQueryParameters();

    //then
    ASSERT_EQ("", requestHolder.getRequestUri());
    ASSERT_TRUE(result.empty());
}

TEST(HttpRequestHolderTest, getQueryParameters_rootTarget) {
    //given
    HttpRequestHolder requestHolder;
    requestHolder.getRequest().target("/");
    requestHolder.init();

    //when
    auto result = requestHolder.getQueryParameters();

    //then
    ASSERT_EQ("/", requestHolder.getRequestUri());
    ASSERT_TRUE(result.empty());
}

TEST(HttpRequestHolderTest, getQueryParameters_noParams) {
    //given
    HttpRequestHolder requestHolder;
    requestHolder.getRequest().target("/foo");
    requestHolder.init();

    //when
    auto result = requestHolder.getQueryParameters();

    //then
    ASSERT_EQ("/foo", requestHolder.getRequestUri());
    ASSERT_TRUE(result.empty());
}

TEST(HttpRequestHolderTest, getQueryParameters_oneParam) {
    //given
    HttpRequestHolder requestHolder;
    requestHolder.getRequest().target("/foo?key=value");
    requestHolder.init();

    //when
    auto result = requestHolder.getQueryParameters();

    //then
    ASSERT_EQ("/foo", requestHolder.getRequestUri());
    ASSERT_EQ(1, result.size());
    auto it = result.begin();
    ASSERT_EQ("key", it->first);
    ASSERT_EQ("value", it->second);
}

TEST(HttpRequestHolderTest, getQueryParameters_manyParams) {
    //given
    HttpRequestHolder requestHolder;
    requestHolder.getRequest().target("/foo?key1=value1&key2=value2&key3=value3");
    requestHolder.init();

    //when
    auto result = requestHolder.getQueryParameters();

    //then
    ASSERT_EQ("/foo", requestHolder.getRequestUri());
    ASSERT_EQ(3, result.size());
    ASSERT_EQ("value1", result["key1"]);
    ASSERT_EQ("value2", result["key2"]);
    ASSERT_EQ("value3", result["key3"]);
}
