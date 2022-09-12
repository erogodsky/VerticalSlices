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
		Подготовка вектора для храниния "сырых" данных сагиттальной проекциb
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

	FindBraincaseSpacing(); // Поиск крайних точек черепной коробки

	/*
		Удаление лишних кадров в проекциях
	*/
	vector<cv::Mat> axialInter(axial_.interData.begin(), axial_.interData.begin() + braincase_.bottom);
	axial_.interData = axialInter;
	axial_.interData.shrink_to_fit();
	vector<cv::Mat> axialOut(axial_.outputData.begin(), axial_.outputData.begin() + braincase_.bottom);
	axial_.outputData = axialOut;
	axial_.outputData.shrink_to_fit();
	vector<cv::Mat> sagittalInter(sagittal_.interData.begin() + braincase_.left, sagittal_.interData.begin() + braincase_.right);
	sagittal_.interData = sagittalInter;
	sagittal_.interData.shrink_to_fit();

	/*
		Обрезка изображений в аксиальной проекции
	*/
	cv::Rect brainAxial(braincase_.left,
		braincase_.front,
		braincase_.right - braincase_.left,
		braincase_.back - braincase_.front);
	for (int i = 0; i < axial_.interData.size(); i++)
	{
		axial_.interData[i] = axial_.interData[i](brainAxial);
		axial_.outputData[i] = axial_.outputData[i](brainAxial);
	}

	/*
		Подготовка оставшихся векторов проекций
	*/
	int i = 0;
	while ((i < axial_.interData[0].rows) || (i < axial_.interData[0].cols))
	{
		if (i < axial_.interData[0].rows)
		{
			coronar_.interData.push_back(cv::Mat::zeros(axial_.interData.size(), axial_.interData[0].cols, CV_16SC1));
			coronar_.outputData.push_back(cv::Mat::zeros(axial_.outputData.size(), axial_.outputData[0].cols, CV_8UC1));
		}
		if (i < axial_.interData[0].cols)
		{
			sagittal_.outputData.push_back(cv::Mat::zeros(axial_.outputData.size(), axial_.outputData[0].rows, CV_8UC1));
		}
		i++;
	}
	
	/*
		Попиксельное аполнение оставшихся векторов проекций
	*/
	for (int i = 0; i < axial_.outputData[0].rows; i++)
	{
		//cout << i * 100. / axial_.outputData[0].rows << "% \n";

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
	i = 0;
	while ((i < coronar_.interData.size()) || (i < sagittal_.interData.size()))
	{
		if (i < coronar_.interData.size())
		{
			cv::resize(coronar_.interData[i], coronar_.interData[i], 
				cv::Size(coronar_.interData[i].cols, coronar_.interData[i].rows * resizeCoef), 0.0, resizeCoef);
			cv::resize(coronar_.outputData[i], coronar_.outputData[i],
				cv::Size(coronar_.outputData[i].cols, coronar_.outputData[i].rows * resizeCoef), 0.0, resizeCoef);
		}
		if (i < sagittal_.interData.size())
		{
			cv::resize(sagittal_.interData[i], sagittal_.interData[i], 
				cv::Size(sagittal_.interData[i].cols, sagittal_.interData[i].rows * resizeCoef), 0.0, resizeCoef);
			cv::resize(sagittal_.outputData[i], sagittal_.outputData[i],
				cv::Size(sagittal_.outputData[i].cols, sagittal_.outputData[i].rows * resizeCoef), 0.0, resizeCoef);
		}
		i++;
	}
}
