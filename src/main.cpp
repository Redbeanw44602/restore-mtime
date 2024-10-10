#include "database.h"

#include "util/hash.h"
#include "util/time.h"

#include <argparse/argparse.hpp>

#include <filesystem>

void setup_logger() {
#ifdef DEBUG
    logging::set_level(logging::level::debug);
#endif
    logging::set_pattern("[%H:%M:%S.%e] [%^%l%$] %v");
}

int main(int argc, char** argv) try {

    setup_logger();

    // clang-format off

    argparse::ArgumentParser program("restore-mtime", "0.1.0");

    argparse::ArgumentParser program_save("save");
    program_save.add_description("Generates a database that stores the mtime of files.");

    program_save.add_argument("-d", "--add-directory")
        .help("Add a directory.")
        .append()
        .default_value<std::vector<std::string>>({ "." });
    
    program_save.add_argument("-f", "--add-file")
        .help("Add a file.")
        .append();
    
    program_save.add_argument("-o", "--output")
        .help("Where to save the database file?")
        .required();

    argparse::ArgumentParser program_restore("restore");
    program_restore.add_description("Restore the file's mtime from the database.");

    program_restore.add_argument("-b", "--base-dir")
        .help("Set the base path for restore.")
        .default_value(".");
    
    program_restore.add_argument("-i", "--input")
        .help("Where is the database that stores file mtime?")
        .required();

    program.add_subparser(program_save);
    program.add_subparser(program_restore);

    program.parse_args(argc, argv);

    // clang-format on

    if (program.is_subcommand_used(program_save)) {
        auto directories = program_save.get<std::vector<std::string>>("-d");
        auto files       = program_save.get<std::vector<std::string>>("-f");
        auto paths       = std::move(directories);
        paths.reserve(paths.size() + files.size());
        paths.insert(paths.end(), std::make_move_iterator(files.begin()), std::make_move_iterator(files.end()));

        Database db;
        for (const auto& path : paths) {
            if (std::filesystem::is_directory(path)) {
                for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
                    if (!entry.is_regular_file()) {
                        if (!entry.is_directory())
                            logging::warn("'{}' is not a regular file, ignored.", entry.path().string());
                        continue;
                    }
                    auto file_hash = util::hash::calc_file_md5(entry.path().string());
                    logging::debug("[{}:{}] {}", file_hash, "x", entry.path().string());
                    db.storeLastModifiedTime(file_hash, util::time::from_filetime(entry.last_write_time()));
                }
            } else {
                if (!std::filesystem::exists(path)) {
                    logging::warn("'{}' is not exists, ignored", path);
                    continue;
                }
                if (!std::filesystem::is_regular_file(path)) {
                    logging::warn("'{}' is not a regular file, ignored.", path);
                    continue;
                }
                auto file_hash = util::hash::calc_file_md5(path);
                logging::debug("[{}:{}] {}", file_hash, "x", path);
                db.storeLastModifiedTime(file_hash, util::time::from_filetime(std::filesystem::last_write_time(path)));
            }
        }

        auto output_file_path = program_save.get<std::string>("-o");

        std::error_code ec;
        db.saveTo(output_file_path, ec);

        if (ec) {
            logging::error(ec.message());
            return 1;
        }

        return 0;
    }

    if (program.is_subcommand_used(program_restore)) {
        return 0;
    }

    return 0;
} catch (const std::runtime_error& e) {
    logging::error(e.what());
    return 1;
}
