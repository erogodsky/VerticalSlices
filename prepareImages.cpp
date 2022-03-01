//#include "prepareImages.h"
#include "DICOMImage.h"

/*
	Функция создаёт упорядоченный вектор изображений
*/
void DICOMImage::prepareImages()
{
	DJDecoderRegistration::registerCodecs();

	sort(files_.begin(), files_.end(), dicomSlicesOrderSort); //Сортировка файлов по порядку срезов

	/*
		Создание и заполнение вектора изображений типа cv::Mat
	*/
	////////////////////////////
	ofstream myfile;
	myfile.open("densities_OutputData.txt");
	////////////////////////////
	bool topReached = false;
	for (int i = 0; i < files_.size(); i++)
	{
		cv::Mat dst;

		DicomImage* image = new DicomImage (files_[i].c_str());
		
		int nWidth = image->getWidth();
		int nHeight = image->getHeight();
		int depth = image->getDepth();

		//image->setWindow(300, 400);
		if (image != NULL)
		{
			if (image->getStatus() == EIS_Normal)
			{
				int nWidth = image->getWidth();
				int nHeight = image->getHeight();

				const DiPixel* dmp = image->getInterData();
				void* pixelData = NULL;
				pixelData = (void*)dmp->getData();

				//Uint16* pixelData = (Uint16*)(image->getOutputData(16));

				if (pixelData != NULL)
				{
					dst = cv::Mat(nHeight, nWidth, CV_16SC1, pixelData);

					double minVal;
					double maxVal;
					cv::Point minLoc;
					cv::Point maxLoc;
					cv::minMaxLoc(dst, &minVal, &maxVal, &minLoc, &maxLoc);
					if (topReached)
					{
						axial_.push_back(dst);
					}
					else 
					{
						if (maxVal > 1500)
						{
							topReached = true;
						}
					}
				}
				/*const DiPixel* dmp = image->getInterData();
				void* interPixelData = NULL;
				interPixelData = (void*)dmp->getData();
				int nWidth = image->getWidth();
				int nHeight = image->getHeight();
				cv::Mat density_avg(nHeight, nWidth, CV_16SC1, interPixelData);
				float avg_dens = cv::mean(density_avg).val[0];
				myfile << i + 1 << "    " << avg_dens << endl;*/


			}
			else
				cerr << "Error: cannot load DICOM image (" << DicomImage::getString(image->getStatus()) << ")" << endl;
		}
	}

	myfile.close();
}
