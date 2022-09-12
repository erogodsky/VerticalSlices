#pragma once
#pragma comment(lib, "ws2_32.lib")

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

#include "pcl/visualization/cloud_viewer.h"

namespace stdfs = std::filesystem;

struct Projection
{
	std::vector<cv::Mat> interData;
	std::vector<cv::Mat> outputData;
};

struct BodyPartSpacing
{
	int top;
	int bottom;
	int left;
	int right;
	int front;
	int back;
};

class DICOMImage
{
public:
	DICOMImage(stdfs::path path): path_{ path }
	{
		BuildDICOMProjections();
	}

	Projection GetSagittalProjection() { return sagittal_; }
	Projection GetCoronarProjection() { return coronar_; }
	Projection GetAxialProjection() { return axial_; }

	void GenerateVesselMap();

private:
	void PrepareAxial();
	void FetchFileNames();
	float GetResizeCoef();
	void BuildDICOMProjections();
	void FindBraincaseSpacing();

	std::vector<string> files_;
	stdfs::path path_;

	Projection sagittal_;
	Projection coronar_;
	Projection axial_;

	std::vector<cv::Mat> vesselMap_;

	BodyPartSpacing braincase_;
};

