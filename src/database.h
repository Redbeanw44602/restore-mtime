#pragma once

class Database {
public:
    using FileHashT = std::string;
    using MTimeT    = unsigned long long;

    // TODO: replace with std::expected
    static std::optional<Database> fromFile(std::string_view path, std::error_code& ec);
    void                           saveTo(std::string_view path, std::error_code& ec);

    std::optional<MTimeT> queryLastModifiedTime(FileHashT hash) const;
    void                  storeLastModifiedTime(FileHashT hash, MTimeT mtime);

private:
    std::unordered_map<FileHashT, MTimeT> m_database;
};
