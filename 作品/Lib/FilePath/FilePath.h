#pragma once
#include <string>
namespace MyLib
{
	class FilePath :public std::string
	{
	private:
		typedef std::string string;
	public:
		FilePath() {}
		FilePath(const string& str) : string(str) {}
		FilePath(const char* str) : string(str) {}
		~FilePath() {}

		//ファイル拡張子を取得
		string GetFileExtension() {
			size_t index = rfind('.');

			return  (index != string::npos) ? substr(index) : string();
		}
		//ファイル階層を取得
		string GetFileHierarchy() {
			size_t index = rfind('/');

			return (index != string::npos) ? substr(0, index) : string();
		}
	};
}