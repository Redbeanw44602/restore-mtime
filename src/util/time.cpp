#include "time.h"

namespace util::time {

std::filesystem::file_time_type to_filetime(int64_t timestamp) {
    auto time_point = std::chrono::system_clock::from_time_t(timestamp);
    auto duration   = std::chrono::time_point_cast<std::filesystem::file_time_type::duration>(time_point);
    return std::filesystem::file_time_type(duration.time_since_epoch());
}

long int from_filetime(const std::filesystem::file_time_type& filetime) {
    auto time_point = filetime.time_since_epoch();
    auto duration   = std::chrono::duration_cast<std::chrono::seconds>(time_point);
    return duration.count();
}

} // namespace util::time
