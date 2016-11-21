#include "cv.h"
#include "highgui.h"
#include "stdio.h"
#include "math.h"
#include "iostream"

using namespace std;
using namespace cv;

int main()
{
//-----------------------------------�����ڴ�ռ䣬����ͼ��--------------------------------	
	IplImage  *img,*gimg,*edge,*cedge,*rimg,*result1,*result2,*result3;//,*result4;
	img=cvLoadImage("ˮ��1.jpg");
	cvNamedWindow("ˮ��",1);
	cvShowImage("ˮ��",img);
	
	int N_line=0;
	int count=0;
	
	gimg=cvCreateImage(cvSize(img->width,img->height),8,1);
    edge=cvCreateImage(cvSize(img->width,img->height),8,1);
	cedge=cvCreateImage(cvSize(img->width,img->height),8,1);
	
	rimg=cvCreateImage(cvSize(img->width,img->height),8,1);
	cvNot(rimg,rimg);
	
	result1=cvCreateImage(cvSize(img->width,img->height),8,1);
	result2=cvCreateImage(cvSize(img->width,img->height),8,1);
	result3=cvCreateImage(cvSize(img->width,img->height),8,1);
	
	cvNot(result1,result1);
	cvNot(result2,result2);
	cvNot(result3,result3);


//------------------------ͼ����ֵ������----------------------------------------
	cvCvtColor(img,gimg,CV_BGR2GRAY);//�Ҷ�ͼת��
    
	cvSmooth(gimg,edge,CV_BLUR,3,3,0);//��˹ģ��

	cvDilate(gimg,gimg,NULL,1);

	cvNamedWindow("�Ҷ�ͼ��",1);
	cvShowImage("�Ҷ�ͼ��",gimg);

	CvMemStorage *store;
	CvSeq  *outline;

	double thread=200;//������ֵ

	cvSmooth(gimg,edge,CV_BLUR,3,3,0);
	cvNot(gimg,edge);
	cvThreshold(gimg,edge,thread,255,CV_THRESH_BINARY);
	cvNot(edge,edge);


	IplConvKernel *space=cvCreateStructuringElementEx(4,4,2,2,CV_SHAPE_ELLIPSE,NULL);
	
    cvDilate(edge,edge,space,1);
	cvErode(edge,edge,space,1);
	
	cvNamedWindow("��ֵͼ��",1);
	cvShowImage("��ֵͼ��",edge);


//------------------------��Ե��ȡ----------------------------------------
	cvCanny(edge,cedge,thread,thread*7,3);
//	cvThreshold(gimg,edge,(double)edge_t,255,CV_THRESH_BINARY);
	
	store=cvCreateMemStorage(0);
	cvCreateSeq(CV_SEQ_ELTYPE_POINT,sizeof(CvSeq),sizeof(CvPoint),store);
	
	
	N_line=cvFindContours(cedge,store,&outline,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));
    cout<<" the number of outline:"<<N_line<<endl;
//	printf("nCon=%d",nCon);


//------------------------���������׼��ͼ�����----------------------------------------
	for(;outline;outline=outline->h_next)
	{
		
		if(outline->total<8)
        continue;
		if(count%2==0)
		{	
			cvDrawContours(rimg,outline,CV_RGB(0,0,0),CV_RGB(0,0,0),0,1,8,cvPoint(0,0));
			cvNamedWindow("��Ե����",1);
			cvShowImage("��Ե����",rimg);

			double area,length;
			area=(cvContourArea(outline,CV_WHOLE_SEQ)+fabs(cvContourArea(outline->h_next,CV_WHOLE_SEQ)));
			area/=2;
			length=(cvArcLength(outline,CV_WHOLE_SEQ,-1)+fabs(cvArcLength(outline->h_next,CV_WHOLE_SEQ,-1)));
			length/=2;
			
			printf("\n area=%f\n length=%f\n",area,length);


			double roundper;
			roundper=4*3.14*fabs(area)/(fabs(length)*fabs(length));
			printf(" roundper=%5f\n",roundper);
			

			if(roundper<0.6&&area>=4000)
			{
				cvDrawContours(result1,outline,CV_RGB(0,0,0),CV_RGB(0,0,0),0,CV_FILLED,8,cvPoint(0,0));

			}
			

			else if(area>=23000&&roundper<0.5)
			{
				cvDrawContours(result2,outline,CV_RGB(0,0,0),CV_RGB(0,0,0),0,CV_FILLED,8,cvPoint(0,0));
			}


			else if(area>20000&&roundper>0.6)
			{
				cvDrawContours(result2,outline,CV_RGB(0,0,0),CV_RGB(0,0,0),0,CV_FILLED,8,cvPoint(0,0));
			}
			

			else if(area<20000&&roundper>0.6)
			{
				cvDrawContours(result3,outline,CV_RGB(0,0,0),CV_RGB(0,0,0),0,CV_FILLED,8,cvPoint(0,0));
			}

//			else if(area<6000&&roundper<0.89)
//			{
//				cvDrawContours(result4,cont,CV_RGB(0,0,0),CV_RGB(0,0,0),0,CV_FILLED,8,cvPoint(0,0));
//			}
		}

		count++;

	}
//------------------------�����ͼ�����----------------------------------------
	cvNamedWindow("�㽶",1);
	cvShowImage("�㽶",result1);
	cvNamedWindow("��",1);
	cvShowImage("��",result2);
	cvNamedWindow("ƻ��",1);
	cvShowImage("ƻ��",result3);
//	cvNamedWindow("����",1);
//	cvShowImage("����",result4);
	cvWaitKey(0);
	cvDestroyWindow("ˮ��ʶ��");

	
    return 1;
}
