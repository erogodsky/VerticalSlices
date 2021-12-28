#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include "getFileNames.h"
#include "prepareImages.h"
#include "getResizeCoef.h"
#pragma comment(lib, "ws2_32.lib")
#define _CRT_SECURE_NO_WARNINGS
#define OPENCV_TRAITS_ENABLE_DEPRECATED

using namespace std;

void getDICOMProjections(vector<cv::Mat>& coronar, vector<cv::Mat>& sagittal, vector<cv::Mat>& axial)
{
	vector<string> files;

	files = getFileNames("ANONIM"); //Получение вектора имён файлов

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
			coronar.push_back(cv::Mat::zeros(axial.size(), axial[0].cols, CV_8UC3));
		}
		if (i < axial[0].cols)
		{
			sagittal.push_back(cv::Mat::zeros(axial.size(), axial[0].rows, CV_8UC3));
		}
		i++;
	}

	/*
		Попиксельное заполнение векторов проекций
	*/
	cv::Vec3b activePixel;
	for (int i = 0; i < axial[0].rows; i++)
	{
		cout << i * 100. / axial[0].rows << "% \n";

		for (int j = 0; j < axial.size(); j++)
		{
			for (int k = 0; k < axial[0].cols; k++)
			{
				activePixel = axial[j].at<cv::Vec3b>(cv::Point(axial[j].cols - k - 1, i));;

				coronar[i].at<cv::Vec3b>(cv::Point(k, j)) = activePixel;

				sagittal[axial[j].cols - k - 1].at<cv::Vec3b>(cv::Point(i, j)) = activePixel;
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
