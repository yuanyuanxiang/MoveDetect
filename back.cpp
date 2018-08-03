#include "functions.h"
#include <iostream>

int main()
{
	VideoCapture video(0);
	if(!video.isOpened())
		return -1;

	Mat fore, back, frame; // 存储前一帧和当前帧
	Line line(Point(320, 0), Point(320, 640));
	cvNamedWindow("拌线检测", CV_WINDOW_NORMAL);
	while(1)
	{
		video >> frame;
		if (frame.empty())// 对帧进行异常检测
			continue;

		//如果是第一帧，需要初始化背景图像
		if(back.empty())
		{
			cvtColor(frame, back, CV_BGR2GRAY);
			back.convertTo(back, CV_32FC1);
		}
		else
		{
			Mat result = MoveDetect(frame, back, 0.5, line);
			imshow("拌线检测", result);
			if (waitKey(40) == 27)// 按原25fps显示, ESC退出
				break;
		}
	}
	//销毁窗口
	destroyAllWindows();
	return 0;
}
