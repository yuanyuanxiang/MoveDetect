#pragma once

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;

#define MIN_SIZE 22

// 线段类型
struct Line
{
	cv::Point p1;	// 端点1（int, int）
	cv::Point p2;	// 端点2（int, int）
	Line(const cv::Point &x, const cv::Point &y) : p1(x), p2(y) { }
};

// 判断线段是否与矩形相交
bool IsInterSec(const Line &line, const Rect &rect);

// 运动物体检测函数声明
Mat MoveDetect(const Mat &last, const Mat &cur, const Line &line);

// 采用背景模型的运动物体检测
Mat MoveDetect(const Mat &cur, Mat &back, double k, const Line &line);
