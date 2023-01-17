#include "SQLite3Stmt.hpp"
#include "../ThirdParty/sqlite3/sqlite3.h"
#include <Siv3DSQL/SQLError.hpp>
#include <Siv3D.hpp>

namespace
{
    using namespace s3dsql;
    DBRow GetRow(sqlite3_stmt* stmt, int columnCount)
    {
        DBRow record;
        for (int columnIdx = 0; columnIdx < columnCount; ++columnIdx) {
            auto type = sqlite3_column_type(stmt, columnIdx);
            const auto* nameRaw = sqlite3_column_name(stmt, columnIdx);
            auto name = Unicode::Widen(nameRaw);

            switch (type) {
            case SQLITE_NULL:
                record.emplace(name, nullptr);
                break;
            case SQLITE_INTEGER:
                record.emplace(name, sqlite3_column_int64(stmt, columnIdx));
                break;
            case SQLITE_FLOAT:
                record.emplace(name, sqlite3_column_double(stmt, columnIdx));
                break;
            case SQLITE_TEXT:
            {
                auto* value = sqlite3_column_text(stmt, columnIdx);
                record.emplace(name, Unicode::Widen(reinterpret_cast<const char*>(value)));
            }
            break;
            case SQLITE_BLOB:
            {
                auto* value = sqlite3_column_blob(stmt, columnIdx);
                auto size = sqlite3_column_bytes(stmt, columnIdx);
                record.emplace(name, Blob(value, size));
            }
            break;
            default:
                record.emplace(name, nullptr);
                break;
            }
        }
        return record;
    }
}
namespace s3dsql
{
    SQLite3Stmt::SQLite3Stmt(sqlite3* const db):
        m_db(db)
    {}
    SQLite3Stmt::~SQLite3Stmt()
    {
        if (m_stmt != nullptr) {
            sqlite3_finalize(m_stmt);
        }
    }
    bool SQLite3Stmt::prepare(s3d::StringView sql)
    {
        auto result = sqlite3_prepare_v2(m_db, s3d::Unicode::Narrow(sql).c_str(), -1, &m_stmt, nullptr);
        if (result != SQLITE_OK || !m_stmt) {
            throw SQLError(U"Failed SQL : {}"_fmt(Unicode::Widen(sqlite3_errmsg(m_db))));
        }
        return true;
    }

    bool SQLite3Stmt::bind(s3d::int32 index, const DBValue& value) const
    {
        int result = 0;
        switch (value.getType()) {
        case DBValueType::Null:
            result = sqlite3_bind_null(m_stmt, index);
            break;
        case DBValueType::Int:
            result = sqlite3_bind_int64(m_stmt, index, value.get<s3d::int64>());
            break;
        case DBValueType::Float:
            result = sqlite3_bind_double(m_stmt, index, value.get<double>());
            break;
        case DBValueType::String:
            result = sqlite3_bind_text(m_stmt, index, Unicode::ToUTF8(value.get<s3d::String>()).c_str(), -1, SQLITE_TRANSIENT);
            break;
        case DBValueType::Blob:
        {
            auto&& blob = value.get<s3d::Blob>();
            result = sqlite3_bind_blob(m_stmt, index, blob.data(), static_cast<int>(blob.size()), SQLITE_TRANSIENT);
        }
            break;
        default:
            return false;
        }
        return result != SQLITE_OK;
    }
    bool SQLite3Stmt::bind(s3d::StringView name, const DBValue& value) const
    {
        auto index = sqlite3_bind_parameter_index(m_stmt, s3d::Unicode::Narrow(name).c_str());
        return this->bind(index, value);
    }
    s3d::int32 SQLite3Stmt::exec() const
    {
        if (auto error = sqlite3_step(m_stmt); error != SQLITE_DONE) {
            throw SQLError(U"Failed SQL : {}"_fmt(Unicode::Widen(sqlite3_errmsg(m_db))));
        }
        return sqlite3_changes(m_db);
    }
    s3d::Array<DBRow> SQLite3Stmt::fetch() const
    {
        s3d::Array<DBRow> result;
        int error = 0;
        int columnCount = sqlite3_column_count(m_stmt);
        while (SQLITE_ROW == (error = sqlite3_step(m_stmt))) {
            result.push_back(::GetRow(m_stmt, columnCount));
        }
        if (error != SQLITE_DONE) {
            throw SQLError(U"Failed SQL : {}"_fmt(Unicode::Widen(sqlite3_errmsg(m_db))));
        }
        return result;
    }
    s3d::Optional<DBRow> SQLite3Stmt::fetchOne() const
    {
        int error = 0;
        int columnCount = sqlite3_column_count(m_stmt);
        if (SQLITE_ROW == (error = sqlite3_step(m_stmt))) {
            return ::GetRow(m_stmt, columnCount);
        }
        if (error != SQLITE_DONE) {
            throw SQLError(U"Failed SQL : {}"_fmt(Unicode::Widen(sqlite3_errmsg(m_db))));
        }
        return s3d::none;
    }
}
