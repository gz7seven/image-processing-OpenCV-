// read��deal andshow.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <stdlib.h>
#include<stdio.h>
#include<math.h>
#include<cv.h>
#include<highgui.h>

int main(int argc, char* argv[])
{
	IplImage *image=0;
	int i,j,k;
	int mean=0,count=0;
	char c;

	image=cvLoadImage("c:/AIPCV/1.jpg");
	//imageӦΪһ��ͼ�����ݣ������ɹ���ͬ��ǰһ����ʾһ������
	if(image)
	   {
	    printf("Height %d X with %d\n",image->height,image->width);
	    cvNamedWindow("mainWin",CV_WINDOW_AUTOSIZE);
	    cvShowImage("mainWin",image);
	    printf("Display of image is done.\n");
	    cvWaitKey(0);
		//����ִ����ֵ������������֪ͼ��Ϊ��ɫͼ����������Ӧ��ͨ��3����ɫ������ƽ��ֵ��ͼ��ת���ɻҶ�ͼ��
        for(i=0;i<image->height;i++)
			for(j=0;j<image->width;j++)
			{
			k=((image->imageData+i*image->widthStep)[j*image->nChannels+0]
			   +(image->imageData+i*image->widthStep)[j*image->nChannels+1]
			   +(image->imageData+i*image->widthStep)[j*image->nChannels+2])/3;
            (image->imageData+i*image->widthStep)[j*image->nChannels+0]=(uchar) k;
			(image->imageData+i*image->widthStep)[j*image->nChannels+1]=(uchar) k;
			(image->imageData+i*image->widthStep)[j*image->nChannels+2]=(uchar) k;
        //��ʱѭ���У��������صĸ��������ۼ�����ֵ����������Ϳ��Լ���ƽ��ֵ��
			mean+=k;
			count++;
			}

		//����һ���µĴ��ڣ���������ʾ�ҽ�ͼ��
        cvNamedWindow("grey",CV_WINDOW_AUTOSIZE);
		cvShowImage("grey",image);
		cvWaitKey(0);

		//����ƽ��ɫ��������ֵ�������ٴα���ͼ�񣬽�С��ƽ��ֵ����������Ϊ0��������ƽ��ֵ����������Ϊ255

		mean=mean/count;
		for(j=0;j<image->height;i++);
		  for(j=0;j<image->width;j++);
		  {
			  k=(image->imageData+i*image->widthStep)[j*image->nChannels+0];
			  if(k<mean) k=0;
			  else k=255;
			  (image->imageData+i*image->widthStep)[j*image->nChannels+0]=(uchar) k;
	          (image->imageData+i*image->widthStep)[j*image->nChannels+1]=(uchar) k;
			  (image->imageData+i*image->widthStep)[j*image->nChannels+2]=(uchar) k;
		  }
	    //�������һ�����ڣ���ʾ���������յ���ֵ����ͼ��
	    cvNamedWindow("thresh");
		cvShowImage("thresh",image);
		cvSaveImage("c:/thresholded.jpg",image);
		//�ȴ��û�����һ���������¼�֮���������еĴ��ڲ��˳�
		cvWaitKey(0);
		cvDestroyWindow("mainWin");
		cvDestroyWindow("grey");
		cvDestroyWindow("thresh");
		cvReleaseImage(&image);
	}
	else
		fprintf(stderr,"Error reading image\n");
	return 0;

}