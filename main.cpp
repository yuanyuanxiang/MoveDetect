/* �˶������⡪��֡�
* C++����֡��������ʵ���˶�Ŀ���⣨opencv��
* https://blog.csdn.net/xiao__run/article/details/76849908
*/

#include "functions.h"
#include <iostream>

int main()
{
	VideoCapture video(0);
	if(!video.isOpened())
		return -1;

	Mat temp, frame; // �洢ǰһ֡�͵�ǰ֡
	Line line(Point(320, 0), Point(320, 640));
	cvNamedWindow("���߼��", CV_WINDOW_NORMAL);
	while(1)
	{
		temp = frame.clone();
		video >> frame;
		if (frame.empty() || temp.empty())// ��֡�����쳣���
			continue;

		Mat result = MoveDetect(temp, frame, line);
		imshow("���߼��", result);
		if (waitKey(40) == 27)// ��ԭ25fps��ʾ, ESC�˳�
			break;
	}
	destroyAllWindows();
	return 0;
}
