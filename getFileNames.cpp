#include <iostream>
#include <filesystem>

using namespace std;
namespace stdfs = std::filesystem;

/*
	Создание вектора имён файлов исследования
*/
vector<string> getFileNames(stdfs::path path = "ANONIM")
{
	vector<string> files;

	const stdfs::directory_iterator end{};
	for (stdfs::directory_iterator iter{ path }; iter != end; ++iter) 
		files.push_back(string(iter->path().string()));

	return files;
}
