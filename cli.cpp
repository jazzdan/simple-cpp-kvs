#include "cli.h"

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

Command::Command(Op o, key k, value v) {
  myOp = o;
  myKey = k;
  myValue = v;
}

std::string op_to_string(Op op) {
  switch (op) {
    case getOp:
      return "get";
    case setOp:
      return "set";
    case deleteOp:
      return "delete";
  }
}

std::ostream &operator<<(std::ostream &strm, const Command &cmd) {
  strm << "Command(" << op_to_string(cmd.myOp) << " " << cmd.myKey;
  if (cmd.myValue.has_value()) {
    strm << " " << cmd.myValue.value();
  }
  strm << ")" << std::endl;

  return strm;
}

std::string Command::getKey() { return this->myKey; }
Op Command::getOp() { return this->myOp; }
std::optional<std::string> Command::getValue() { return this->myValue; }

Command parse(string cmd) {
  vector<string> parts = absl::StrSplit(cmd, " ");

  if (parts.size() == 0) {
    throw "can't parse an empty string";
  }

  auto op = parts[0];

  if (op == "set") {
    auto p = handleSet(parts);
    return Command(setOp, get<1>(p), get<2>(p));
  } else if (op == "get") {
    auto p = handleGet(parts);
    return Command(getOp, get<1>(p), get<2>(p));
  } else if (op == "delete") {
    auto p = handleDelete(parts);
    return Command(deleteOp, get<1>(p), get<2>(p));
  } else {
    throw "invalid op";
  }
}
