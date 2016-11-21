// sobel.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <cv.h>
#include <highgui.h>
void main()
{
IplImage *frame,*gray,*sobel;
frame=cvLoadImage("lena.jpg");//����ͼ��
gray=cvCreateImage(cvGetSize(frame),frame->depth,1);//����ͼ��ռ�
sobel=cvCreateImage(cvGetSize(frame),IPL_DEPTH_16S,1);
cvNamedWindow("frame");
cvNamedWindow("gray");
cvNamedWindow("sobel");
cvCvtColor(frame,gray,CV_BGR2GRAY);//תΪ�Ҷ�
cvSobel(gray,sobel,1,0,3);
 
IplImage *sobel8u=cvCreateImage(cvGetSize(sobel),IPL_DEPTH_8U,1);
cvConvertScaleAbs(sobel,sobel8u,1,0);
cvShowImage("frame",frame);//��ʾͼ��
cvShowImage("gray",gray);
cvShowImage("sobel",sobel8u);
cvWaitKey(0);//�ȴ�
cvReleaseImage(&frame);//�ͷſռ䣨����Ƶ�������Ҫ�����ͷŻ�����ڴ�й¶��
cvReleaseImage(&gray);
cvReleaseImage(&sobel);
cvDestroyWindow("frame");
cvDestroyWindow("gray");
cvDestroyWindow("sobel");
}
