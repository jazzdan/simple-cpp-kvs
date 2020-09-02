#include <optional>
#include <string>
#include <tuple>

#include "absl/strings/str_split.h"

using namespace std;

std::tuple<string, string, optional<string>> handleSet(vector<string> parts) {
  if (parts.size() != 3) {
    throw "incorrect number of arguments";
  }

  return tuple<string, string, optional<string>>{"set", parts[1], parts[2]};
}

std::tuple<string, string, optional<string>> parse(string cmd) {
  vector<string> parts = absl::StrSplit(cmd, " ");

  if (parts.size() == 0) {
    throw "can't parse an empty string";
  }

  auto verb = parts[0];

  if (verb == "set") {
    return handleSet(parts);
  } else {
    throw "invalid verb";
  }

  return tuple<string, string, optional<string>>{};
}