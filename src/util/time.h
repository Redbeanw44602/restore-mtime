#pragma once

#include <filesystem>

namespace util::time {

std::filesystem::file_time_type to_filetime(int64_t timestamp);

long int from_filetime(const std::filesystem::file_time_type& filetime);

} // namespace util::time
