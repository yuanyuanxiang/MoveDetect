/************************************************************************/
/* 2018-8-4 更新   by yuan_yuanxiang@163.com                            */
/************************************************************************/

#pragma once

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;

// 目标最小尺寸
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

// 采用背景模型的运动物体检测（k=1时背景图像采用上一帧图像）
Mat MoveDetect(const Mat &cur, Mat &back, const Line &line, double k = 1.0);

// 运行运动目标检测的例子（k=1时背景图像采用上一帧图像）
int RunTest(double k);
