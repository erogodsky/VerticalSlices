#ifndef DICOM
#define DICOM

#define _CRT_SECURE_NO_WARNINGS
#define OPENCV_TRAITS_ENABLE_DEPRECATED

#include "Sortings.h"
#include <iostream>
#include <filesystem>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#pragma comment(lib, "ws2_32.lib")

#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmdata/dctagkey.h"
#include "dcmtk/dcmimage/diregist.h"
#include "dcmtk/dcmjpeg/djdecode.h"

using namespace std;
namespace stdfs = std::filesystem;

class DICOMImage
{
public:
	DICOMImage(stdfs::path path) 
	{
		path_ = path;

		/*
			Получение векторов проекций
		*/
		getDICOMProjections();
	}

	vector<cv::Mat> GetSagittalProjection() { return sagittal_; }
	vector<cv::Mat> GetCoronarProjection() { return coronar_; }
	vector<cv::Mat> GetAxialProjections() { return axial_; }

private:
	void prepareImages();
	void getFileNames();
	float getResizeCoef();
	void getDICOMProjections();

	vector<string> files_;
	stdfs::path path_;
	vector<cv::Mat> sagittal_;
	vector<cv::Mat> coronar_;
	vector<cv::Mat> axial_;
};

#endif