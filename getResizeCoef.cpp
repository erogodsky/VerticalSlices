#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include "getResizeCoef.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dctagkey.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmimage/diregist.h"
#pragma comment(lib, "ws2_32.lib")
#define _CRT_SECURE_NO_WARNINGS
#define OPENCV_TRAITS_ENABLE_DEPRECATED

/*
	Растягивание изображений вдоль оси Y с
		коэффициентом = (толщина среза + расстояние между срезами)/толщина среза
*/
float getResizeCoef(vector<string> files)
{
	float resizeCoef;

	string file = files[5];
	DcmFileFormat fileformat1;
	Float64 sliceThickness;
	Float64 spacingBetweenSlices;
	OFCondition status = fileformat1.loadFile(file.c_str());
	if (status.good())
	{
		if (!fileformat1.getDataset()->findAndGetFloat64(DCM_SliceThickness, sliceThickness).good())
		{
			cerr << "Error: cannot access Slice thickness!" << endl;
		}
		if (!fileformat1.getDataset()->findAndGetFloat64(DCM_SpacingBetweenSlices, spacingBetweenSlices).good())
		{
			spacingBetweenSlices = 0.6;
			//cerr << "Error: cannot access Spacing between slices!" << endl;
		}
	}
	else
		cerr << "Error: cannot read DICOM file (" << status.text() << ")" << endl;

	resizeCoef = (sliceThickness + spacingBetweenSlices)/sliceThickness - 1./files.size();

	return resizeCoef;
}
