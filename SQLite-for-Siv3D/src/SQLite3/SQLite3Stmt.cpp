#include "SQLite3Stmt.hpp"

#include <Siv3D.hpp>

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
            LOG_FAIL(U"Failed Prepare Stmt : {}"_fmt(Unicode::Widen(sqlite3_errmsg(m_db))));
            return false;
        }
        return true;
    }
    bool SQLite3Stmt::bind(s3d::int32 index, const DBValue& value)
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
            auto&& blob = value.get<s3d::ByteArray>();
            result = sqlite3_bind_blob(m_stmt, index, blob.data(), static_cast<int>(blob.size()), SQLITE_TRANSIENT);
        }
            break;
        default:
            return false;
        }
        return result != SQLITE_OK;
    }
    bool SQLite3Stmt::bind(s3d::StringView name, const DBValue& value)
    {
        auto index = sqlite3_bind_parameter_index(m_stmt, s3d::Unicode::Narrow(name).c_str());
        return this->bind(index, value);
    }
    s3d::Array<DBRow> SQLite3Stmt::fetch()
    {
        s3d::Array<DBRow> result;
        int error = 0;
        int columnCount = sqlite3_column_count(m_stmt);
        while (SQLITE_ROW == (error = sqlite3_step(m_stmt))) {
            DBRow record;
            for (int columnIdx = 0; columnIdx < columnCount; ++columnIdx) {
                auto type = sqlite3_column_type(m_stmt, columnIdx);
                auto* nameRaw = sqlite3_column_name(m_stmt, columnIdx);
                auto name = Unicode::Widen(reinterpret_cast<const char*>(nameRaw));

                switch (type) {
                case SQLITE_NULL:
                    record.emplace(name, nullptr);
                    break;
                case SQLITE_INTEGER:
                    record.emplace(name, sqlite3_column_int64(m_stmt, columnIdx));
                    break;
                case SQLITE_FLOAT:
                    record.emplace(name, sqlite3_column_double(m_stmt, columnIdx));
                    break;
                case SQLITE_TEXT:
                {
                    auto* value = sqlite3_column_text(m_stmt, columnIdx);
                    record.emplace(name, Unicode::Widen(reinterpret_cast<const char*>(value)));
                }
                break;
                case SQLITE_BLOB:
                {
                    auto* value = sqlite3_column_blob(m_stmt, columnIdx);
                    auto size = sqlite3_column_bytes(m_stmt, columnIdx);
                    record.emplace(name, ByteArray(value, size));
                }
                break;
                default:
                    record.emplace(name, nullptr);
                    break;
                }
            }
            result.push_back(std::move(record));
        }
        if (error != SQLITE_DONE) {
            LOG_FAIL(U"Failed Fetch SQL : {}"_fmt(Unicode::Widen(sqlite3_errmsg(m_db))));
            return s3d::Array<DBRow>();
        }
        return result;
    }
}