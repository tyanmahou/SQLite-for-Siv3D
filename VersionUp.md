# バージョンアップの手順


- 既存プロジェクトの名前を変える
- 新規プロジェクトSQLite-for-Siv3D追加
  - ソースファイルを移動

- 構成の種類を `.lib` に
- インクルードディレクトリィに追加
```
$(ProjectDir)\includes
```

- ビルド後イベント設定
```
xcopy /I /D /Y "$(OutDir)$(TargetFileName)" "$(ProjectDir)lib"
```
- [Debug]のターゲット名を `$(ProjectName)_d` に修正

- ビルドオプション追加
```
/source-charset:utf-8 
```

- ビルド構成 Test をDebugベースで追加
  - プリプロセッサ追加
```
S3DSQL_TEST
```

- Main.cpp をビルド対象から除外
  - Testは除外しない

- sqlite3
  - プリコンパイル済みヘッダーは使用しないように
  - stdafx.hはインクルードしない
    - 親の設定を継承しない