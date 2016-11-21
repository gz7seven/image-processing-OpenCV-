#include <stdio.h>
#include "cv.h"
#include "highgui.h"

#define MAX_CORNERS 100

int main(void)
{
int cornersCount=MAX_CORNERS;//�õ��Ľǵ���Ŀ
CvPoint2D32f corners[MAX_CORNERS];//����ǵ㼯��
IplImage *srcImage = 0,*grayImage = 0,*corners1 = 0,*corners2 = 0;
int i;
CvScalar color = CV_RGB(255,0,0);
char *filename = "colorflower.jpg";
cvNamedWindow("image",1);

//Load the image to be processed
srcImage = cvLoadImage(filename,1);
grayImage = cvCreateImage(cvGetSize(srcImage),IPL_DEPTH_8U,1);

//copy the source image to copy image after converting the format
//���Ʋ�תΪ�Ҷ�ͼ��
cvCvtColor(srcImage,grayImage,CV_BGR2GRAY);

//create empty images os same size as the copied images
//������ʱ32λ����ͼ��cvGoodFeaturesToTrack���õ�
corners1 = cvCreateImage(cvGetSize(srcImage),IPL_DEPTH_32F,1);
corners2 = cvCreateImage(cvGetSize(srcImage),IPL_DEPTH_32F,1);

cvGoodFeaturesToTrack(grayImage,corners1,
corners2,corners,
&cornersCount,0.05,
30,//�ǵ����С������30
0,//����ͼ��
3,0,0.4);
//Ĭ��ֵ
printf("num corners found: %d/n",cornersCount);

//��ʼ����ÿ����
if (cornersCount>0)
{
for (i=0;i<cornersCount;i++)
{
cvCircle(srcImage,cvPoint((int)(corners[i].x),(int)(corners[i].y)),
2,color,2,CV_AA,0);
}
}

cvShowImage("image",srcImage);
cvSaveImage("imagedst.png",srcImage);

cvReleaseImage(&srcImage);
cvReleaseImage(&grayImage);
cvReleaseImage(&corners1);
cvReleaseImage(&corners2);

cvWaitKey(0);
return 0;
} 