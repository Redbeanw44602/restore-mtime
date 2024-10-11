#include "database.h"
#include "error.h"

#include "util/string.h"

#include <fstream>

constexpr char HEADER[] = "# restore-mtime database file, version: 1.";

std::optional<Database> Database::fromFile(std::string_view path, std::error_code& ec) {
    std::ifstream file(path.data());
    if (!file) {
        ec = make_database_load_error(DatabaseLoadErrorCode::UnableToOpenFile);
        return {};
    }

    std::string line;
    if (!std::getline(file, line) || line != HEADER) {
        ec = make_database_load_error(DatabaseLoadErrorCode::UnableToReadHeader);
        return {};
    }

    Database result;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        auto space_position = line.find(' ');
        if (space_position == std::string::npos) {
            ec = make_database_load_error(DatabaseLoadErrorCode::IllegalFileFormat);
            return {};
        }
        auto hash     = line.substr(0, space_position);
        auto time_str = line.substr(space_position);

        auto time_val = util::string::to_longint(time_str);
        if (!time_val || !util::string::is_hexstring(hash)) {
            ec = make_database_load_error(DatabaseLoadErrorCode::IllegalFileFormat);
            return {};
        }

        logging::debug("Loaded [{}:{}]", hash, *time_val);
        result.storeLastModifiedTime(hash, *time_val);
    }
    return result;
}

void Database::saveTo(std::string_view path, std::error_code& ec) {
    std::ofstream file(path.data());
    if (!file) {
        ec = make_database_save_error(DatabaseSaveErrorCode::UnableToOpenFile);
        return;
    }

    file << HEADER;
    for (auto& [key, value] : m_database) {
        file << '\n' << key << ' ' << value;
    }
}

std::optional<Database::MTimeT> Database::queryLastModifiedTime(FileHashT hash) const {
    if (!m_database.contains(hash)) return std::nullopt;
    return m_database.at(hash);
}

void Database::storeLastModifiedTime(FileHashT hash, MTimeT mtime) { m_database.try_emplace(hash, mtime); }
