#include "DICOMImage.h"

void DICOMImage::GenerateVesselMap()
{
	cv::Mat img;
	vector<vector<cv::Point>> contours;
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr pointCloud(new pcl::PointCloud<pcl::PointXYZRGB>());
	pcl::PointXYZRGB point;

	for (int k = 0; k < sagittal_.interData.size(); k++)
	{
		sagittal_.interData[k].copyTo(img);

		cv::Mat binary;
		cv::threshold(img, binary, 150, 5000, cv::THRESH_BINARY);
		binary.convertTo(binary, CV_8UC1);

		vector<cv::Vec4i> hierarchy;
		cv::findContours(binary, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

		binary.convertTo(binary, CV_32F);
		cv::Mat mask(binary.rows, binary.cols, CV_32F, cv::Scalar(0.f));
		for (int i = 0; i < contours.size(); i++)
		{
			if ((cv::arcLength(contours[i], true) > 30) && (hierarchy[i][3] == -1))
			{
				int child = hierarchy[i][2];
				bool vesselsFound = false;
				bool bigChildFound = false;
				if ((cv::contourArea(contours[i]) > 500) &&
					(child != -1) &&
					(!vesselsFound))
				{
					if (hierarchy[child][2] != -1)
					{
						vesselsFound = true;
						if (cv::contourArea(contours[child]) > 300)
						{
							bigChildFound = true;
						}
					}
					else
					{
						int neighbour = hierarchy[child][0];
						while (neighbour != -1)
						{
							if (hierarchy[neighbour][2] != -1)
							{
								vesselsFound = true;
								if (cv::contourArea(contours[neighbour]) > 200)
								{
									bigChildFound = true;
								}
								break;
							}
							neighbour = hierarchy[neighbour][0];
						}
						if (!vesselsFound)
						{
							neighbour = hierarchy[child][1];
							while (neighbour != -1)
							{
								if (hierarchy[neighbour][2] != -1)
								{
									vesselsFound = true;
									if (cv::contourArea(contours[neighbour]) > 200)
									{
										bigChildFound = true;
									}
									break;
								}
								neighbour = hierarchy[neighbour][1];
							}
						}
					}
					if ((vesselsFound) && (bigChildFound))
					{
						cv::drawContours(mask, contours, i, cv::Scalar(1.f), -1);
						cv::drawContours(mask, contours, i, cv::Scalar(1.f), 1);
						binary = binary.mul(mask);
					}
				}
				cv::drawContours(binary, contours, i, cv::Scalar(0.f), -1, cv::LINE_8, hierarchy, 1);
				cv::drawContours(binary, contours, i, cv::Scalar(0.f), 1);
			}
		}
		for (int i = 0; i < img.rows; i++)
		{
			for (int j = 0; j < img.cols; j++)
			{
				if (binary.at<float>(i, j) > 0)
				{
					point.x = j - int(img.cols / 2);
					point.y = -i + int(img.rows / 2);
					point.z = k - int(sagittal_.interData.size() / 2);
					point.r = 255;
					point.g = 255;
					point.b = 255;
					pointCloud->points.push_back(point);
				}
			}
		}
	}
	pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer("Vessel map"));
	viewer->setBackgroundColor(0, 0, 0);
	pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb(pointCloud);
	viewer->addPointCloud<pcl::PointXYZRGB>(pointCloud, rgb, "Vessels");
	viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "Vessels");
	viewer->addCoordinateSystem(1.0);
	viewer->initCameraParameters();
	while (!viewer->wasStopped())
	{
		viewer->spinOnce(100);
		std::this_thread::sleep_for(100ms);
	}
}