
# include <Siv3D.hpp> // OpenSiv3D v0.4.3
#include <Siv3DSQL.hpp>


void Main()
{
	using namespace s3dsql;

	SQLite3 db(U"test.db");

	Print << db.exec(U"Drop TABLE sample");
	Print << db.exec(U"CREATE TABLE sample(id integer primary key, worker text not null, age integer, place text)");
	Print << db.exec(U"insert into sample values(0,'IKD',30,'Tokyo')");
	while (System::Update())
	{

	}
}
