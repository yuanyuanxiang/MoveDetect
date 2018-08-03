/* 运动物体检测——帧差法
* C++利用帧差法背景差分实现运动目标检测（opencv）
* https://blog.csdn.net/xiao__run/article/details/76849908
*/

#include "functions.h"
#include <iostream>

int main()
{
	VideoCapture video(0);
	if(!video.isOpened())
		return -1;

	Mat temp, frame; // 存储前一帧和当前帧
	Line line(Point(320, 0), Point(320, 640));
	cvNamedWindow("拌线检测", CV_WINDOW_NORMAL);
	while(1)
	{
		temp = frame.clone();
		video >> frame;
		if (frame.empty() || temp.empty())// 对帧进行异常检测
			continue;

		Mat result = MoveDetect(temp, frame, line);
		imshow("拌线检测", result);
		if (waitKey(40) == 27)// 按原25fps显示, ESC退出
			break;
	}
	destroyAllWindows();
	return 0;
}
