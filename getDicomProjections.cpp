#include "getDicomProjections.h"

void getDICOMProjections(vector<cv::Mat>& coronar, vector<cv::Mat>& sagittal, vector<cv::Mat>& axial)
{
	vector<string> files;

	files = getFileNames("Sitnikov\\ANONIM_MONOCHROME2"); //Получение вектора имён файлов

	axial.resize(files.size());
	axial = prepareImages(files); //Получение вектора изображений

	/*
		Подготовка векторов для храниния коронарной и сагиттальной проекций
	*/
	int i = 0;
	while ((i < axial[0].rows) || (i < axial[0].cols))
	{
		if (i < axial[0].rows)
		{
			coronar.push_back(cv::Mat::zeros(axial.size(), axial[0].cols, CV_16UC1));
		}
		if (i < axial[0].cols)
		{
			sagittal.push_back(cv::Mat::zeros(axial.size(), axial[0].rows, CV_16UC1));
		}
		i++;
	}

	/*
		Попиксельное заполнение векторов проекций
	*/
	unsigned short activePixel;
	for (int i = 0; i < axial[0].rows; i++)
	{
		cout << i * 100. / axial[0].rows << "% \n";

		for (int j = 0; j < axial.size(); j++)
		{
			for (int k = 0; k < axial[0].cols; k++)
			{
				activePixel = axial[j].at<unsigned short>(cv::Point(axial[j].cols - k - 1, i));

				coronar[i].at<unsigned short>(cv::Point(k, j)) = activePixel;

				sagittal[axial[j].cols - k - 1].at<unsigned short>(cv::Point(i, j)) = activePixel;
			}
		}

	}

	/*
		Растягивание изображений вдоль оси Y с 
			коэффициентом = (толщина среза + расстояние между срезами)/толщина среза
	*/
	float resizeCoef = getResizeCoef(files);
	i = 0;
	while ((i < axial[0].rows) || (i < axial[0].cols))
	{
		if (i < axial[0].rows)
		{
			cv::resize(coronar[i], coronar[i], cv::Size(coronar[i].cols, coronar[i].rows * resizeCoef), 0.0, resizeCoef);
		}
		if (i < axial[0].cols)
		{
			cv::resize(sagittal[i], sagittal[i], cv::Size(sagittal[i].cols, sagittal[i].rows * resizeCoef), 0.0, resizeCoef);
		}
		i++;
	}
}
