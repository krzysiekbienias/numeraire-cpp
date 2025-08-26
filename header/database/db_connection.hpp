#pragma once
#include <sqlite3.h>
#include <string>

/**
 * RAII wrapper for a sqlite3* connection.
 * Opens in ctor, closes in dtor. Non-copyable, movable.
 */
class DbConnection {
public:
    explicit DbConnection(const std::string& path);
    ~DbConnection();

    DbConnection(const DbConnection&) = delete;
    DbConnection& operator=(const DbConnection&) = delete;

    DbConnection(DbConnection&& other) noexcept;
    DbConnection& operator=(DbConnection&& other) noexcept;

    sqlite3* get() const noexcept { return m_db; }
    bool isOpen() const noexcept { return m_db != nullptr; }

private:
    void open(const std::string& path);
    void close() noexcept;

    sqlite3* m_db = nullptr;
};
