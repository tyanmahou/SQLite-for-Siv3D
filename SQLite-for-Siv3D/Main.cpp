
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
			"  (1, 'hanako', 15);"
			"INSERT INTO users(user_id, name, age)"
			"VALUES"
			"  (2, 'taaaro', 10),"
			"  (3, 'hanaako', 15);";
		db.exec(sql);
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

		for (auto&& row : db.fetch(U"SELECT * FROM users WHERE user_id = :id", values)) {
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
