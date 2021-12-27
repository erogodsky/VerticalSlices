#pragma once

#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include "prepareImages.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmimage/diregist.h"

using namespace std;

vector<cv::Mat> prepareImages(vector<string> files);