#include<cv.h>  
#include<highgui.h>  

int main()  
{  
    IplImage* pImgSrc = NULL;    //Դͼ��  
    IplImage* pImg8u = NULL;     //�Ҷ�ͼ  
    IplImage* pImgCanny = NULL;  //��Ե�����ͼ  
    IplImage* pImgDst = NULL;    //��ͼ���ϻ��ϼ�⵽��ֱ�ߺ��ͼ��  
    CvSeq* lines = NULL;  
    CvMemStorage* storage = NULL;  
  
    /*��Ե���*/  
    pImgSrc = cvLoadImage ("6.jpg", 1);  
    pImg8u = cvCreateImage (cvGetSize(pImgSrc), IPL_DEPTH_8U, 1);  
    pImgCanny = cvCreateImage (cvGetSize(pImgSrc), IPL_DEPTH_8U, 1);  
    pImgDst = cvCreateImage (cvGetSize(pImgSrc), IPL_DEPTH_8U, 1);  
    cvCvtColor (pImgSrc, pImg8u, CV_BGR2GRAY);  
    cvCanny (pImg8u, pImgCanny, 20, 200, 3);  
  
    /*���ֱ��*/  
    storage = cvCreateMemStorage (0);  
    lines = cvHoughLines2 (pImgCanny, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 80, 200, 10);  
    pImgDst = cvCreateImage (cvGetSize(pImgSrc), IPL_DEPTH_8U, 3);  
    cvCvtColor (pImg8u, pImgDst, CV_GRAY2BGR);  
  
    /*��pImgDst�ϻ�����⵽��ֱ��*/  
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