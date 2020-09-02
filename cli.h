#include <optional>
#include <ostream>
#include <string>
#include <tuple>

enum Op { getOp, setOp, deleteOp };

using key = std::string;
using value = std::optional<std::string>;

class Command {
 private:
  Op myOp;
  key myKey;
  value myValue;

 public:
  Command(Op, key, value);
  friend std::ostream& operator<<(std::ostream&, const Command&);
  bool operator==(const Command& cmd) const {
    return this->myKey == cmd.myKey && this->myOp == cmd.myOp &&
           this->myValue == cmd.myValue;
  };
  Op getOp();
  key getKey();
  value getValue();
};

Command parse(std::string cmd);