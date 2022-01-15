#pragma once

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

void getDICOMProjections(vector<cv::Mat>& coronar, vector<cv::Mat>& sagittal, vector<cv::Mat>& axial);