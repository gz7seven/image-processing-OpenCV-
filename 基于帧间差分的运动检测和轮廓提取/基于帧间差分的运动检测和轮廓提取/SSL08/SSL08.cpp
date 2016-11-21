// SSL08.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <stdio.h>
#include<opencv\highgui.h>
#include<opencv\cv.h> 
#include <opencv2\opencv.hpp>
#include "cvaux.h"
int main(int argc, char* argv[])
{		
    IplImage* pFrame = NULL;//声明IplImage指针
	IplImage *curgrayImage=NULL;//声明灰色图像
	IplImage *pregrayImage=NULL;//声明帧差分图像
	IplImage *m_pImgContour=NULL;//声明图像轮廓指针

	IplImage *difImg=NULL;
	int i=0;
	char s[21];
	CvMemStorage *stor = 0;
	stor = cvCreateMemStorage(0); // //开辟默认大小的空间
	CvSeq * contour = 0;
	contour = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), stor); 
	cvNamedWindow("video", 1);
	cvNamedWindow("gray_video", 1);
	cvNamedWindow("帧间差分", 1);
	cvNamedWindow("轮廓", 1);
	CvCapture* pCapture = NULL;
	pCapture = cvCaptureFromAVI("C:/1.mp4"); 

	if( !pCapture ) //打开视频文件
	{
		fprintf(stderr,"Could not initialize capturing...\n");
		return -1;
	}
	//逐帧读取视频
	while(pFrame = cvQueryFrame( pCapture ))
	{
		i++;
		sprintf(s,"1.bmp",i);
		cvShowImage("video", pFrame); //显示图像
		double t = (double)cvGetTickCount(); 
		if(i==1)
		{   //IplImage* cvCreateImage( CvSize size, int depth, int channels );size 图像宽、高.depth 图像元素的位深度,channels为通道数 
			curgrayImage=cvCreateImage(cvSize(pFrame->width,pFrame->height),IPL_DEPTH_8U,1);
            pregrayImage=cvCreateImage(cvSize(pFrame->width,pFrame->height),IPL_DEPTH_8U,1);
            difImg=cvCreateImage(cvSize(pFrame->width,pFrame->height),IPL_DEPTH_8U,1);
			m_pImgContour=cvCreateImage(cvSize(pFrame->width,pFrame->height),IPL_DEPTH_8U,3);
			//cvCvtColor(...)是Opencv里的颜色空间转换函数，参数CV_RGB2GRAY是RGB到gray,可以转换为灰度图像
	        cvCvtColor(pFrame,curgrayImage,CV_RGB2GRAY);
		    //	cvCopy(curgrayImage,pregrayImage,NULL);
		}
		else
		{ 
		    cvCvtColor(pFrame,curgrayImage,CV_RGB2GRAY);
            curgrayImage->origin=1;
            cvShowImage("gray_video", curgrayImage);
			//void cvAbsDiff( const CvArr* src1, const CvArr* src2, CvArr* dst );函数 cvAbsDiff 计算两个数组差的绝对值,dst(I)c = abs(src1(I)c - src2(I)c).
			cvAbsDiff(curgrayImage, pregrayImage, difImg);
			pregrayImage->origin=1;
			difImg->origin=1;
			//对灰度图像进行阈值操作得到二值图像
            cvThreshold(difImg, difImg, 30, 255.0,CV_THRESH_BINARY);

			//计算运行时间
			t = (double)cvGetTickCount()-t;///cvGetTickCount()获取毫秒数
			printf( "帧间差分速度：第%d帧,  耗时:%.1fms\n",i,t/(cvGetTickFrequency()*1000.) );
			cvShowImage("帧间差分", difImg);
			//cvCopy拷贝一个数组给另一个数组
			cvCopy(curgrayImage,pregrayImage,NULL);
			//从二值图像中检索轮廓，并返回检测到的轮廓的个数
			int Contour_Object = cvFindContours( difImg, stor, &contour, sizeof(CvContour), CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE , cvPoint(0,0) ); //仅找外轮廓
			cvCvtColor(difImg,m_pImgContour,CV_GRAY2RGB);
			//cvDrawContours在图像上绘制外部和内部轮廓
			cvDrawContours(m_pImgContour, contour, CV_RGB(0,255,255), CV_RGB(0, 0, 0),2, 2,8, cvPoint(0,0));
			m_pImgContour->origin=1;
			cvShowImage("轮廓", m_pImgContour);
		}

		//如果有按键事件，则跳出循环
		if( cvWaitKey(2) >= 0 )		break;
	}
	//销毁窗口
	cvDestroyWindow("video"); 
	cvDestroyWindow("gray_video");
	cvDestroyWindow("帧间差分");
	cvDestroyWindow("轮廓");
	//释放刚才载入图像的内存空间
	cvReleaseImage(&curgrayImage);
	cvReleaseImage(&pregrayImage);
	cvReleaseImage(&difImg);
	cvReleaseImage(&pFrame);
	cvReleaseImage(&m_pImgContour);
    cvReleaseCapture(&pCapture);
	return 0;
}

