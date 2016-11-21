#include<iostream>
#include<opencv2/opencv.hpp>
#include<algorithm>
#include<stdio.h>
#include<vector>
#include<iostream>
#include<fstream>
using namespace std;
using namespace cv;

void getBreakpoint(cv::Mat &EDLine,CvPoint pt)
{
	//breakpoint = cv::Mat::zeros(EDImage.rows,EDImage.cols,CV_8UC1); 
     
	uchar* data_ED= EDLine.data;
	//uchar* data_breakpoint= breakpoint.data;
	int rows =EDLine.rows, cols =EDLine.cols, step =EDLine.step; 
    int i=1,j=1,x,y,sum=0; 
	//cout<<EDImage<<endl;
    for (i = 1; i < rows-1; i++)     
    {   
        for (j = 1; j < cols-1; j++) 
        {   
             if(data_ED[i*step+j]==255)
			 {
				sum=data_ED[i*step+j]+data_ED[(i-1)*step+(j-1)]+data_ED[(i-1)*step+j]+data_ED[(i-1)*step+(j+1)]+data_ED[i*step+(j-1)]+data_ED[i*step+j+1]+data_ED[(i+1)*step+j-1]+data_ED[(i+1)*step+j]+data_ED[(i+1)*step+j+1];
				if(sum == 510)
				{
					CvPoint pt = cvPoint(x,y);
					pt.x=j;
					pt.y=i;
					//breakpoint.push_back(breakP);
					//参数为：承载的图像、圆心、半径、颜色、粗细、线型
					circle(EDLine,pt,2,255,2,0);
					//cvCircle( EDLine, centerpoint ,3 , CV_RGB(0,255,0),1, 8, 3 );
				 }
					//data_breakpoint[j*step+i]=data_ED[j*step+i];
             } 
		}
    } 
	       
}
int main()
{
	Mat img,img1;
	// 载入原图
    string filename1= "canny.jpg";
	img1 = imread(filename1,0);
    //threshold(img1, img1,40, 255, THRESH_BINARY); //二值化
	//提取断点
	int x,y;
	CvPoint pt= cvPoint(x,y); 
	getBreakpoint(img1, pt);
	imshow("img",img1);
	imwrite("img.pgm",img1);
	
	waitKey(0);
	getchar();
	return 0;

}