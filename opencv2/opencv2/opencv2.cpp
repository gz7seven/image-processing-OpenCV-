/***********************************************************************
 * OpenCV 2.3.1 ��������
 * �������ṩ
 ***********************************************************************/
//#include "stdafx.h"
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main(int argc, char*argv[])
{
     constchar* imagename = "lena.jpg";
     //���ļ��ж���ͼ��
     Mat img = imread(imagename);
     //�������ͼ��ʧ��
     if(img.empty())
     {
         fprintf(stderr, "Can not load image %s\n", imagename);
         return-1;
     }
     //��ʾͼ��
     imshow("image",img);
     //�˺����ȴ�������������������ͷ���
     waitKey();
     return0;
}