#include "cli.h"

#include "gtest/gtest.h"

TEST(GreetingShould, ReturnHelloWorld) {
  std::string input = "set foo bar";
  std::string op = "set";
  std::string key = "foo";
  auto expected =
      std::make_tuple(op, key, std::optional<std::string>{"bar"});
  auto output = parse(input);
  EXPECT_EQ(output, expected);
}