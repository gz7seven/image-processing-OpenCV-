#include "cv.h"
#include "highgui.h"
#include "stdio.h"
#include "math.h"
#include "iostream"

using namespace std;
using namespace cv;

int main()
{
//-----------------------------------开创内存空间，载入图像--------------------------------	
	IplImage  *img,*gimg,*edge,*cedge,*rimg,*result1,*result2,*result3;//,*result4;
	img=cvLoadImage("水果1.jpg");
	cvNamedWindow("水果",1);
	cvShowImage("水果",img);
	
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


//------------------------图像阈值化处理----------------------------------------
	cvCvtColor(img,gimg,CV_BGR2GRAY);//灰度图转化
    
	cvSmooth(gimg,edge,CV_BLUR,3,3,0);//高斯模糊

	cvDilate(gimg,gimg,NULL,1);

	cvNamedWindow("灰度图像",1);
	cvShowImage("灰度图像",gimg);

	CvMemStorage *store;
	CvSeq  *outline;

	double thread=200;//设立阈值

	cvSmooth(gimg,edge,CV_BLUR,3,3,0);
	cvNot(gimg,edge);
	cvThreshold(gimg,edge,thread,255,CV_THRESH_BINARY);
	cvNot(edge,edge);


	IplConvKernel *space=cvCreateStructuringElementEx(4,4,2,2,CV_SHAPE_ELLIPSE,NULL);
	
    cvDilate(edge,edge,space,1);
	cvErode(edge,edge,space,1);
	
	cvNamedWindow("二值图像",1);
	cvShowImage("二值图像",edge);


//------------------------边缘提取----------------------------------------
	cvCanny(edge,cedge,thread,thread*7,3);
//	cvThreshold(gimg,edge,(double)edge_t,255,CV_THRESH_BINARY);
	
	store=cvCreateMemStorage(0);
	cvCreateSeq(CV_SEQ_ELTYPE_POINT,sizeof(CvSeq),sizeof(CvPoint),store);
	
	
	N_line=cvFindContours(cedge,store,&outline,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));
    cout<<" the number of outline:"<<N_line<<endl;
//	printf("nCon=%d",nCon);


//------------------------建立分类标准，图像分类----------------------------------------
	for(;outline;outline=outline->h_next)
	{
		
		if(outline->total<8)
        continue;
		if(count%2==0)
		{	
			cvDrawContours(rimg,outline,CV_RGB(0,0,0),CV_RGB(0,0,0),0,1,8,cvPoint(0,0));
			cvNamedWindow("边缘轮廓",1);
			cvShowImage("边缘轮廓",rimg);

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
//------------------------分类后图像输出----------------------------------------
	cvNamedWindow("香蕉",1);
	cvShowImage("香蕉",result1);
	cvNamedWindow("梨",1);
	cvShowImage("梨",result2);
	cvNamedWindow("苹果",1);
	cvShowImage("苹果",result3);
//	cvNamedWindow("柠檬",1);
//	cvShowImage("柠檬",result4);
	cvWaitKey(0);
	cvDestroyWindow("水果识别");

	
    return 1;
}
