#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>

#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcfilefo.h"

using namespace std;

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

/*
	Сравнение контуров по координате Y верхнего экстремума
*/
auto contoursComparison = [](vector<cv::Point>& cnt1, vector<cv::Point>& cnt2)
{
	cv::Point extTop1 = *min_element(cnt1.begin(), cnt1.end(),
		[](const cv::Point& p1, const cv::Point& p2)
		{
			return p1.y < p2.y;
		});
	cv::Point extTop2 = *min_element(cnt2.begin(), cnt2.end(),
		[](const cv::Point& p1, const cv::Point& p2)
		{
			return p1.y < p2.y;
		});
	return extTop1.y < extTop2.y;
};