/************************************************************************/
/* 2018-8-4 ����   by yuan_yuanxiang@163.com                            */
/************************************************************************/

#pragma once

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;

// Ŀ����С�ߴ�
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

// ���ñ���ģ�͵��˶������⣨k=1ʱ����ͼ�������һ֡ͼ��
Mat MoveDetect(const Mat &cur, Mat &back, const Line &line, double k = 1.0);

// �����˶�Ŀ��������ӣ�k=1ʱ����ͼ�������һ֡ͼ��
int RunTest(double k);
