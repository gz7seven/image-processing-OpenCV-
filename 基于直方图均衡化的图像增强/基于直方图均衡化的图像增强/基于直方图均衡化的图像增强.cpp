#include <cv.h>
#include <highgui.h>
#include <stdio.h>

void drawHistogram_gyh(IplImage* img, int level);
IplImage* strengthImage_Histogram(IplImage* img);

/*
    1��ͼ����ǿ
*/
//ֱ��ͼ���⻯ͼ����ǿ����������Ե�ͨ���Ҷ�ͼ�����ǿ
IplImage* strengthImage_Histogram(IplImage* img)
{
    IplImage* dst =  cvCreateImage(cvSize(512,512),img->depth,img->nChannels);
    int width = img->width;
    int height = img->height;
    int step = img->widthStep;
    double his[256] = {0};//�Ҷ� 
    double p_hist[256]={0};//�Ҷȱ���
    double s_hist[256]={0};//�ۼƻҶȱ���
    double total = img->width*img->height;//��������
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        his[((uchar*)img->imageData+i*step)[j]]++;
    }
    
    //p(r) = n(k)/n; k = 0��1��2��������
    //0<= r <= 1;nΪ����������n(k)��ʾ�Ҷ�k��������
    for(int i = 0; i < 256; i++)
    {
        p_hist[i] = (double)his[i]/total;
        if(i == 0)s_hist[i] = p_hist[i];
        else s_hist[i] = s_hist[i-1]+p_hist[i];
    }
    
    //ͼ����ǿ
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



//ֱ��ͼ��һ��
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
        ������ 8-bit 1��ͨ����ͼ�� I (IplImage* img)�� 
        I(x,y) ~ ((uchar*)(img->imageData + img->widthStep*y))[x]
        */
        for(int j = 0; j < width; j++)
        his[(data+i*step)[j]]++;
    }

    /*
        ���ûҶȵȼ�
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
    //ע�������ǿ������ת���������д��������
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