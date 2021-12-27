#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include "getDicomProjections.h"

using namespace std;

void getDICOMProjections(vector<cv::Mat>& coronar, vector<cv::Mat>& sagittal, vector<cv::Mat>& axial);