#pragma once
#include <Siv3D/StringView.hpp>
#include <Siv3D/Array.hpp>
#include <Siv3D/HashTable.hpp>

#include <Siv3DSQL/DBValue.hpp>

#include "../ThirdParty/sqlite3/sqlite3.h"
namespace s3dsql
{
    class SQLite3Stmt
    {
        sqlite3_stmt* m_stmt = nullptr;
        sqlite3*const m_db = nullptr;
    public:
        SQLite3Stmt(sqlite3*const db);
        ~SQLite3Stmt();

        bool prepare(s3d::StringView sql);

        bool bind(s3d::int32 index, const DBValue& value);
        bool bind(s3d::StringView name, const DBValue& value);

        s3d::Array<DBRow> fetch();
    };
}