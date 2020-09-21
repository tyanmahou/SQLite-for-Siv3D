#pragma once
#include <memory>
#include <Siv3D/StringView.hpp>

namespace s3dsql
{
	/// <summary>
	/// SQLite3
	/// </summary>
	class SQLite3
	{
		class Impl;
		std::shared_ptr<Impl> m_pImpl;
	public:
		SQLite3();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="path">ファイルパス</param>
		SQLite3(s3d::FilePathView path);

		/// <summary>
		/// ファイルを開く
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		bool open(s3d::FilePathView path) const;

		/// <summary>
		/// ファイルを閉じる
		/// </summary>
		void release() const;

		/// <summary>
		/// SQLの実行
		/// </summary>
		/// <param name="sql"></param>
		/// <returns></returns>
		bool exec(s3d::StringView sql) const;
	};
}