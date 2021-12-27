#pragma once

#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include "getResizeCoef.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmimage/diregist.h"

using namespace std;

float getResizeCoef(vector<string> files);