#include "database/db_connection.hpp"
#include <stdexcept>

DbConnection::DbConnection(const std::string& path) {
    open(path);
}

DbConnection::~DbConnection() {
    close();
}

DbConnection::DbConnection(DbConnection&& other) noexcept
    : m_db(other.m_db) {
    other.m_db = nullptr;
}

DbConnection& DbConnection::operator=(DbConnection&& other) noexcept {
    if (this != &other) {
        close();
        m_db = other.m_db;
        other.m_db = nullptr;
    }
    return *this;
}

void DbConnection::open(const std::string& path) {
    if (sqlite3_open(path.c_str(), &m_db) != SQLITE_OK) {
        m_db = nullptr;
        throw std::runtime_error("Failed to open DB: " + path);
    }
}

void DbConnection::close() noexcept {
    if (m_db) {
        sqlite3_close_v2(m_db);
        m_db = nullptr;
    }
}
