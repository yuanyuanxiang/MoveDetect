#include "functions.h"
#include <iostream>

int main()
{
	VideoCapture video(0);
	if(!video.isOpened())
		return -1;

	Mat fore, back, frame; // �洢ǰһ֡�͵�ǰ֡
	Line line(Point(320, 0), Point(320, 640));
	cvNamedWindow("���߼��", CV_WINDOW_NORMAL);
	while(1)
	{
		video >> frame;
		if (frame.empty())// ��֡�����쳣���
			continue;

		//����ǵ�һ֡����Ҫ��ʼ������ͼ��
		if(back.empty())
		{
			cvtColor(frame, back, CV_BGR2GRAY);
			back.convertTo(back, CV_32FC1);
		}
		else
		{
			Mat result = MoveDetect(frame, back, 0.5, line);
			imshow("���߼��", result);
			if (waitKey(40) == 27)// ��ԭ25fps��ʾ, ESC�˳�
				break;
		}
	}
	//���ٴ���
	destroyAllWindows();
	return 0;
}
