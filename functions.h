#pragma once

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;

#define MIN_SIZE 22

// �߶�����
struct Line
{
	cv::Point p1;	// �˵�1��int, int��
	cv::Point p2;	// �˵�2��int, int��
	Line(const cv::Point &x, const cv::Point &y) : p1(x), p2(y) { }
};

// �ж��߶��Ƿ�������ཻ
bool IsInterSec(const Line &line, const Rect &rect);

// �˶������⺯������
Mat MoveDetect(const Mat &last, const Mat &cur, const Line &line);

// ���ñ���ģ�͵��˶�������
Mat MoveDetect(const Mat &cur, Mat &back, double k, const Line &line);
