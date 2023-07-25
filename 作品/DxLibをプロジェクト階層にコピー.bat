@echo off

rem バッチファイルが存在するディレクトリにDxLibをコピーする

rem /D 同名ファイルが存在した場合、日付が新しいファイルのみコピーする
rem /S サブフォルダーも対象にする
rem /E フォルダ内が空でもコピーする
rem /Q コピーするファイル名を表示しない
rem /I コピーするファイルを自動的にフォルダとしてコピー
rem /Y 上書き時の確認メッセージを表示しない
xcopy /D /S /E /Q /I /Y C:\DxLib_VC\プロジェクトに追加すべきファイル_VC用 DxLib