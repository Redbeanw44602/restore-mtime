#pragma once

namespace util::string {

constexpr bool is_hexstring(std::string_view str) {
    return std::all_of(str.begin(), str.end(), [](unsigned char chr) { return std::isxdigit(chr); });
}

std::optional<long> to_longint(const std::string& str);

} // namespace util::string