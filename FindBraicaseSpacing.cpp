#include "DICOMImage.h"

void DICOMImage::FindBraicaseSpacing()
{
	cv::Mat img;
	vector<vector<cv::Point>> topContours;
	bool leftReached = false, rightReached = false;

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
	vector<int> possibleSkullBottoms, possibleSkullBacks, possibleSkullFronts;
	vector<int> tops(topContours.size());
	vector<int> bottoms(topContours.size());
	for (int i=0; i < topContours.size(); i++)
	{
		vector<cv::Point> cnt = topContours[i];
		int top = (*min_element(cnt.begin(), cnt.end(),
			[](const cv::Point& p1, const cv::Point& p2) {
				return p1.y < p2.y;
			})).y;
		tops.push_back(top);
		if (top == 0)
		{
			int cntBottom = (*max_element(cnt.begin(), cnt.end(),
				[](const cv::Point& p1, const cv::Point& p2) {
					return p1.y < p2.y;
				})).y;
			possibleSkullBottoms.push_back(cntBottom);
			bottoms[i] = cntBottom;
		}
	}
    braincase_.bottom = *max_element(possibleSkullBottoms.begin(), possibleSkullBottoms.end());

	for (int i = 0; i < topContours.size(); i++)
	{
		vector<cv::Point> cnt = topContours[i];

		if (bottoms[i] > 0)
		{
			int front = (*min_element(cnt.begin(), cnt.end(),
				[](const cv::Point& p1, const cv::Point& p2) {
					return p1.x < p2.x;
				})).x;
			int back = (*max_element(cnt.begin(), cnt.end(),
				[](const cv::Point& p1, const cv::Point& p2) {
					return p1.x < p2.x;
				})).x;
			possibleSkullBacks.push_back(back);
			possibleSkullFronts.push_back(front);
		}
	}
	braincase_.front = *min_element(possibleSkullFronts.begin(), possibleSkullFronts.end());
	braincase_.back = *max_element(possibleSkullBacks.begin(), possibleSkullBacks.end());

	cv::Rect brainSagittal(braincase_.front,
		0,
		braincase_.back - braincase_.front,
		braincase_.bottom);
	for (int i = 0; i < sagittal_.interData.size(); i++)
	{
		sagittal_.interData[i] = sagittal_.interData[i](brainSagittal); // Обрезка "сырых" изображений в сагиттальной проекции
		
		img = sagittal_.interData[i];
		double minVal;
		double maxVal;
		cv::Point minLoc;
		cv::Point maxLoc;
		if (!leftReached)
		{
			cv::minMaxLoc(img, &minVal, &maxVal, &minLoc, &maxLoc);
			if (maxVal >= 1500)
			{
				braincase_.left = i;
				leftReached = true;
			}
		}
		else
		{
			if ((!rightReached) && (i > sagittal_.interData.size() / 2))
			{
				cv::minMaxLoc(img, &minVal, &maxVal, &minLoc, &maxLoc);
				if (maxVal < 1500)
				{
					braincase_.right = i;
					rightReached = true;
				}
			}
		}
	}
}