#include "functions.h"

// 参考: https://blog.csdn.net/fwq990720/article/details/79284735
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
	//如果是第一帧，需要初始化背景图像
	if(back.empty())
	{
		cvtColor(cur, back, CV_BGR2GRAY);
	}else
	{
		//1.将当前图像转为灰度图
		Mat gray;
		cvtColor(cur, gray, CV_BGR2GRAY);
		//2.当前帧跟背景图相减
		Mat diff;
		absdiff(gray, back, diff);
		//2.1 更新背景图像
		back = (1-k) * back + k * gray;
		//3.对差值图diff_thresh进行阈值化处理
		Mat diff_thresh;
		threshold(diff, diff_thresh, 60.0, 255.0, CV_THRESH_BINARY);
		//4.腐蚀
		Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(3, 3));
		Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(18, 18));
		erode(diff_thresh, diff_thresh, kernel_erode);
		//5.膨胀
		dilate(diff_thresh, diff_thresh, kernel_dilate);
		//6.查找轮廓并绘制轮廓
		std::vector<std::vector<Point> > contours;
		findContours(diff_thresh, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		result = cur.clone();
		drawContours(result, contours, -1, Scalar(255.0, 255.0, 255.0), 1.5);
		//7.查找正外接矩形
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
	Mat back, frame; // 存储前一帧和当前帧
	video.read(frame);
	if (frame.empty())
		return -2;
	Line line(Point(frame.cols/4, 0), Point(frame.cols/4, frame.rows));
	cvNamedWindow("拌线检测", CV_WINDOW_NORMAL);
	while(1)
	{
		video >> frame;
		if (frame.empty())// 对帧进行异常检测
			continue;

		Mat result = MoveDetect(frame, back, line, k);
		if(!result.empty()) imshow("拌线检测", result);
		if (waitKey(40) == 27)// 按原25fps显示, ESC退出
			break;
	}
	destroyAllWindows();
	return 0;
}
