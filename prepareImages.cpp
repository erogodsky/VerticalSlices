#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include "getFileNames.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dctagkey.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmimage/diregist.h"
#include "dcmtk/dcmjpeg/djrplol.h"
#include "dcmtk/dcmjpeg/djdecode.h"
#include "prepareImages.h"
#pragma comment(lib, "ws2_32.lib")
#define _CRT_SECURE_NO_WARNINGS
#define OPENCV_TRAITS_ENABLE_DEPRECATED

using namespace std;

/*
	Функция создаёт упорядоченный вектор изображений
*/
vector<cv::Mat> prepareImages(vector<string> files)
{
	DJDecoderRegistration::registerCodecs();

	/*
		Ключ сортировки – тег DCM_SliceLocation, DcmTagKey(0x0020, 0x1041)
	*/
	auto dicomSlicesOrderSort = [](string const& f1, string const& f2) -> bool
	{
		DcmFileFormat fileformat1;
		Float64 sliceLocation1;
		OFCondition status1 = fileformat1.loadFile(f1.c_str());
		if (status1.good())
		{
			if (!fileformat1.getDataset()->findAndGetFloat64(DCM_SliceLocation, sliceLocation1).good())
			{
				cerr << "Error: cannot access Slice location!" << endl;
			}
		}
		else
			cerr << "Error: cannot read DICOM file (" << status1.text() << ")" << endl;

		DcmFileFormat fileformat2;
		Float64 sliceLocation2;
		OFCondition status2 = fileformat2.loadFile(f2.c_str());
		if (status2.good())
		{
			if (!fileformat2.getDataset()->findAndGetFloat64(DCM_SliceLocation, sliceLocation2).good())
			{
				cerr << "Error: cannot access Slice location!" << endl;
			}				
		}
		else
			cerr << "Error: cannot read DICOM file (" << status2.text() << ")" << endl;

		return  sliceLocation1 > sliceLocation2;
	};

	sort(files.begin(), files.end(), dicomSlicesOrderSort); //Сортировка файлов по порядку срезов

	/*
		Создание и заполнение вектора изображений типа cv::Mat
	*/
	vector<cv::Mat> DICOMs;
	for (int i = 0; i < files.size(); i++)
	{
		cv::Mat dst;

		/*DcmFileFormat fileformat;
		DJ_RPLossless params;
		fileformat.loadFile(files[i].c_str());
		fileformat.chooseRepresentation(EXS_LittleEndianImplicit, &params);*/
		DicomImage* image = new DicomImage (files[i].c_str());
		
		int nWidth = image->getWidth();
		int nHeight = image->getHeight();
		int depth = image->getDepth();

		image->setWindow(100, 400);
		if (image != NULL)
		{
			if (image->getStatus() == EIS_Normal)
			{
				Uint16* pixelData = (Uint16*)(image->getOutputData(16));
				if (pixelData != NULL)
				{
					dst = cv::Mat(nHeight, nWidth, CV_16UC1, pixelData);
				}
			}
			else
				cerr << "Error: cannot load DICOM image (" << DicomImage::getString(image->getStatus()) << ")" << endl;
		}
		DICOMs.push_back(dst);
	}

	return DICOMs;
}
