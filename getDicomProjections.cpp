//#include "getDicomProjections.h"
#include "DICOMImage.h"

void DICOMImage::getDICOMProjections()
{
	getFileNames(); //Получение вектора имён файлов

	prepareImages(); //Получение вектора изображений
	
	/*
		Подготовка векторов для храниния коронарной и сагиттальной проекций
	*/
	int i = 0;
	while ((i < axial_[0].rows) || (i < axial_[0].cols))
	{
		if (i < axial_[0].rows)
		{
			coronar_.push_back(cv::Mat::zeros(axial_.size(), axial_[0].cols, CV_16SC1));
		}
		if (i < axial_[0].cols)
		{
			sagittal_.push_back(cv::Mat::zeros(axial_.size(), axial_[0].rows, CV_16SC1));
		}
		i++;
	}

	/*double minVal;
	double maxVal;
	cv::Point minLoc;
	cv::Point maxLoc; 

	cv::minMaxLoc(axial_[0], &minVal, &maxVal, &minLoc, &maxLoc);

	cout << "min val: " << minVal << endl;
	cout << "max val: " << maxVal << endl;
	cout << "min val loc: " << minLoc << endl;
	cout << "max val loc: " << maxLoc << endl;
	cout << axial_[0].at<short>(maxLoc.y, maxLoc.x) << endl << endl;*/

	/*
		Попиксельное заполнение векторов проекций
	*/
	short activePixel;
	for (int i = 0; i < axial_[0].rows; i++)
	{
		cout << i * 100. / axial_[0].rows << "% \n";

		for (int j = 0; j < axial_.size(); j++)
		{
			for (int k = 0; k < axial_[0].cols; k++)
			{
				activePixel = axial_[j].at<short>(cv::Point(axial_[j].cols - k - 1, i));

				coronar_[i].at<short>(cv::Point(k, j)) = activePixel;

				sagittal_[axial_[j].cols - k - 1].at<short>(cv::Point(i, j)) = activePixel;
			}
		}

	}

	/*
		Растягивание изображений вдоль оси Y с 
			коэффициентом = (толщина среза + расстояние между срезами)/толщина среза
	*/
	float resizeCoef = getResizeCoef();
	i = 0;
	while ((i < axial_[0].rows) || (i < axial_[0].cols))
	{
		if (i < axial_[0].rows)
		{
			cv::resize(coronar_[i], coronar_[i], cv::Size(coronar_[i].cols, coronar_[i].rows * resizeCoef), 0.0, resizeCoef);
		}
		if (i < axial_[0].cols)
		{
			cv::resize(sagittal_[i], sagittal_[i], cv::Size(sagittal_[i].cols, sagittal_[i].rows * resizeCoef), 0.0, resizeCoef);
		}
		i++;
	}
}
