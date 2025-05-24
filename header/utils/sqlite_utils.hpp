
#pragma once
#include <sqlite3.h>
#include <optional>
#include <string>

namespace sqlite_utils {

// Open/close helpers
sqlite3* openDatabase(const std::string& db_path);
void closeDatabase(sqlite3* db);

// Statement helpers
std::optional<sqlite3_stmt*> prepareStatement(sqlite3* db, const std::string& sql);
void bindInt(sqlite3_stmt* stmt, int index, int value);

// Column extractors
std::optional<std::string> getStringOrNull(sqlite3_stmt* stmt, int col);
std::optional<double> getDoubleOrNull(sqlite3_stmt* stmt, int col);

}
