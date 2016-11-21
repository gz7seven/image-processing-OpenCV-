// sobel 1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include "cv.h"
#include "cxmisc.h"
#include "highgui.h"
#include <vector>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <ctype.h>

#pragma comment(lib, "G:\\OpenCV-2.1.0\\vc2008\\lib\\cxcore210d.lib")
#pragma comment(lib, "G:\\OpenCV-2.1.0\\vc2008\\lib\\cv210d.lib")
#pragma comment(lib, "G:\\OpenCV-2.1.0\\vc2008\\lib\\highgui210d.lib")

//对不同深度图片和较大的图片进行放缩，以至于可以在显示器上完全显示

void ShowConvertImage(char name[200],IplImage* Image)
{
 cvNamedWindow(name,1);
 char savename[350];
 sprintf(savename,"%s.jpg",name);
 
 cvSaveImage(savename,Image);
    if(Image->width<1280)
 {
  
  if(Image->depth!=IPL_DEPTH_8U)
  {   
    IplImage* NormalizeImage=NULL;
    NormalizeImage=cvCreateImage(cvGetSize(Image),IPL_DEPTH_8U,1);
    cvConvertScale(Image,NormalizeImage,1,0);//将图转为0-256，用于图片显示，
    cvShowImage(name,NormalizeImage);
             cvReleaseImage(&NormalizeImage);
  }
  else
  {
             cvShowImage(name,Image);
  }
 }
 else
 {
  IplImage* ImageResize=cvCreateImage(cvSize(1280,Image->height/(Image->width/1280)),Image->depth ,Image->nChannels);
  cvResize(Image,ImageResize,1);
     if(ImageResize->depth!=IPL_DEPTH_8U)
  {   
    IplImage* NormalizeImage=NULL;
    NormalizeImage=cvCreateImage(cvGetSize(ImageResize),IPL_DEPTH_8U,1);
    cvConvertScale(Image,NormalizeImage,1,0);//将图转为0-256，用于图片显示，
    cvShowImage(name,NormalizeImage);
             cvReleaseImage(&NormalizeImage);
  }
  else
  {
             cvShowImage(name,ImageResize);
  }
 
  cvReleaseImage(&ImageResize); 
 }
 
}
//对较大的图片缩放，不然显示器分辨率不支持，只能部分显示,具体见http://blog.csdn.net/yanmy2012/article/details/8110516
int MaxImageWidth=2650;
float Scale=1;
int MinPicWidth=640;
int MinPicHeight=428*MinPicWidth/640;
int Maxradius_self=68*MinPicWidth/640;
int Minradius_self=50*MinPicWidth/640;
int Radius_dist=20*MinPicWidth/640;
float MaxPicWidth=MinPicWidth*Scale;
float MaxPicHeight=MinPicHeight*Scale;

