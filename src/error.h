#pragma once

// Load

enum class DatabaseLoadErrorCode { SUCCESS = 0, UnableToOpenFile, UnableToReadHeader, IllegalFileFormat };

class DatabaseLoadErrorCategory : public std::error_category {
public:
    const char* name() const noexcept override { return "DatabaseLoadError"; }
    std::string message(int ev) const override {
        switch (static_cast<DatabaseLoadErrorCode>(ev)) {
        case DatabaseLoadErrorCode::SUCCESS:
            return "Everything is ok.";
        case DatabaseLoadErrorCode::UnableToOpenFile:
            return "Unable to open file.";
        case DatabaseLoadErrorCode::UnableToReadHeader:
            return "Invalid file header, did you select the correct database file?";
        case DatabaseLoadErrorCode::IllegalFileFormat:
            return "Unable to parse the database, perhaps the file is corrupted.";
        default:
            return {};
        }
    }
};

inline const DatabaseLoadErrorCategory database_load_error_category{};

inline std::error_code make_database_load_error(DatabaseLoadErrorCode ec) {
    return {static_cast<int>(ec), database_load_error_category};
}

// Save

enum class DatabaseSaveErrorCode { SUCCESS = 0, UnableToOpenFile };

class DatabaseSaveErrorCategory : public std::error_category {
public:
    const char* name() const noexcept override { return "DatabaseSaveError"; }
    std::string message(int ev) const override {
        switch (static_cast<DatabaseSaveErrorCode>(ev)) {
        case DatabaseSaveErrorCode::SUCCESS:
            return "Everything is ok.";
        case DatabaseSaveErrorCode::UnableToOpenFile:
            return "Unable to open file.";
        default:
            return {};
        }
    }
};

inline const DatabaseSaveErrorCategory database_save_error_category{};

inline std::error_code make_database_save_error(DatabaseSaveErrorCode ec) {
    return {static_cast<int>(ec), database_save_error_category};
}