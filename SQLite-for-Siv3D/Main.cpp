
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
            "  `name` TEXT NOT NULL,"
            "  `age` INTEGER"
            ");"
            ;
        db.exec(sql);
    }

    try {
        // トランザクション開始
        auto transaction = db.beginTransaction();
        // 挿入
        {
            StringView sql = U""
                "INSERT INTO users(user_id, name, age)"
                "VALUES"
                "  (?, ?, ?)";
            db.exec(sql, { 0, U"taro", 10 });
        }
        // コミット
        transaction.commit();
    } catch (SQLError& error) {
        Print << error.what();
    }
    // 取得
    {
        DBValueMap values;
        values[U":userId"] = 0;

        for (auto&& row : db.fetch(U"SELECT * FROM users WHERE user_id = :userId", values)) {
            Print << row[U"user_id"].get<int32>()
                << U" " << row[U"name"].get<String>()
                << U" " << row[U"age"].get<int32>()
                ;
        }
    }

    while (System::Update()) {
    }
}
