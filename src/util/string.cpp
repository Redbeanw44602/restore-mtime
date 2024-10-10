#include "string.h"

namespace util::string {

std::optional<long> to_longint(const std::string& str) {
    char* str_end = nullptr;
    errno         = 0;

    auto value = std::strtol(str.c_str(), &str_end, 0);

    if (str_end == str.c_str()) return {};
    if (errno == ERANGE) return {};
    if (*str_end != '\0') return {};

    return value;
}

} // namespace util::string