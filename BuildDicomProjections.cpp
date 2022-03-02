#include "DICOMImage.h"

/*
	Получение векторов проекций
*/
void DICOMImage::BuildDICOMProjections()
{
	short activePixelInter;
	uchar activePixelOut;

	FetchFileNames(); //Получение вектора имён файлов

	PrepareAxial(); //Получение вектора изображений в аксиальной проекции
	
	/*
		Подготовка вектора для храниния "сырых" данных сагиттальной проекций
	*/
	for (int i = 0; i < axial_.interData[0].cols; i++)
	{
		sagittal_.interData.push_back(cv::Mat::zeros(axial_.interData.size(), axial_.interData[0].rows, CV_16SC1));
	}

	/*
		Попиксельное заполнение вектора "сырых" данных сагиттальной проекции для
			определения по ней нижней точки мозга
	*/
	for (int i = 0; i < axial_.interData[0].rows; i++)
	{
		for (int j = 0; j < axial_.interData.size(); j++)
		{
			for (int k = 0; k < axial_.interData[0].cols; k++)
			{
				activePixelInter = axial_.interData[j].at<short>(cv::Point(axial_.interData[j].cols - k - 1, i));

				sagittal_.interData[axial_.interData[j].cols - k - 1].at<short>(cv::Point(i, j)) = activePixelInter;
			}
		}
	}

	int brainBottom = FindBrainBottom(); // Поиск нижней точки мозга

	/*
		Обрезка "сырых" изображений в сегиттальной проекции
		*/
	cv::Rect brain(0, 0, sagittal_.interData[0].cols, brainBottom);
	for (int i = 0; i < sagittal_.interData.size(); i++)
	{
		sagittal_.interData[i] = sagittal_.interData[i](brain);
	}

	/*
		Удаление лишних кадров в аксиальной проекции
	*/
	vector<cv::Mat> axialInter(axial_.interData.begin(), axial_.interData.begin() + brainBottom);
	axial_.interData = axialInter;
	axial_.interData.shrink_to_fit();
	vector<cv::Mat> axialOut(axial_.outputData.begin(), axial_.outputData.begin() + brainBottom);
	axial_.outputData = axialOut;
	axial_.outputData.shrink_to_fit();

	/*
		Подготовка оставшихся векторов проекций
	*/
	for (int i = 0; i < axial_.interData[0].cols; i++)
	{
		coronar_.interData.push_back(cv::Mat::zeros(axial_.interData.size(), axial_.interData[0].rows, CV_16SC1));
		coronar_.outputData.push_back(cv::Mat::zeros(axial_.outputData.size(), axial_.outputData[0].rows, CV_8UC1));

		sagittal_.outputData.push_back(cv::Mat::zeros(axial_.outputData.size(), axial_.outputData[0].rows, CV_8UC1));
	}
	
	/*
		Попиксельное аполнение оставшихся векторов проекций
	*/
	for (int i = 0; i < axial_.outputData[0].rows; i++)
	{
		cout << i * 100. / axial_.outputData[0].rows << "% \n";

		for (int j = 0; j < axial_.outputData.size(); j++)
		{
			for (int k = 0; k < axial_.outputData[0].cols; k++)
			{
				activePixelInter = axial_.interData[j].at<short>(cv::Point(axial_.interData[j].cols - k - 1, i));
				activePixelOut = axial_.outputData[j].at<uchar>(cv::Point(axial_.outputData[j].cols - k - 1, i));

				coronar_.interData[i].at<short>(cv::Point(k, j)) = activePixelInter;
				coronar_.outputData[i].at<uchar>(cv::Point(k, j)) = activePixelOut;

				sagittal_.outputData[axial_.outputData[j].cols - k - 1].at<uchar>(cv::Point(i, j)) = activePixelOut;
			}
		}
	}

	/*
		Растягивание изображений вдоль оси Y с 
			коэффициентом = (толщина среза + расстояние между срезами)/толщина среза
	*/
	float resizeCoef = GetResizeCoef();
	int i = 0;
	while ((i < axial_.interData[0].rows) || (i < axial_.interData[0].cols))
	{
		if (i < axial_.interData[0].rows)
		{
			cv::resize(coronar_.interData[i], coronar_.interData[i], 
				cv::Size(coronar_.interData[i].cols, coronar_.interData[i].rows * resizeCoef), 0.0, resizeCoef);
			cv::resize(coronar_.outputData[i], coronar_.outputData[i],
				cv::Size(coronar_.outputData[i].cols, coronar_.outputData[i].rows * resizeCoef), 0.0, resizeCoef);
		}
		if (i < axial_.interData[0].cols)
		{
			cv::resize(sagittal_.interData[i], sagittal_.interData[i], 
				cv::Size(sagittal_.interData[i].cols, sagittal_.interData[i].rows * resizeCoef), 0.0, resizeCoef);
			cv::resize(sagittal_.outputData[i], sagittal_.outputData[i],
				cv::Size(sagittal_.outputData[i].cols, sagittal_.outputData[i].rows * resizeCoef), 0.0, resizeCoef);
		}
		i++;
	}
}
