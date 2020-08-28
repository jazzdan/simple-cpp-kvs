#include <optional>
#include <string>
#include <tuple>

using key = std::string;
using op = std::string;

std::tuple<op, key, std::optional<std::string>> parse(std::string cmd);