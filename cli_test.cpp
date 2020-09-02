#include "cli.h"

#include "gtest/gtest.h"

TEST(ParseShould, SetString) {
  std::string input = "set foo bar";
  std::string key = "foo";
  auto expected = Command(setOp, key, "bar");
  auto output = parse(input);
  EXPECT_EQ(output, expected);
}

TEST(ParseGet, GetString) {
  std::string input = "get foo";
  std::string key = "foo";
  auto expected = Command(getOp, key, std::optional<std::string>{});
  auto output = parse(input);
  EXPECT_EQ(output, expected);
}

TEST(ParseDelete, DeleteString) {
  std::string input = "delete foo";
  std::string key = "foo";
  auto expected = Command(deleteOp, key, std::optional<std::string>{});
  auto output = parse(input);
  EXPECT_EQ(output, expected);
}

// TODO(dmiller): not sure how to assert on a specific exception
TEST(ParseThrows, UnknownOp) {
  std::string input = "foo bar";
  EXPECT_ANY_THROW(parse(input));
}

TEST(ParseThrows, EmptyString) {
  std::string input = "";
  EXPECT_ANY_THROW(parse(input));
}