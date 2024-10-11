#include "time.h"

namespace util::time {

constexpr time_t std_time_point_to_timestamp_second_offset = 6437635199;

std::filesystem::file_time_type to_filetime(time_t timestamp) {
    timestamp       -= std_time_point_to_timestamp_second_offset;
    auto time_point  = std::chrono::system_clock::from_time_t(timestamp);
    auto duration    = std::chrono::time_point_cast<std::filesystem::file_time_type::duration>(time_point);
    return std::filesystem::file_time_type(duration.time_since_epoch());
}

time_t from_filetime(const std::filesystem::file_time_type& filetime) {
    auto time_point = filetime.time_since_epoch();
    auto duration   = std::chrono::duration_cast<std::chrono::seconds>(time_point);
    return duration.count() + std_time_point_to_timestamp_second_offset;
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
