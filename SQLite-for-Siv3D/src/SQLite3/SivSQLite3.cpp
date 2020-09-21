#include <Siv3DSQL/SQLite3.hpp>
#include "../ThirdParty/sqlite3/sqlite3.h"

#include <Siv3D.hpp>

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
			if (auto result = sqlite3_open16(s3d::Unicode::ToWString(path).c_str(), &m_db); result != SQLITE_OK) {
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
		bool exec(s3d::StringView sql)
		{
			char* errorMsg = nullptr;
			if (auto result = sqlite3_exec(m_db, s3d::Unicode::Narrow(sql).c_str(), nullptr, nullptr, &errorMsg); result != SQLITE_OK) {
				LOG_FAIL(U"Failed Exec SQL : {}"_fmt(Unicode::Widen(errorMsg)));
				return false;
			}
			return true;
		}
	};

	SQLite3::SQLite3():
		m_pImpl(std::make_shared<Impl>())
	{}

	SQLite3::SQLite3(s3d::FilePathView path):
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

	bool SQLite3::exec(s3d::StringView sql) const
	{
		return m_pImpl->exec(sql);
	}
}