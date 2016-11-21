// cvWatershed.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<cv.h>  
#include<highgui.h>  
#include<iostream>  
  
using namespace  std;  
  
IplImage* marker_mask = 0;  
IplImage* markers = 0;  
IplImage* img0 = 0, *img = 0, *img_gray = 0, *wshed = 0;  
CvPoint prev_pt = {-1,-1};  
void on_mouse( int event, int x, int y, int flags, void* param )//opencv 会自动给函数传入合适的值  
{  
    if( !img )  
        return;  
    if( event == CV_EVENT_LBUTTONUP || !(flags & CV_EVENT_FLAG_LBUTTON) )  
        prev_pt = cvPoint(-1,-1);  
    else if( event == CV_EVENT_LBUTTONDOWN )  
        prev_pt = cvPoint(x,y);  
    else if( event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON) )  
    {  
        CvPoint pt = cvPoint(x,y);  
        if( prev_pt.x < 0 )  
            prev_pt = pt;  
        cvLine( marker_mask, prev_pt, pt, cvScalarAll(255), 5, 8, 0 );//CvScalar 成员：double val[4] RGBA值A=alpha  
        cvLine( img, prev_pt, pt, cvScalarAll(255), 5, 8, 0 );  
        prev_pt = pt;  
        cvShowImage( "image", img);  
    }  
}  
  
int main( int argc, char** argv )  
{  
    char* filename = argc >= 2 ? argv[1] : (char*)"C:/AIPCV/1.jpg";  
    CvMemStorage* storage = cvCreateMemStorage(0);  
    CvRNG rng = cvRNG(-1); //初始化随机数生成器状态 
    if( (img0 = cvLoadImage(filename,1)) == 0 )  
        return 0;  
    printf( "Hot keys: \n"  
        "\tESC - quit the program\n"  
        "\tr - restore the original image\n"  
        "\tw or SPACE - run watershed algorithm\n"  
        "\t\t(before running it, roughly mark the areas on the image)\n"  
        "\t  (before that, roughly outline several markers on the image)\n" );  
    cvNamedWindow( "image", 1 );  
    cvNamedWindow( "watershed transform", 1 );  
    img = cvCloneImage( img0 );  
    img_gray = cvCloneImage( img0 );//在使用函数之前，不用特地开辟内存，即该函数会自己开一段内存，然后复制好image里面的数据，然后把这段内存中的数据返回.  
    wshed = cvCloneImage( img0 );  
    marker_mask = cvCreateImage( cvGetSize(img), 8, 1 );  //创建首地址并分配存储空间
    markers = cvCreateImage( cvGetSize(img), IPL_DEPTH_32S, 1 );  
    cvCvtColor( img, marker_mask, CV_BGR2GRAY );  //这两句只用将RGB转成3通道的灰度图即R=G=B,用来显示用
    cvCvtColor( marker_mask, img_gray, CV_GRAY2BGR );  
    cvZero( marker_mask );//是让矩阵的值都为0,有初始化的作用,或者说清零  
    cvZero( wshed );  
    cvShowImage( "image", img );  
    cvShowImage( "watershed transform", wshed );  
    cvSetMouseCallback( "image", on_mouse, 0 ); //设置处理鼠标消息的回调函数 
    for(;;)  
    {  
        int c = cvWaitKey(0);  
        if( (char)c == 27 )  
            break;  
        if( (char)c == 'r' )  
        {  
            cvZero( marker_mask );  
            cvCopy( img0, img );//cvCopy（）也可以这样用，不影响原img0图像，也随时更新  
            cvShowImage( "image", img );  
        }  
        if( (char)c == 'w' || (char)c == ' ' )  
        {  
            CvSeq* contours = 0; //创建一序列 
            CvMat* color_tab = 0;  
            int i, j, comp_count = 0;  
  
            //下面选将标记的图像取得其轮廓, 将每种轮廓用不同的整数表示  
            //不同的整数使用分水岭算法时，就成为不同的种子点  
            //算法本来就是以各个不同的种子点为中心扩张  
            cvClearMemStorage(storage);//用来创建一个内存存储器，来统一管理各种动态对象的内存。  
            cvFindContours( marker_mask, storage, &contours, sizeof(CvContour),  
                CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE ); //从二值图像中检索轮廓，并返回检测到的轮廓的个数。 
            cvZero( markers );  
            for( ; contours != 0; contours = contours->h_next, comp_count++ )  
            {  
                cvDrawContours(markers, contours, cvScalarAll(comp_count+1),  
                    cvScalarAll(comp_count+1), -1, -1, 8, cvPoint(0,0) ); //用于在图像上绘制外部和内部轮廓 
            }  
            //cvShowImage("image",markers);  
            if( comp_count == 0 )  
                continue;  
            color_tab = cvCreateMat( 1, comp_count, CV_8UC3 );//创建随机颜色列表,hang,lie,tongdao  
            for( i = 0; i < comp_count; i++ )//不同的整数标记  
            {  
                uchar* ptr = color_tab->data.ptr + i*3;  
                ptr[0] = (uchar)(cvRandInt(&rng)%180 + 50); //cvRandInt返回均匀分布32位的随机数
                ptr[1] = (uchar)(cvRandInt(&rng)%180 + 50);  
                ptr[2] = (uchar)(cvRandInt(&rng)%180 + 50);  
            }  
            {  
                double t = (double)cvGetTickCount();//返回64位长整数的时间数据,在OpenCV是为CvRNG设置的专用种子。 
                cvWatershed( img0, markers );  
                cvSave("img0.xml",markers);  
                t = (double)cvGetTickCount() - t;  
                printf( "exec time = %gms\n", t/(cvGetTickFrequency()*1000.) ); //cvGetTickFrequency()返回系统时钟频率  
            }  
            // paint the watershed image  
            for( i = 0; i < markers->height; i++ )  
                for( j = 0; j < markers->width; j++ )  
                {  
                    int idx = CV_IMAGE_ELEM( markers, int, i, j );//markers的数据类型为IPL_DEPTH_32S  
                    uchar* dst = &CV_IMAGE_ELEM( wshed, uchar, i, j*3 );//BGR三个通道的数是一起的,故要j*3  
                    if( idx == -1 ) //输出时若为-1，表示各个部分的边界  
                        dst[0] = dst[1] = dst[2] = (uchar)255;  
                    else if( idx <= 0 || idx > comp_count )  //异常情况  
                        dst[0] = dst[1] = dst[2] = (uchar)0; // should not get here  
                    else //正常情况  
                    {  
                        uchar* ptr = color_tab->data.ptr + (idx-1)*3;  
                        dst[0] = ptr[0]; dst[1] = ptr[1]; dst[2] = ptr[2];  
                    }  
                }  
                cvAddWeighted( wshed, 0.5, img_gray, 0.5, 0, wshed );//wshed.x.y=0.5*wshed.x.y+0.5*img_gray+0加权融合图像  
                cvShowImage( "watershed transform", wshed );  
                cvReleaseMat( &color_tab );  
        }  
    }  
    return 1;  
}  