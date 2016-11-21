// read、deal andshow.cpp : 定义控制台应用程序的入口点。
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
	//image应为一个图像数据，操作成功则同先前一样显示一个窗口
	if(image)
	   {
	    printf("Height %d X with %d\n",image->height,image->width);
	    cvNamedWindow("mainWin",CV_WINDOW_AUTOSIZE);
	    cvShowImage("mainWin",image);
	    printf("Display of image is done.\n");
	    cvWaitKey(0);
		//下面执行阈值操作，但是已知图像为彩色图像，所以首先应该通过3个颜色分量的平均值将图像转换成灰度图像
        for(i=0;i<image->height;i++)
			for(j=0;j<image->width;j++)
			{
			k=((image->imageData+i*image->widthStep)[j*image->nChannels+0]
			   +(image->imageData+i*image->widthStep)[j*image->nChannels+1]
			   +(image->imageData+i*image->widthStep)[j*image->nChannels+2])/3;
            (image->imageData+i*image->widthStep)[j*image->nChannels+0]=(uchar) k;
			(image->imageData+i*image->widthStep)[j*image->nChannels+1]=(uchar) k;
			(image->imageData+i*image->widthStep)[j*image->nChannels+2]=(uchar) k;
        //此时循环中，计算像素的个数并且累加像素值，这样后面就可以计算平均值了
			mean+=k;
			count++;
			}

		//创建一个新的窗口，在其中显示灰阶图像
        cvNamedWindow("grey",CV_WINDOW_AUTOSIZE);
		cvShowImage("grey",image);
		cvWaitKey(0);

		//计算平均色阶用作阈值，并且再次遍历图像，将小于平均值的像素设置为0，将大于平均值的像素设置为255

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
	    //创建最后一个窗口，显示并保存最终的阈值化的图像
	    cvNamedWindow("thresh");
		cvShowImage("thresh",image);
		cvSaveImage("c:/thresholded.jpg",image);
		//等待用户按下一个箭，按下键之后销毁所有的窗口并退出
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