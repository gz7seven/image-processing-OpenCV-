// sobel.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <cv.h>
#include <highgui.h>
void main()
{
IplImage *frame,*gray,*sobel;
frame=cvLoadImage("lena.jpg");//加载图像
gray=cvCreateImage(cvGetSize(frame),frame->depth,1);//分配图像空间
sobel=cvCreateImage(cvGetSize(frame),IPL_DEPTH_16S,1);
cvNamedWindow("frame");
cvNamedWindow("gray");
cvNamedWindow("sobel");
cvCvtColor(frame,gray,CV_BGR2GRAY);//转为灰度
cvSobel(gray,sobel,1,0,3);
 
IplImage *sobel8u=cvCreateImage(cvGetSize(sobel),IPL_DEPTH_8U,1);
cvConvertScaleAbs(sobel,sobel8u,1,0);
cvShowImage("frame",frame);//显示图像
cvShowImage("gray",gray);
cvShowImage("sobel",sobel8u);
cvWaitKey(0);//等待
cvReleaseImage(&frame);//释放空间（对视频处理很重要，不释放会造成内存泄露）
cvReleaseImage(&gray);
cvReleaseImage(&sobel);
cvDestroyWindow("frame");
cvDestroyWindow("gray");
cvDestroyWindow("sobel");
}
