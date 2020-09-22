
# include <Siv3D.hpp> // OpenSiv3D v0.4.3
#include <Siv3DSQL.hpp>


void Main()
{
	using namespace s3dsql;
	// DBロード (なければ生成)
	SQLite3 db(U"test.db");

	// テーブル削除
	{
		StringView sql = U"DROP TABLE IF EXISTS users;";
		db.exec(sql);
	}
	// テーブル生成
	{
		StringView sql = U""
			"CREATE TABLE IF NOT EXISTS users("
			"  `user_id` INTEGER PRIMARY KEY,"
			"  `name` TEXT,"
			"  `age` INTEGER"
			");"
			;
		db.exec(sql);
	}
	// 挿入
	{
		StringView sql = U""
			"INSERT INTO users(user_id, name, age)"
			"VALUES"
			"  (0, 'taro', 10),"
			"  (1, 'hanako', 15);";
		db.exec(sql);
	}
	// 挿入
	{
		DBValueArray values;
		values.emplace_back(4);
		values.emplace_back(String(U"mahou"));
		values.emplace_back(24);


		StringView sql = U""
			"INSERT INTO users(user_id, name, age)"
			"VALUES"
			"  (?, ?, ?);";
		db.exec(sql, values);
	}
	{
		DBValueMap values;
		values[U":userId"] = 6;
		values[U":name"] = U"mahou3";

		StringView sql = U""
			"INSERT INTO users(user_id, name, age)"
			"VALUES"
			"  (:userId, :name, :userId)";
		db.exec(sql, values);
	}
	// 取得
	for (auto&& row : db.fetch(U"SELECT * FROM users")) {
		Print << row[U"user_id"].get<int32>() 
			<< U" " << row[U"name"].get<String>()
			<< U" " << row[U"age"].get<int32>()
			;
	}
	// 取得(pareped statement)
	{
		DBValueArray values;
		values.emplace_back(1);

		for (auto&& row : db.fetch(U"SELECT * FROM users WHERE user_id = ?", values)) {
			Print << row[U"user_id"].get<int32>()
				<< U" " << row[U"name"].get<String>()
				<< U" " << row[U"age"].get<int32>()
				;
		}
	}
	// 取得(pareped statement by name)
	{
		DBValueMap values;
		values[U":id"] = 0;
		values[U":age"] = 10;

		for (auto&& row : db.fetch(U"SELECT * FROM users WHERE user_id = :id AND age = :age", values)) {
			Print << row[U"user_id"].get<int32>()
				<< U" " << row[U"name"].get<String>()
				<< U" " << row[U"age"].get<int32>()
				;
		}
	}
	while (System::Update())
	{

	}
}
