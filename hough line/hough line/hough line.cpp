#include<cv.h>  
#include<highgui.h>  

int main()  
{  
    IplImage* pImgSrc = NULL;    //源图像  
    IplImage* pImg8u = NULL;     //灰度图  
    IplImage* pImgCanny = NULL;  //边缘检测后的图  
    IplImage* pImgDst = NULL;    //在图像上画上检测到的直线后的图像  
    CvSeq* lines = NULL;  
    CvMemStorage* storage = NULL;  
  
    /*边缘检测*/  
    pImgSrc = cvLoadImage ("6.jpg", 1);  
    pImg8u = cvCreateImage (cvGetSize(pImgSrc), IPL_DEPTH_8U, 1);  
    pImgCanny = cvCreateImage (cvGetSize(pImgSrc), IPL_DEPTH_8U, 1);  
    pImgDst = cvCreateImage (cvGetSize(pImgSrc), IPL_DEPTH_8U, 1);  
    cvCvtColor (pImgSrc, pImg8u, CV_BGR2GRAY);  
    cvCanny (pImg8u, pImgCanny, 20, 200, 3);  
  
    /*检测直线*/  
    storage = cvCreateMemStorage (0);  
    lines = cvHoughLines2 (pImgCanny, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 80, 200, 10);  
    pImgDst = cvCreateImage (cvGetSize(pImgSrc), IPL_DEPTH_8U, 3);  
    cvCvtColor (pImg8u, pImgDst, CV_GRAY2BGR);  
  
    /*在pImgDst上画出检测到的直线*/  
    for (int i = 0; i < lines->total; i++)  
    {  
        CvPoint* line = (CvPoint*)cvGetSeqElem (lines, i);  
        cvLine (pImgDst, line[0], line[1], CV_RGB(255,0,0), 3, 8);  
    }  
  
    cvNamedWindow ("src",1);  
    cvNamedWindow ("canny", 1);  
    cvNamedWindow ("hough", 1);  
    cvShowImage ("src", pImgSrc);  
    cvShowImage ("canny", pImgCanny);  
    cvShowImage ("hough", pImgDst);  
  
    cvWaitKey (0);  
  
    cvReleaseImage (&pImgSrc);  
    cvReleaseImage (&pImg8u);  
    cvReleaseImage (&pImgCanny);  
    cvReleaseImage (&pImgDst);  
    cvReleaseMemStorage (&storage);  
  
    return 0;  
}  