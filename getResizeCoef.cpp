//#include "getResizeCoef.h"
#include "DICOMImage.h"

/*
	Растягивание изображений вдоль оси Y с
		коэффициентом K = ((T + S - 1/s)/T)*P,
		где s - количество слоёв в аксиальной проекции;
			T - толщина среза в аксиальной проекции;
			S - расстояние между срезами в аксиальной проекции;
			P - размер пикселя.
*/
float DICOMImage::getResizeCoef()
{
	float resizeCoef;

	string file = files_[5];
	DcmFileFormat fileformat1;
	Float64 sliceThickness;
	Float64 pixelSpacing;
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
		if (!fileformat1.getDataset()->findAndGetFloat64(DCM_PixelSpacing, pixelSpacing).good())
		{
			cerr << "Error: cannot access Pixel spacing!" << endl;
		}
	}
	else
		cerr << "Error: cannot read DICOM file (" << status.text() << ")" << endl;

	resizeCoef = ((sliceThickness + spacingBetweenSlices)/sliceThickness - 1./files_.size()) * pixelSpacing;

	return resizeCoef;
}
