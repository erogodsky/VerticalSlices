#include "Source.h"

int main()
{
	/*
		Получение векторов проекций
	*/
	vector<cv::Mat> coronar, sagittal, axial;
	getDICOMProjections(coronar, sagittal, axial);

	int i = 0;
	while (i <= coronar.size())
	{
		cv::Mat dst1 = coronar[i];
		cv::Mat dst2 = sagittal[i];
		cv::Mat dst3 = axial[i];
		
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
		if (k == 255) //z
			i--;
		if (k == 247) //x
			i++;
		if (k == 27) //Esc
			break;
	}
	
	return 0;
}
