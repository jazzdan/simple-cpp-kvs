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

std::tuple<string, string, optional<string>> handleGet(vector<string> parts) {
  if (parts.size() != 2) {
    throw "incorrect number of arguments";
  }

  return tuple<string, string, optional<string>>{"get", parts[1],
                                                 optional<string>{}};
}

std::tuple<string, string, optional<string>> handleDelete(
    vector<string> parts) {
  if (parts.size() != 2) {
    throw "incorrect number of arguments";
  }

  return tuple<string, string, optional<string>>{"delete", parts[1],
                                                 optional<string>{}};
}

std::tuple<string, string, optional<string>> parse(string cmd) {
  vector<string> parts = absl::StrSplit(cmd, " ");

  if (parts.size() == 0) {
    throw "can't parse an empty string";
  }

  auto op = parts[0];

  if (op == "set") {
    return handleSet(parts);
  } else if (op == "get") {
    return handleGet(parts);
  } else if (op == "delete") {
    return handleDelete(parts);
  } else {
    throw "invalid op";
  }
}