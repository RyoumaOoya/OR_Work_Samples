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

		//�t�@�C���g���q���擾
		string GetFileExtension() {
			size_t index = rfind('.');

			return  (index != string::npos) ? substr(index) : string();
		}
		//�t�@�C���K�w���擾
		string GetFileHierarchy() {
			size_t index = rfind('/');

			return (index != string::npos) ? substr(0, index) : string();
		}
	};
}