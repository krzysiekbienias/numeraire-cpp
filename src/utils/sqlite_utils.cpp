#include "utils/sqlite_utils.hpp"
#include "utils/logger.hpp"

namespace sqlite_utils {

sqlite3* openDatabase(const std::string& db_path) {
    sqlite3* db = nullptr;
    if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
        Logger::get()->error("❌ Could not open DB: {}", sqlite3_errmsg(db));
        return nullptr;
    }
    Logger::get()->info("✅ Connected to DB at: {}", db_path);
    return db;
}

void closeDatabase(sqlite3* db) {
    if (db) sqlite3_close(db);
}

std::optional<sqlite3_stmt*> prepareStatement(sqlite3* db, const std::string& sql) {
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        Logger::get()->error("❌ Failed to prepare statement: {}", sqlite3_errmsg(db));
        return std::nullopt;
    }
    return stmt;
}

void bindInt(sqlite3_stmt* stmt, int index, int value) {
    sqlite3_bind_int(stmt, index, value);
}

std::optional<std::string> getStringOrNull(sqlite3_stmt* stmt, int col) {
    return sqlite3_column_type(stmt, col) != SQLITE_NULL
        ? std::optional<std::string>{reinterpret_cast<const char*>(sqlite3_column_text(stmt, col))}
        : std::nullopt;
}

std::optional<double> getDoubleOrNull(sqlite3_stmt* stmt, int col) {
    return sqlite3_column_type(stmt, col) != SQLITE_NULL
        ? std::optional<double>{sqlite3_column_double(stmt, col)}
        : std::nullopt;
}

}
