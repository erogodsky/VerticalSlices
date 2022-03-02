#include "DICOMImage.h"

/*
	Функция создаёт упорядоченный вектор изображений
*/
void DICOMImage::PrepareAxial()
{
	DJDecoderRegistration::registerCodecs();

	sort(files_.begin(), files_.end(), dicomSlicesOrderSort); // Сортировка файлов по порядку срезов

	/*
		Создание и заполнение вектора изображений типа cv::Mat
	*/
	bool topReached = false; // Найдена макушка?
	for (int i = 0; i < files_.size(); i++)
	{
		cv::Mat dstInter, dstOutput;

		DicomImage* image = new DicomImage (files_[i].c_str());
		
		int nWidth = image->getWidth();
		int nHeight = image->getHeight();
		int depth = image->getDepth();

		image->setWindow(100, 400); 
		if (image != NULL)
		{
			if (image->getStatus() == EIS_Normal)
			{
				int nWidth = image->getWidth();
				int nHeight = image->getHeight();

				const DiPixel* dmp = image->getInterData(); // Получение "сырых данных" (12 бит) DICOM-изображения
				void* interData = NULL;
				interData = (void*)dmp->getData();

				Uint8* outputData = (Uint8*)(image->getOutputData(8)); // Получение данных (8 бит) после применения
																		// LUT с заданными параметрами окна 

				dstInter = cv::Mat(nHeight, nWidth, CV_16SC1, interData);

				double minVal;
				double maxVal;
				cv::Point minLoc;
				cv::Point maxLoc;
				cv::minMaxLoc(dstInter, &minVal, &maxVal, &minLoc, &maxLoc);

				if (topReached)
				{
					axial_.interData.push_back(dstInter);

					dstOutput = cv::Mat(nHeight, nWidth, CV_8UC1, outputData);
					axial_.outputData.push_back(dstOutput);
				}
				else 
				{
					if (maxVal > 1500) // Если при проходе сверху вниз макимальная яркость
									// данного изображения больше 1500, считаем, что макушка найдена
					{
						topReached = true;
					}
				}
			}
			else
				cerr << "Error: cannot load DICOM image (" << DicomImage::getString(image->getStatus()) << ")" << endl;
		}
	}
}