void main()
{

    IplImage * pictemp=NULL;
    IplImage * pic=NULL;
    char *imgpath="C:/AIPCV/3.jpg"; 
    pictemp=cvLoadImage(imgpath,-1);///获取图片，原色获取
 //pictemp=cvLoadImage("IMG_02071.jpg",-1);///获取图片，原色获取
    /////////////////改变图片的像素大小
 
 
 if(pic!=NULL)
 {
  cvReleaseImage(&pic);
 }
   
 if(pictemp->width>MaxImageWidth)
 {
     pic=cvCreateImage(cvSize(MaxPicWidth,MaxPicHeight),pictemp->depth ,3);
   
     cvResize(pictemp,pic,CV_INTER_AREA );
 }
 else
 {
 
    pic=cvCloneImage(pictemp);
 
 }  
    ShowConvertImage("pic",pic);
    cvReleaseImage(&pictemp);
 
 IplImage * Gray_pic=cvCreateImage(cvGetSize(pic),pic->depth ,1);
 cvCvtColor(pic,Gray_pic, CV_BGR2GRAY );    //////将Image变成灰度图片保存在gray中
 cvCanny(Gray_pic,Gray_pic,50,150,3);
    IplImage * Result_pic=cvCreateImage(cvGetSize(pic),IPL_DEPTH_16S ,1);
   // IplImage * Result_pic=cvCreateImage(cvGetSize(pic),IPL_DEPTH_8U ,1);

 CvMat *kernel=cvCreateMat(3,3,CV_32FC1);
 ///卷积核的初始化
 ////90度模板卷积核
 {
    cvSetReal2D(kernel,0,0, 1);  cvSetReal2D(kernel,0,1, 2); cvSetReal2D(kernel,0,2, 1);
    cvSetReal2D(kernel,1,0, 0);  cvSetReal2D(kernel,1,1, 0); cvSetReal2D(kernel,1,2, 0);
 cvSetReal2D(kernel,2,0,-1);  cvSetReal2D(kernel,2,1,-2); cvSetReal2D(kernel,2,2,-1);
 }
 ////////////进行卷积核计算
 cvFilter2D(Gray_pic,Result_pic,kernel,cvPoint(1,1));
    ShowConvertImage("卷积结果90°",Result_pic);

 

 ////225度模板卷积核
 {
    cvSetReal2D(kernel,0,0, 2);  cvSetReal2D(kernel,0,1, 1); cvSetReal2D(kernel,0,2, 0);
    cvSetReal2D(kernel,1,0, 1);  cvSetReal2D(kernel,1,1, 0); cvSetReal2D(kernel,1,2,-1);
 cvSetReal2D(kernel,2,0, 0);  cvSetReal2D(kernel,2,1,-1); cvSetReal2D(kernel,2,2,-2);
 }
 ////////////进行卷积核计算
 cvFilter2D(Gray_pic,Result_pic,kernel,cvPoint(1,1));
    ShowConvertImage("卷积结果225°",Result_pic);
 ////180度模板卷积核
 {
    cvSetReal2D(kernel,0,0, 1);  cvSetReal2D(kernel,0,1, 0); cvSetReal2D(kernel,0,2,-1);
    cvSetReal2D(kernel,1,0, 2);  cvSetReal2D(kernel,1,1, 0); cvSetReal2D(kernel,1,2,-2);
 cvSetReal2D(kernel,2,0, 1);  cvSetReal2D(kernel,2,1, 0); cvSetReal2D(kernel,2,2,-1);
 }
 ////////////进行卷积核计算
 cvFilter2D(Gray_pic,Result_pic,kernel,cvPoint(1,1));
    ShowConvertImage("卷积结果180°",Result_pic);
 ////135度模板卷积核
 {
    cvSetReal2D(kernel,0,0, 0);  cvSetReal2D(kernel,0,1,-1); cvSetReal2D(kernel,0,2,-2);
    cvSetReal2D(kernel,1,0, 1);  cvSetReal2D(kernel,1,1, 0); cvSetReal2D(kernel,1,2,-1);
 cvSetReal2D(kernel,2,0, 2);  cvSetReal2D(kernel,2,1, 1); cvSetReal2D(kernel,2,2, 0);
 }
 ////////////进行卷积核计算
 cvFilter2D(Gray_pic,Result_pic,kernel,cvPoint(1,1));
    ShowConvertImage("卷积结果135°",Result_pic);
 //90度模板卷积核
 {
    cvSetReal2D(kernel,0,0,-1);  cvSetReal2D(kernel,0,1,-2); cvSetReal2D(kernel,0,2,-1);
    cvSetReal2D(kernel,1,0, 0);  cvSetReal2D(kernel,1,1, 0); cvSetReal2D(kernel,1,2, 0);
 cvSetReal2D(kernel,2,0, 1);  cvSetReal2D(kernel,2,1, 2); cvSetReal2D(kernel,2,2, 1);
 }
 ////////////进行卷积核计算
 cvFilter2D(Gray_pic,Result_pic,kernel,cvPoint(1,1));
    ShowConvertImage("卷积结果90°",Result_pic);
 ////45度模板卷积核
 {
    cvSetReal2D(kernel,0,0,-2);  cvSetReal2D(kernel,0,1,-1); cvSetReal2D(kernel,0,2, 0);
    cvSetReal2D(kernel,1,0,-1);  cvSetReal2D(kernel,1,1, 0); cvSetReal2D(kernel,1,2, 1);
 cvSetReal2D(kernel,2,0, 0);  cvSetReal2D(kernel,2,1, 1); cvSetReal2D(kernel,2,2, 2);
 }
 ////////////进行卷积核计算
 cvFilter2D(Gray_pic,Result_pic,kernel,cvPoint(1,1));
    ShowConvertImage("卷积结果45°",Result_pic);
 ////0度模板卷积核
 {
    cvSetReal2D(kernel,0,0,-1);  cvSetReal2D(kernel,0,1, 0); cvSetReal2D(kernel,0,2, 1);
    cvSetReal2D(kernel,1,0,-2);  cvSetReal2D(kernel,1,1, 0); cvSetReal2D(kernel,1,2, 2);
 cvSetReal2D(kernel,2,0,-1);  cvSetReal2D(kernel,2,1, 0); cvSetReal2D(kernel,2,2, 1);
 }
 ////////////进行卷积核计算
 cvFilter2D(Gray_pic,Result_pic,kernel,cvPoint(1,1));
    ShowConvertImage("卷积结果0°",Result_pic);
   
 //315度模板卷积核
 {
    cvSetReal2D(kernel,0,0, 0);  cvSetReal2D(kernel,0,1, 1); cvSetReal2D(kernel,0,2, 2);
    cvSetReal2D(kernel,1,0,-1);  cvSetReal2D(kernel,1,1, 0); cvSetReal2D(kernel,1,2, 1);
 cvSetReal2D(kernel,2,0,-2);  cvSetReal2D(kernel,2,1,-1); cvSetReal2D(kernel,2,2, 0);
 }
 ////////////进行卷积核计算
 cvFilter2D(Gray_pic,Result_pic,kernel,cvPoint(-1,-1));
    ShowConvertImage("卷积结果315",Result_pic);
    
 
 cvSobel(Gray_pic,Result_pic,0,1,3);
 ShowConvertImage("Sobel结果X=0,Y=1",Result_pic);
 cvSobel(Gray_pic,Result_pic,0,2,3);
 ShowConvertImage("Sobel结果X=0,Y=2",Result_pic);
 cvSobel(Gray_pic,Result_pic,1,0,3);
 ShowConvertImage("Sobel结果X=1,Y=0",Result_pic);
 cvSobel(Gray_pic,Result_pic,1,1,3);
 ShowConvertImage("Sobel结果X=1,Y=1",Result_pic);
 cvSobel(Gray_pic,Result_pic,1,2,3);
 ShowConvertImage("Sobel结果X=1,Y=2",Result_pic);
 cvSobel(Gray_pic,Result_pic,2,0,3);
 ShowConvertImage("Sobel结果X=2,Y=0",Result_pic);
 cvSobel(Gray_pic,Result_pic,2,1,3);
 ShowConvertImage("Sobel结果X=2,Y=1",Result_pic);
 cvSobel(Gray_pic,Result_pic,2,2,3);
 ShowConvertImage("Sobel结果X=2,Y=2",Result_pic);

 cvWaitKey(0);
    cvReleaseImage(&Result_pic);
 cvReleaseImage(&Gray_pic);
 cvReleaseImage(&pic);
 cvReleaseMat(&kernel);

 

 


}

 
