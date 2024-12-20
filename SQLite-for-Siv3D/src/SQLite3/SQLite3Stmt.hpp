﻿#pragma once
#include <Siv3D/StringView.hpp>
#include <Siv3D/Array.hpp>
#include <Siv3D/HashTable.hpp>

#include <Siv3DSQL/DBValue.hpp>

struct sqlite3_stmt;
struct sqlite3;

namespace s3dsql
{
    class SQLite3Stmt
    {
    public:
        SQLite3Stmt(sqlite3* const db);
        ~SQLite3Stmt();

        bool prepare(s3d::StringView sql);

        bool bind(s3d::int32 index, const DBValue& value) const;
        bool bind(s3d::StringView name, const DBValue& value) const;

        s3d::int32 exec() const;
        s3d::Array<DBRow> fetch() const;
        s3d::Optional<DBRow> fetchOne() const;
    private:
        sqlite3_stmt* m_stmt = nullptr;
        sqlite3* const m_db = nullptr;
        const char* m_tail = nullptr;
    };
}
