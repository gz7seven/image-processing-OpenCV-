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
    IplImage* pFrame = NULL;//����IplImageָ��
	IplImage *curgrayImage=NULL;//������ɫͼ��
	IplImage *pregrayImage=NULL;//����֡���ͼ��
	IplImage *m_pImgContour=NULL;//����ͼ������ָ��

	IplImage *difImg=NULL;
	int i=0;
	char s[21];
	CvMemStorage *stor = 0;
	stor = cvCreateMemStorage(0); // //����Ĭ�ϴ�С�Ŀռ�
	CvSeq * contour = 0;
	contour = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), stor); 
	cvNamedWindow("video", 1);
	cvNamedWindow("gray_video", 1);
	cvNamedWindow("֡����", 1);
	cvNamedWindow("����", 1);
	CvCapture* pCapture = NULL;
	pCapture = cvCaptureFromAVI("C:/1.mp4"); 

	if( !pCapture ) //����Ƶ�ļ�
	{
		fprintf(stderr,"Could not initialize capturing...\n");
		return -1;
	}
	//��֡��ȡ��Ƶ
	while(pFrame = cvQueryFrame( pCapture ))
	{
		i++;
		sprintf(s,"1.bmp",i);
		cvShowImage("video", pFrame); //��ʾͼ��
		double t = (double)cvGetTickCount(); 
		if(i==1)
		{   //IplImage* cvCreateImage( CvSize size, int depth, int channels );size ͼ�����.depth ͼ��Ԫ�ص�λ���,channelsΪͨ���� 
			curgrayImage=cvCreateImage(cvSize(pFrame->width,pFrame->height),IPL_DEPTH_8U,1);
            pregrayImage=cvCreateImage(cvSize(pFrame->width,pFrame->height),IPL_DEPTH_8U,1);
            difImg=cvCreateImage(cvSize(pFrame->width,pFrame->height),IPL_DEPTH_8U,1);
			m_pImgContour=cvCreateImage(cvSize(pFrame->width,pFrame->height),IPL_DEPTH_8U,3);
			//cvCvtColor(...)��Opencv�����ɫ�ռ�ת������������CV_RGB2GRAY��RGB��gray,����ת��Ϊ�Ҷ�ͼ��
	        cvCvtColor(pFrame,curgrayImage,CV_RGB2GRAY);
		    //	cvCopy(curgrayImage,pregrayImage,NULL);
		}
		else
		{ 
		    cvCvtColor(pFrame,curgrayImage,CV_RGB2GRAY);
            curgrayImage->origin=1;
            cvShowImage("gray_video", curgrayImage);
			//void cvAbsDiff( const CvArr* src1, const CvArr* src2, CvArr* dst );���� cvAbsDiff �������������ľ���ֵ,dst(I)c = abs(src1(I)c - src2(I)c).
			cvAbsDiff(curgrayImage, pregrayImage, difImg);
			pregrayImage->origin=1;
			difImg->origin=1;
			//�ԻҶ�ͼ�������ֵ�����õ���ֵͼ��
            cvThreshold(difImg, difImg, 30, 255.0,CV_THRESH_BINARY);

			//��������ʱ��
			t = (double)cvGetTickCount()-t;///cvGetTickCount()��ȡ������
			printf( "֡�����ٶȣ���%d֡,  ��ʱ:%.1fms\n",i,t/(cvGetTickFrequency()*1000.) );
			cvShowImage("֡����", difImg);
			//cvCopy����һ���������һ������
			cvCopy(curgrayImage,pregrayImage,NULL);
			//�Ӷ�ֵͼ���м��������������ؼ�⵽�������ĸ���
			int Contour_Object = cvFindContours( difImg, stor, &contour, sizeof(CvContour), CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE , cvPoint(0,0) ); //����������
			cvCvtColor(difImg,m_pImgContour,CV_GRAY2RGB);
			//cvDrawContours��ͼ���ϻ����ⲿ���ڲ�����
			cvDrawContours(m_pImgContour, contour, CV_RGB(0,255,255), CV_RGB(0, 0, 0),2, 2,8, cvPoint(0,0));
			m_pImgContour->origin=1;
			cvShowImage("����", m_pImgContour);
		}

		//����а����¼���������ѭ��
		if( cvWaitKey(2) >= 0 )		break;
	}
	//���ٴ���
	cvDestroyWindow("video"); 
	cvDestroyWindow("gray_video");
	cvDestroyWindow("֡����");
	cvDestroyWindow("����");
	//�ͷŸղ�����ͼ����ڴ�ռ�
	cvReleaseImage(&curgrayImage);
	cvReleaseImage(&pregrayImage);
	cvReleaseImage(&difImg);
	cvReleaseImage(&pFrame);
	cvReleaseImage(&m_pImgContour);
    cvReleaseCapture(&pCapture);
	return 0;
}

