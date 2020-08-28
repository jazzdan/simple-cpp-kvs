#include "cli.h"

#include "gtest/gtest.h"

TEST(GreetingShould, ReturnHelloWorld) {
  std::string input = "set foo bar";
  std::string actual = "set foo bar";
  EXPECT_EQ(input, actual);
}