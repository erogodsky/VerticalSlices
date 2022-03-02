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

typedef struct
{
	vector<cv::Mat> interData;
	vector<cv::Mat> outputData;
} Projection;

class DICOMImage
{
public:
	DICOMImage(stdfs::path path) 
	{
		path_ = path;

		BuildDICOMProjections();
	}

	Projection GetSagittalProjection() { return sagittal_; }
	Projection GetCoronarProjection() { return coronar_; }
	Projection GetAxialProjections() { return axial_; }

private:
	void PrepareAxial();
	void FetchFileNames();
	float GetResizeCoef();
	void BuildDICOMProjections();
	int FindBrainBottom();

	vector<string> files_;
	stdfs::path path_;
	
	Projection sagittal_;
	Projection coronar_;
	Projection axial_;
};

#endif