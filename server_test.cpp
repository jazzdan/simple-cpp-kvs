#include "gtest/gtest.h"
#include "kvslib.h"

TEST(ServerShould, Set) {
  auto myKVS = InMemoryKVS<std::string>();
  myKVS.put("foo", "bar");
  EXPECT_EQ(myKVS.get("foo"), "bar");
}