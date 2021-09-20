#include <Siv3DSQL/SQLite3.hpp>
#include <Siv3DSQL/SQLError.hpp>
#include "../ThirdParty/sqlite3/sqlite3.h"
#include "SQLite3Stmt.hpp"
#include <Siv3D.hpp>
#include <Siv3D/EngineLog.hpp>

namespace s3dsql
{
    class SQLite3::Impl
    {
        sqlite3* m_db = nullptr;
    public:
        Impl()
        {}
        ~Impl()
        {
            this->release();
        }
        bool open(s3d::FilePathView path)
        {
            if (auto result = sqlite3_open16(s3d::Unicode::ToWstring(path).c_str(), &m_db); result != SQLITE_OK) {
                LOG_FAIL(U"Failed Open: {}"_fmt(path));
                return false;
            }
            return true;
        }

        void release()
        {
            if (m_db != nullptr) {
                sqlite3_close_v2(m_db);
            }
            m_db = nullptr;
        }
        s3d::int32 exec(s3d::StringView sql)
        {
            SQLite3Stmt stmt(m_db);
            stmt.prepare(sql);

            return stmt.exec();
        }
        s3d::int32 exec(s3d::StringView sql, const DBValueArray& values)
        {
            SQLite3Stmt stmt(m_db);
            stmt.prepare(sql);

            int32 index = 1; // bindは1スタート
            for (const auto& value : values) {
                stmt.bind(index, value);
                ++index;
            }
            return stmt.exec();
        }
        bool exec(s3d::StringView sql, const DBValueMap& values)
        {
            SQLite3Stmt stmt(m_db);
            stmt.prepare(sql);

            for (const auto& [name, value] : values) {
                stmt.bind(name, value);
            }
            return stmt.exec();
        }
        s3d::Array<DBRow> fetch(s3d::StringView sql) const
        {
            SQLite3Stmt stmt(m_db);
            stmt.prepare(sql);

            return stmt.fetch();
        }
        s3d::Array<DBRow> fetch(s3d::StringView sql, const DBValueArray& values) const
        {
            SQLite3Stmt stmt(m_db);
            stmt.prepare(sql);

            int32 index = 1; // bindは1スタート
            for (const auto& value : values) {
                stmt.bind(index, value);
                ++index;
            }
            return stmt.fetch();
        }
        s3d::Array<DBRow> fetch(s3d::StringView sql, const DBValueMap& values) const
        {
            SQLite3Stmt stmt(m_db);
            stmt.prepare(sql);

            for (const auto& [name, value] : values) {
                stmt.bind(name, value);
            }
            return stmt.fetch();
        }
        s3d::Optional<DBRow> fetchOne(s3d::StringView sql) const
        {
            SQLite3Stmt stmt(m_db);
            stmt.prepare(sql);

            return stmt.fetchOne();
        }
        s3d::Optional<DBRow> fetchOne(s3d::StringView sql, const DBValueArray& values) const
        {
            SQLite3Stmt stmt(m_db);
            stmt.prepare(sql);

            int32 index = 1; // bindは1スタート
            for (const auto& value : values) {
                stmt.bind(index, value);
                ++index;
            }
            return stmt.fetchOne();
        }
        s3d::Optional<DBRow> fetchOne(s3d::StringView sql, const DBValueMap& values) const
        {
            SQLite3Stmt stmt(m_db);
            stmt.prepare(sql);

            for (const auto& [name, value] : values) {
                stmt.bind(name, value);
            }
            return stmt.fetchOne();
        }
        bool isOpen()const
        {
            return m_db != nullptr;
        }
    };

    SQLite3::SQLite3() :
        m_pImpl(std::make_shared<Impl>())
    {}

    SQLite3::SQLite3(s3d::FilePathView path) :
        m_pImpl(std::make_shared<Impl>())
    {
        this->open(path);
    }

    bool SQLite3::open(s3d::FilePathView path) const
    {
        return m_pImpl->open(path);
    }

    void SQLite3::release() const
    {
        return m_pImpl->release();
    }

    s3d::int32 SQLite3::exec(s3d::StringView sql) const
    {
        return m_pImpl->exec(sql);
    }
    s3d::int32 SQLite3::exec(s3d::StringView sql, const DBValueArray& values) const
    {
        return m_pImpl->exec(sql, values);
    }
    s3d::int32 SQLite3::exec(s3d::StringView sql, const DBValueMap& values) const
    {
        return m_pImpl->exec(sql, values);
    }
    s3d::Array<DBRow> SQLite3::fetch(s3d::StringView sql) const
    {
        return m_pImpl->fetch(sql);
    }
    s3d::Array<DBRow> SQLite3::fetch(s3d::StringView sql, const DBValueArray& values) const
    {
        return m_pImpl->fetch(sql, values);
    }
    s3d::Array<DBRow> SQLite3::fetch(s3d::StringView sql, const DBValueMap& values) const
    {
        return m_pImpl->fetch(sql, values);
    }
    s3d::Optional<DBRow> SQLite3::fetchOne(s3d::StringView sql) const
    {
        return m_pImpl->fetchOne(sql);
    }
    s3d::Optional<DBRow> SQLite3::fetchOne(s3d::StringView sql, const DBValueArray& values) const
    {
        return m_pImpl->fetchOne(sql, values);
    }
    s3d::Optional<DBRow> SQLite3::fetchOne(s3d::StringView sql, const DBValueMap& values) const
    {
        return m_pImpl->fetchOne(sql, values);
    }
    bool SQLite3::isOpen() const
    {
        return m_pImpl->isOpen();
    }

    SQLite3::Transaction SQLite3::beginTransaction() const
    {
        return Transaction(*this);
    }

    SQLite3::Transaction::Transaction(const SQLite3& db) :
        m_db(db)
    {
        m_db.exec(U"BEGIN");
    }
    SQLite3::Transaction::~Transaction()
    {
        if (!m_isCommit) {
            try {
                m_db.exec(U"ROLLBACK");
            } catch (SQLError&) {

            }
        }
    }
    void SQLite3::Transaction::commit()
    {
        if (!m_isCommit) {
            m_db.exec(U"COMMIT");
            m_isCommit = true;
        }
    }
}