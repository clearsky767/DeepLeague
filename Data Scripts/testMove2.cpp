///运动物体检测——帧差法  
#include"opencv2/opencv.hpp"  
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
using namespace cv;
#include <iostream>  
using namespace std;

Mat MoveDetect(const cv::Mat &temp, const cv::Mat &frame)
{
 
	//平滑、帧差或背景差、二值化、膨胀、腐蚀。
	Mat result = frame.clone();
	//1.平滑处理
	IplImage *tempimg_src, *tempimg_dst;
	IplImage temp_src = IplImage(temp);
	tempimg_src = &temp_src;
	tempimg_dst = cvCreateImage(cvGetSize(tempimg_src), 8, 3);
	cvSmooth(tempimg_src, tempimg_dst, CV_BLUR, 3, 3, 0, 0);//平滑函数
 
	IplImage *frameimg_src, *frameimg_dst;
	IplImage frame_src = IplImage(frame);
	frameimg_src = &frame_src;
	frameimg_dst = cvCreateImage(cvGetSize(frameimg_src), 8, 3);
	cvSmooth(frameimg_src, frameimg_dst, CV_BLUR, 3, 3, 0, 0);//平滑函数
 
															  //2.帧差
	Mat temp1, frame1;
	temp1 = cvarrToMat(tempimg_dst);
	frame1 = cvarrToMat(frameimg_dst);
	//将background和frame转为灰度图  
	Mat gray1, gray2;
	cvtColor(temp1, gray1, CV_BGR2GRAY);
	cvtColor(frame1, gray2, CV_BGR2GRAY);
	//将background和frame做差  
	Mat diff;
	absdiff(gray1, gray2, diff);
	//imshow("帧差图", diff);
	//moveWindow("帧差图",50,50);
 
	//对差值图diff_thresh进行阈值化处理  二值化
	Mat diff_thresh;
	Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(3, 3));//函数会返回指定形状和尺寸的结构元素。																 //调用之后，调用膨胀与腐蚀函数的时候，第三个参数值保存了getStructuringElement返回值的Mat类型变量。也就是element变量。
	Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(18, 18));
 
	//进行二值化处理，选择50，255为阈值
	threshold(diff, diff_thresh, 50, 255, CV_THRESH_BINARY);
	//imshow("二值化处理后", diff_thresh);
	//moveWindow("二值化处理后",800,50);
	//膨胀  
	dilate(diff_thresh, diff_thresh, kernel_dilate);
	//imshow("膨胀处理后", diff_thresh);
	//moveWindow("膨胀处理后",50,500);
	//腐蚀  
	erode(diff_thresh, diff_thresh, kernel_erode);
	//imshow("腐蚀处理后", diff_thresh);
	//moveWindow("腐蚀处理后",800,500);
 
	//查找轮廓并绘制轮廓  
	vector<vector<Point> > contours;
	findContours(diff_thresh, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//找轮廓函数
	//drawContours(result, contours, -1, Scalar(0, 0, 255), 2);//在result上绘制轮廓  

	IplImage* img_result = cvCreateImage(cvSize(diff.cols,diff.rows), IPL_DEPTH_8U, 3);
	for (int i = 0; i < img_result->height; i++)
	{
		uchar *ptrImage = (uchar*)(img_result->imageData + i * img_result->widthStep);
 
		for (int j = 0; j < img_result->width; j++)
		{
			ptrImage[3 * j + 0]=255;
			ptrImage[3 * j + 1]=255;
			ptrImage[3 * j + 2]=255;
		}
	}

	Mat temp_result;
	temp_result = cvarrToMat(img_result);
	 //查找正外接矩形  
	vector<Rect> boundRect(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		boundRect[i] = boundingRect(contours[i]);
		if(100<boundRect[i].width && 100<boundRect[i].height){
			//rectangle(result, boundRect[i], Scalar(0, 255, 0), 2);//在result上绘制正外接矩形 
			Rect rect = Rect( boundRect[i].x, boundRect[i].y, boundRect[i].width, boundRect[i].height);
			Mat p = (Mat(result,rect)).clone();
			p.copyTo(temp_result(rect));
		}
	}
	imshow("temp_result",temp_result);
	return result;//返回result  
}

int main()
{
	//读取帧、平滑、帧差或背景差、二值化、膨胀、腐蚀。
	VideoCapture video("/home/alex/test/py/555.mp4");
	if (!video.isOpened())
		return -1;
	while (1)
	{
		int frameCount = video.get(CV_CAP_PROP_FRAME_COUNT);//获取帧数  
		double FPS = video.get(CV_CAP_PROP_FPS);//获取FPS  
		Mat frame;//存储帧  
		Mat temp;//存储前一帧图像  
		Mat result;//存储结果图像  
		for (int i = 0; i < frameCount; i++)
		{
			//读取帧
			video >> frame;//读帧进frame  
			//imshow("frame", frame);
			if (frame.empty())//对帧进行异常检测  
			{
				cout << "frame is empty!" << endl;
				break;
			}
			//处理帧
			if (i == 0)//如果为第一帧（temp还为空）  
			{
				result = MoveDetect(frame, frame);//调用MoveDetect()进行运动物体检测，返回值存入result  
			}
			else//若不是第一帧（temp有值了）  
			{
				result = MoveDetect(temp, frame);//调用MoveDetect()进行运动物体检测，返回值存入result  
			}
 
			//imshow("result", result);
			if (waitKey(1000.0 / FPS) == 27)//按原FPS显示  
			{
				cout << "ESC退出!" << endl;
				break;
			}
			temp = frame.clone();
		}
	}
	return 0;
 
}