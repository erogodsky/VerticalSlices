#include "DICOMImage.h"

/*
	Создание вектора имён файлов исследования
*/
void DICOMImage::FetchFileNames()
{
	const stdfs::directory_iterator end{};
	for (stdfs::directory_iterator iter{ path_ }; iter != end; ++iter) 
		files_.push_back(string(iter->path().string()));
}