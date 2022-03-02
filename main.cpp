#include "DICOMImage.h"

int main()
{
	DICOMImage DICOMImage("Sitnikov\\ANONIM_MONOCHROME2");

	/*cv::FileStorage fsSagittal("C:/Kursovaya 3 kurs/Brain CT and MRI/Sitnikov/sagittal.xml", cv::FileStorage::WRITE);
	cv::FileStorage fsCoronar("C:/Kursovaya 3 kurs/Brain CT and MRI/Sitnikov/coronar.xml", cv::FileStorage::WRITE);
	for (int i = 0; i < 512; i++)
	{
		fsSagittal << "_" + to_string(i + 1) << sagittal[i];
		fsCoronar << "_" + to_string(i + 1) << coronar[i];
	}
	fsSagittal.release();
	fsCoronar.release();*/

	/*for (int i = 0; i < 512; i++)
	{
		vector<int> params;
		params.push_back(cv::IMWRITE_PNG_COMPRESSION);
		params.push_back(0);
		cv::imwrite("C:/Kursovaya 3 kurs/Brain CT and MRI/Sitnikov/sagittal/" + to_string(i + 1) + ".png", sagittal[i], params);
		cv::imwrite("C:/Kursovaya 3 kurs/Brain CT and MRI/Sitnikov/coronar/" + to_string(i + 1) + ".png", coronar[i], params);
	}*/

	int i = 0;
	while (1)
	{
		cv::Mat dst1 = DICOMImage.GetCoronarProjection().outputData[i];
		//dst1.convertTo(dst1, CV_16UC1);
		cv::Mat dst2 = DICOMImage.GetSagittalProjection().outputData[i];
		//dst2.convertTo(dst2, CV_16UC1);
		cv::Mat dst3 = DICOMImage.GetAxialProjections().outputData[i];
		//dst3.convertTo(dst3, CV_16UC1);
		
		cv::imshow("coronar", dst1);
		cv::imshow("sagittal", dst2);
		cv::imshow("axial", dst3);

		/*
			Перемещение между срезами (русская раскладка):
			z - назад;
			x - вперёд;
			Esc - выход
		*/
		int k = cv::waitKey(0);
		if (k == 255 && i!=0) //z
			i--;
		if (k == 247 && i < DICOMImage.GetAxialProjections().outputData.size()-1) //x
			i++;
		if (k == 27) //Esc
			break;
	}
	
	return 0;
}
