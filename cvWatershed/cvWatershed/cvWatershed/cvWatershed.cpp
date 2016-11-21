// cvWatershed.cpp : �������̨Ӧ�ó������ڵ㡣
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
void on_mouse( int event, int x, int y, int flags, void* param )//opencv ���Զ�������������ʵ�ֵ  
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
        cvLine( marker_mask, prev_pt, pt, cvScalarAll(255), 5, 8, 0 );//CvScalar ��Ա��double val[4] RGBAֵA=alpha  
        cvLine( img, prev_pt, pt, cvScalarAll(255), 5, 8, 0 );  
        prev_pt = pt;  
        cvShowImage( "image", img);  
    }  
}  
  
int main( int argc, char** argv )  
{  
    char* filename = argc >= 2 ? argv[1] : (char*)"C:/AIPCV/1.jpg";  
    CvMemStorage* storage = cvCreateMemStorage(0);  
    CvRNG rng = cvRNG(-1); //��ʼ�������������״̬ 
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
    img_gray = cvCloneImage( img0 );//��ʹ�ú���֮ǰ�������صؿ����ڴ棬���ú������Լ���һ���ڴ棬Ȼ���ƺ�image��������ݣ�Ȼ�������ڴ��е����ݷ���.  
    wshed = cvCloneImage( img0 );  
    marker_mask = cvCreateImage( cvGetSize(img), 8, 1 );  //�����׵�ַ������洢�ռ�
    markers = cvCreateImage( cvGetSize(img), IPL_DEPTH_32S, 1 );  
    cvCvtColor( img, marker_mask, CV_BGR2GRAY );  //������ֻ�ý�RGBת��3ͨ���ĻҶ�ͼ��R=G=B,������ʾ��
    cvCvtColor( marker_mask, img_gray, CV_GRAY2BGR );  
    cvZero( marker_mask );//���þ����ֵ��Ϊ0,�г�ʼ��������,����˵����  
    cvZero( wshed );  
    cvShowImage( "image", img );  
    cvShowImage( "watershed transform", wshed );  
    cvSetMouseCallback( "image", on_mouse, 0 ); //���ô��������Ϣ�Ļص����� 
    for(;;)  
    {  
        int c = cvWaitKey(0);  
        if( (char)c == 27 )  
            break;  
        if( (char)c == 'r' )  
        {  
            cvZero( marker_mask );  
            cvCopy( img0, img );//cvCopy����Ҳ���������ã���Ӱ��ԭimg0ͼ��Ҳ��ʱ����  
            cvShowImage( "image", img );  
        }  
        if( (char)c == 'w' || (char)c == ' ' )  
        {  
            CvSeq* contours = 0; //����һ���� 
            CvMat* color_tab = 0;  
            int i, j, comp_count = 0;  
  
            //����ѡ����ǵ�ͼ��ȡ��������, ��ÿ�������ò�ͬ��������ʾ  
            //��ͬ������ʹ�÷�ˮ���㷨ʱ���ͳ�Ϊ��ͬ�����ӵ�  
            //�㷨���������Ը�����ͬ�����ӵ�Ϊ��������  
            cvClearMemStorage(storage);//��������һ���ڴ�洢������ͳһ������ֶ�̬������ڴ档  
            cvFindContours( marker_mask, storage, &contours, sizeof(CvContour),  
                CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE ); //�Ӷ�ֵͼ���м��������������ؼ�⵽�������ĸ����� 
            cvZero( markers );  
            for( ; contours != 0; contours = contours->h_next, comp_count++ )  
            {  
                cvDrawContours(markers, contours, cvScalarAll(comp_count+1),  
                    cvScalarAll(comp_count+1), -1, -1, 8, cvPoint(0,0) ); //������ͼ���ϻ����ⲿ���ڲ����� 
            }  
            //cvShowImage("image",markers);  
            if( comp_count == 0 )  
                continue;  
            color_tab = cvCreateMat( 1, comp_count, CV_8UC3 );//���������ɫ�б�,hang,lie,tongdao  
            for( i = 0; i < comp_count; i++ )//��ͬ���������  
            {  
                uchar* ptr = color_tab->data.ptr + i*3;  
                ptr[0] = (uchar)(cvRandInt(&rng)%180 + 50); //cvRandInt���ؾ��ȷֲ�32λ�������
                ptr[1] = (uchar)(cvRandInt(&rng)%180 + 50);  
                ptr[2] = (uchar)(cvRandInt(&rng)%180 + 50);  
            }  
            {  
                double t = (double)cvGetTickCount();//����64λ��������ʱ������,��OpenCV��ΪCvRNG���õ�ר�����ӡ� 
                cvWatershed( img0, markers );  
                cvSave("img0.xml",markers);  
                t = (double)cvGetTickCount() - t;  
                printf( "exec time = %gms\n", t/(cvGetTickFrequency()*1000.) ); //cvGetTickFrequency()����ϵͳʱ��Ƶ��  
            }  
            // paint the watershed image  
            for( i = 0; i < markers->height; i++ )  
                for( j = 0; j < markers->width; j++ )  
                {  
                    int idx = CV_IMAGE_ELEM( markers, int, i, j );//markers����������ΪIPL_DEPTH_32S  
                    uchar* dst = &CV_IMAGE_ELEM( wshed, uchar, i, j*3 );//BGR����ͨ��������һ���,��Ҫj*3  
                    if( idx == -1 ) //���ʱ��Ϊ-1����ʾ�������ֵı߽�  
                        dst[0] = dst[1] = dst[2] = (uchar)255;  
                    else if( idx <= 0 || idx > comp_count )  //�쳣���  
                        dst[0] = dst[1] = dst[2] = (uchar)0; // should not get here  
                    else //�������  
                    {  
                        uchar* ptr = color_tab->data.ptr + (idx-1)*3;  
                        dst[0] = ptr[0]; dst[1] = ptr[1]; dst[2] = ptr[2];  
                    }  
                }  
                cvAddWeighted( wshed, 0.5, img_gray, 0.5, 0, wshed );//wshed.x.y=0.5*wshed.x.y+0.5*img_gray+0��Ȩ�ں�ͼ��  
                cvShowImage( "watershed transform", wshed );  
                cvReleaseMat( &color_tab );  
        }  
    }  
    return 1;  
}  