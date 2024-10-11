#pragma once

#include <filesystem>

namespace util::time {

std::filesystem::file_time_type to_filetime(time_t timestamp);

time_t from_filetime(const std::filesystem::file_time_type& filetime);

std::string to_string(time_t timestamp);

} // namespace util::time
