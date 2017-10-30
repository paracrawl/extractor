
#include "gtest/gtest.h"
#include "../src/utils/common.h"

TEST(utils, test_common_parse_uri) {
  utils::parse_uri url1("http://blogos.com/article/118568/");
  utils::parse_uri url2("http://blogos.com");
  utils::parse_uri url3("http://000.blogos.com");
  utils::parse_uri url4("https://blogos.com/article/118568/");
  utils::parse_uri url5("https://blogos.com");
  utils::parse_uri url6("https://000.blogos.com");
  utils::parse_uri url7("111.blogos.com");
  utils::parse_uri url8("eu.blogos.com:1111/article");
  utils::parse_uri url9("username@us.blogos.com:1111/article");
  utils::parse_uri url10("http://username@us.blogos.com:1111/article?username=noname");
  utils::parse_uri url11("ftp://blogos.com");
  utils::parse_uri url12("ftp://username@us.blogos.com:1111/article?username=noname#h1");
  utils::parse_uri url13("foofoofoo");
  utils::parse_uri url14("foofoofoo/bar");

  ASSERT_EQ(url1.get_domain(), "blogos.com");
  ASSERT_EQ(url2.get_domain(), "blogos.com");
  ASSERT_EQ(url3.get_domain(), "000.blogos.com");
  ASSERT_EQ(url4.get_domain(), "blogos.com");
  ASSERT_EQ(url5.get_domain(), "blogos.com");
  ASSERT_EQ(url6.get_domain(), "000.blogos.com");
  ASSERT_EQ(url7.get_domain(), "111.blogos.com");
  ASSERT_EQ(url8.get_domain(), "eu.blogos.com");
  ASSERT_EQ(url9.get_domain(), "us.blogos.com");
  ASSERT_EQ(url10.get_domain(), "us.blogos.com");
  ASSERT_EQ(url11.get_domain(), "blogos.com");
  ASSERT_EQ(url12.get_domain(), "us.blogos.com");
  ASSERT_EQ(url13.get_domain(), "foofoofoo");
  ASSERT_EQ(url14.get_domain(), "foofoofoo");

  ASSERT_EQ(url1.get_tld(), "com");
  ASSERT_EQ(url6.get_tld(), "com");
  ASSERT_EQ(url13.get_tld(), "foofoofoo");

  ASSERT_EQ(url1.get_path(), "article/118568/");
  ASSERT_EQ(url8.get_path(), "article");
  ASSERT_EQ(url10.get_path(), "article");
  ASSERT_EQ(url12.get_path(), "article");
  ASSERT_EQ(url13.get_path(), "");
  ASSERT_EQ(url14.get_path(), "bar");
}