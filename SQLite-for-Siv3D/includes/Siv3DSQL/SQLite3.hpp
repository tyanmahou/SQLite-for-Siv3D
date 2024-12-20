﻿#pragma once
#include <memory>
#include <Siv3D/StringView.hpp>
#include <Siv3D/Array.hpp>
#include <Siv3D/HashTable.hpp>

#include "DBValue.hpp"
#include "SQLError.hpp"

namespace s3dsql
{
	/// <summary>
	/// SQLite3
	/// </summary>
	class SQLite3
	{
	public:
		class Transaction
		{
		private:
			const SQLite3& m_db;
			bool m_isCommit = false;
		public:
			Transaction(const SQLite3& db);
			~Transaction();

			void commit();
		};
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
		/// <returns>更新あった行数</returns>
		s3d::int32 exec(s3d::StringView sql) const;

		/// <summary>
		/// SQLの実行
		/// </summary>
		/// <param name="sql">SQL</param>
		/// <param name="values">プリペアドステートメントのパラメータ</param>
		/// <returns>更新あった行数</returns>
		s3d::int32 exec(s3d::StringView sql, const DBValueArray& values) const;

		/// <summary>
		/// SQLの実行
		/// </summary>
		/// <param name="sql">SQL</param>
		/// <param name="values">プリペアドステートメントのパラメータ</param>
		/// <returns>更新あった行数</returns>
		s3d::int32 exec(s3d::StringView sql, const DBValueMap& values) const;

		/// <summary>
		/// レコードの取得
		/// </summary>
		/// <param name="sql"></param>
		/// <returns></returns>
		[[nodiscard]] s3d::Array<DBRow> fetch(s3d::StringView sql) const;

		/// <summary>
		/// レコードの取得
		/// </summary>
		/// <param name="sql">SQL</param>
		/// <param name="values">プリペアドステートメントのパラメータ</param>
		/// <returns></returns>
		[[nodiscard]] s3d::Array<DBRow> fetch(s3d::StringView sql, const DBValueArray& values) const;

		/// <summary>
		/// レコードの取得
		/// </summary>
		/// <param name="sql">SQL</param>
		/// <param name="values">プリペアドステートメントのパラメータ</param>
		/// <returns></returns>
		[[nodiscard]] s3d::Array<DBRow> fetch(s3d::StringView sql, const DBValueMap& values) const;

		/// <summary>
		/// レコードを一つ取得
		/// </summary>
		/// <param name="sql"></param>
		/// <returns></returns>
		[[nodiscard]] s3d::Optional<DBRow> fetchOne(s3d::StringView sql) const;

		/// <summary>
		/// レコードを一つ取得
		/// </summary>
		/// <param name="sql">SQL</param>
		/// <param name="values">プリペアドステートメントのパラメータ</param>
		/// <returns></returns>
		[[nodiscard]] s3d::Optional<DBRow> fetchOne(s3d::StringView sql, const DBValueArray& values) const;

		/// <summary>
		/// レコードを一つ取得
		/// </summary>
		/// <param name="sql">SQL</param>
		/// <param name="values">プリペアドステートメントのパラメータ</param>
		/// <returns></returns>
		[[nodiscard]] s3d::Optional<DBRow> fetchOne(s3d::StringView sql, const DBValueMap& values) const;

		/// <summary>
		/// DBが開かれているか
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] bool isOpen() const;

		[[nodiscard]] explicit operator bool() const
		{
			return this->isOpen();
		}

		/// <summary>
		/// トランザクションをはる
		/// </summary>
		/// <returns></returns>
		Transaction beginTransaction() const;

	private:
		class Impl;
		std::shared_ptr<Impl> m_pImpl;
	};
}
