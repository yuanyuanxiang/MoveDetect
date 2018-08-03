#include "functions.h"

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

Mat MoveDetect(const Mat &last, const Mat &cur, const Line &line)
{
	Mat result = cur.clone();
	//1.��cur��lastתΪ�Ҷ�ͼ
	Mat gray1, gray2;
	cvtColor(last, gray1, CV_BGR2GRAY);
	cvtColor(cur, gray2, CV_BGR2GRAY);
	//2.��cur��last����
	Mat diff;
	absdiff(gray1, gray2, diff);
	//3.�Բ�ֵͼdiff_thresh������ֵ������
	Mat diff_thresh;
	threshold(diff, diff_thresh, 50.0, 255.0, CV_THRESH_BINARY);
	//4.��ʴ
	Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(18, 18));
	erode(diff_thresh, diff_thresh, kernel_erode);
	//5.����
	dilate(diff_thresh, diff_thresh, kernel_dilate);
	//6.������������������
	std::vector<std::vector<Point> > contours;
	findContours(diff_thresh, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
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
	return result;
}

Mat MoveDetect(const Mat &cur, Mat &back, double k, const Line &line)
{
	//1.����ǰͼ��תΪ�Ҷ�ͼ
	Mat gray;
	cvtColor(cur, gray, CV_BGR2GRAY);
	gray.convertTo(gray, CV_32FC1);
	//2.��ǰ֡������ͼ���
	Mat diff;
	absdiff(gray, back, diff);
	diff.convertTo(diff, CV_8UC1);
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
	Mat result = cur.clone();
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
	//8.���±���
	back = (1-k) * back + k * gray;

	return result;
}
