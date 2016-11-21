#include <cv.h>
#include <highgui.h>
#include <stdio.h>

void drawHistogram_gyh(IplImage* img, int level);
IplImage* strengthImage_Histogram(IplImage* img);

/*
    1、图像增强
*/
//直方图均衡化图像增强，这里是针对单通道灰度图像的增强
IplImage* strengthImage_Histogram(IplImage* img)
{
    IplImage* dst =  cvCreateImage(cvSize(512,512),img->depth,img->nChannels);
    int width = img->width;
    int height = img->height;
    int step = img->widthStep;
    double his[256] = {0};//灰度 
    double p_hist[256]={0};//灰度比例
    double s_hist[256]={0};//累计灰度比例
    double total = img->width*img->height;//总像素数
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        his[((uchar*)img->imageData+i*step)[j]]++;
    }
    
    //p(r) = n(k)/n; k = 0、1、2。。。；
    //0<= r <= 1;n为总像素数，n(k)表示灰度k的像素数
    for(int i = 0; i < 256; i++)
    {
        p_hist[i] = (double)his[i]/total;
        if(i == 0)s_hist[i] = p_hist[i];
        else s_hist[i] = s_hist[i-1]+p_hist[i];
    }
    
    //图像增强
    for(int i = 0; i < img->height; i++)
    {
        for(int j = 0; j < img->width; j++)
        {
            ((uchar*)(i * dst->widthStep +dst->imageData))[j] = s_hist[((uchar*)(i * img->widthStep +img->imageData))[j]]*255+0.5;
        }
    }
    
    
    cvShowImage("img",img);
    cvShowImage("dst",dst);
    drawHistogram_gyh(img,256);
    //drawHistogram_gyh(dst,256);
    cvWaitKey(0);
    return dst;
}



//直方图归一化
void drawHistogram_gyh(IplImage* img, int level)
{
    int width = img->width;
    int height = img->height;
    int step = img->widthStep;
    uchar* data = (uchar*)img->imageData;
    int his[256] = {0};
    for(int i = 0; i < height; i++)
    {
        /*
        假设有 8-bit 1－通道的图像 I (IplImage* img)： 
        I(x,y) ~ ((uchar*)(img->imageData + img->widthStep*y))[x]
        */
        for(int j = 0; j < width; j++)
        his[(data+i*step)[j]]++;
    }

    /*
        设置灰度等级
    */ 
    int dep = 256/level;
    for(int i = 1; i < 256; i++)
    {
        int step = i/dep;
        his[step] += his[i];
    }

    int max = 0;
    for(int i = 0; i < level; i++)
    {
        if(his[i]>max)
            max = his[i];
    }

    IplImage* histogram = cvCreateImage(cvSize(400,300),8,1);
    cvSet(histogram,cvScalarAll(255),0);
    //注意这里的强制类型转换，如果不写会造成误差
    double bin_width = (double)histogram->width/(level*2);
    double bin_height = (double)histogram->height/max;
    for(int i = 0; i < level+level; i=i+2)
    {
        CvPoint p0 = cvPoint(i*bin_width,histogram->height);
        CvPoint p1 = cvPoint((i+1)*bin_width,histogram->height-his[i]*bin_height);
        cvRectangle(histogram,p0,p1,cvScalar(i*(dep/2)),-1,8,0);
    }
    cvShowImage("Histogram_gyh",histogram);
    cvWaitKey(0);
    cvReleaseImage(&histogram);
}




int main(int argc,char* argv[])
{
    IplImage* img = cvLoadImage("lena.jpg",0);
    IplImage* des = cvCreateImage(cvSize(512,512),img->depth,img->nChannels);
    cvResize(img,des);
    IplImage* dst = cvCreateImage(cvSize(512,512),img->depth,img->nChannels);

    strengthImage_Histogram(des);
    cvWaitKey(0);
    cvReleaseImage(&des);
    return 0;
}