#include "time.h"

namespace util::time {

std::filesystem::file_time_type to_filetime(time_t timestamp) {
    auto time_point = std::chrono::system_clock::from_time_t(timestamp);
    return std::chrono::clock_cast<std::chrono::file_clock>(time_point);
}

time_t from_filetime(const std::filesystem::file_time_type& filetime) {
    auto system_clock = std::chrono::clock_cast<std::chrono::system_clock>(filetime);
    auto time_point   = system_clock.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::seconds>(time_point).count();
}

std::string to_string(time_t timestamp) {
    auto time_point = std::chrono::system_clock::from_time_t(timestamp);
    auto time       = std::chrono::system_clock::to_time_t(time_point);
    auto localtime  = std::localtime(&time);

    std::ostringstream stream;
    stream << std::put_time(localtime, "%Y-%m-%d %H:%M:%S %Z");
    return stream.str();
}

} // namespace util::time
