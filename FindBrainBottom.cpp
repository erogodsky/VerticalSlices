#include "DICOMImage.h"

int DICOMImage::FindBrainBottom()
{
	cv::Mat img;
	vector<vector<cv::Point>> topContours;

	/*
		Поиск контуров костей на каждом срезе сагиттальной проекции. 
		Из каждого среза выбирается контур с наименьшей координатой Y верхнего экстремума.
	*/
	for (int i = 0; i < sagittal_.interData.size(); i++)
	{
		img = sagittal_.interData[i];

		cv::Mat mask;
		cv::inRange(img, cv::Scalar(600), cv::Scalar(5000), mask);

		vector<vector<cv::Point>> contours;
		cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		topContours.push_back(*min_element(contours.begin(), contours.end(), contoursComparison));
	}

	/*
		Выбор контуров с верхней точкой, лежащей на найденной макушке в аксиальной проекции.
		Для каждого находится нижний экстремум.
		Точка отсечения выбирается как минимальное значение координаты Y среди найденных экстремумов.
	*/
	vector<int> possibleSkullBottoms;
	for (auto cnt : topContours)
	{
		cv::Point top = *min_element(cnt.begin(), cnt.end(),
			[](const cv::Point& p1, const cv::Point& p2) {
				return p1.y < p2.y;
			});
		if (top.y == 0)
		{
			int cntBottom = (*max_element(cnt.begin(), cnt.end(),
				[](const cv::Point& p1, const cv::Point& p2) {
					return p1.y < p2.y;
				})).y;
			possibleSkullBottoms.push_back(cntBottom);
		}
	}
    int skullContourBottom = *max_element(possibleSkullBottoms.begin(), possibleSkullBottoms.end());

	return skullContourBottom;
}