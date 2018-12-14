#include "functions.h"

// �ο�: https://blog.csdn.net/fwq990720/article/details/79284735
bool IsInterSec(const Line &line, const Rect &rt)
{
	int x1 = rt.x, y1 = rt.y, x2 = x1 + rt.width, y2 = y1 + rt.height, 
		xl = line.p1.x, yt = line.p1.y, xr = line.p2.x, yb = line.p2.y;
	if (xl > xr) swap(xl, xr);
	if (yt < yb) swap(yt, yb);
	int a = y1 - y2;
	int b = x2 - x1;
	int c = x1 * y2 - y1 * x2;
	int f1 = a * xl + b * yb + c;
	int f2 = a * xl + b * yt + c;
	int f3 = a * xr + b * yb + c;
	int f4 = a * xr + b * yt + c;
	if ((f1>0 && f2>0 && f3>0 && f4>0) || (f1<0 && f2<0 && f3<0 && f4<0))
		return false;
	else if ((x1 > xr && x2 > xr) || (x1 < xl && x2 < xl))
		return false;
	else if ((y1 > yt && y2 > yt) || (y1 < yb && y2 < yb))
		return false;
	else
		return true;
}

Mat MoveDetect(const Mat &cur, Mat &back, const Line &line, double k)
{
	Mat result;
	//����ǵ�һ֡����Ҫ��ʼ������ͼ��
	if(back.empty())
	{
		cvtColor(cur, back, CV_BGR2GRAY);
	}else
	{
		//1.����ǰͼ��תΪ�Ҷ�ͼ
		Mat gray;
		cvtColor(cur, gray, CV_BGR2GRAY);
		//2.��ǰ֡������ͼ���
		Mat diff;
		absdiff(gray, back, diff);
		//2.1 ���±���ͼ��
		back = (1-k) * back + k * gray;
		//3.�Բ�ֵͼdiff_thresh������ֵ������
		Mat diff_thresh;
		threshold(diff, diff_thresh, 60.0, 255.0, CV_THRESH_BINARY);
		//4.��ʴ
		Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(3, 3));
		Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(18, 18));
		erode(diff_thresh, diff_thresh, kernel_erode);
		//5.����
		dilate(diff_thresh, diff_thresh, kernel_dilate);
		//6.������������������
		std::vector<std::vector<Point> > contours;
		findContours(diff_thresh, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		result = cur.clone();
		drawContours(result, contours, -1, Scalar(255.0, 255.0, 255.0), 1.5);
		//7.��������Ӿ���
		cv::line(result, line.p1, line.p2, Scalar(255.0, 0, 0), 2);
		for (std::vector<std::vector<Point> >::const_iterator i = contours.begin(); i != contours.end(); ++i)
		{
			Rect boundRect = boundingRect(*i);
			if (boundRect.width > MIN_SIZE && boundRect.height > MIN_SIZE)
			{
				Scalar color = IsInterSec(line, boundRect) ? Scalar(0, 0, 255.0) : Scalar(0, 255.0, 0);
				rectangle(result, boundRect, color, 2);
			}
		}
	}
	return result;
}

int RunTest(double k, const char *file)
{
	VideoCapture video;
	video.open(file);
	if(!video.isOpened() && file){
		video.open(0);
		if (!video.isOpened())
			return -1;
	}
	Mat back, frame; // �洢ǰһ֡�͵�ǰ֡
	video.read(frame);
	if (frame.empty())
		return -2;
	Line line(Point(frame.cols/4, 0), Point(frame.cols/4, frame.rows));
	cvNamedWindow("���߼��", CV_WINDOW_NORMAL);
	while(1)
	{
		video >> frame;
		if (frame.empty())// ��֡�����쳣���
			continue;

		Mat result = MoveDetect(frame, back, line, k);
		if(!result.empty()) imshow("���߼��", result);
		if (waitKey(40) == 27)// ��ԭ25fps��ʾ, ESC�˳�
			break;
	}
	destroyAllWindows();
	return 0;
}
