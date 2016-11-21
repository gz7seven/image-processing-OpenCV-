#include "cv.h"
#include "highgui.h"
#include <stdio.h>
int main(int argc,char **argv)
{
    IplImage *src=0,*dst=0,*dst2=0;
    float k[9]={
        1.f/16,2.f/16,1.f/16,
        2.f/16,4.f/16,2.f/16,
        1.f/16,2.f/16,1.f/16};
    CvMat Km;
    Km = cvMat(3,3,CV_32F,k);
    src=cvLoadImage("5_13_s.bmp",0);//Force to gray image
    dst=cvCloneImage(src);
    cvNamedWindow("src",0);
    cvShowImage("src",src);
    cvNamedWindow("filtering",0);
    cvFilter2D(src,dst,&Km,cvPoint(-1,-1));
    cvShowImage("filtering",dst);
    cvWaitKey(0);
    cvReleaseImage(&src);
    cvReleaseImage(&dst);
    return 0;
}