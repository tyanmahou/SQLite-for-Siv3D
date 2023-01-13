#pragma once
#include <Siv3D/Error.hpp>

namespace s3dsql
{
	class SQLError : public s3d::Error
	{
	public:

		explicit SQLError(s3d::StringView what)
			: Error(what)
		{}
	};
}
