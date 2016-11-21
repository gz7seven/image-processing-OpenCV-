#include "cv.h"
#include "highgui.h"
using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
       
        Mat src = imread("123.jpg");
        Mat dst;
        //Canny(src,dst,60,180,3);

		cvtColor(src,dst,CV_BGR2GRAY);
	    //GaussianBlur(dst,dst,Size(5,5),0,0);

		//Sobel(dst, dst, src.depth(), 1, 0, 3, 1, 0, BORDER_DEFAULT);//x方向
		//Sobel(dst, dst, src.depth(), 0, 1, 3, 1, 0, BORDER_DEFAULT);//y方向

		//Laplacian(dst,dst,src.depth());

        //二值化
	    threshold(dst,dst,50, 255, THRESH_BINARY); 

		int rows =dst.rows, cols =dst.cols, step =dst.step; 
		Mat aa;
		aa=dst.clone();
		uchar* data_ED=(uchar *)aa.data;
	    vector<Point> breakpoint;
	//uchar* data_breakpoint= breakpoint.data;
	int m=0;
    int sum=0; 
	//cout<<EDImage<<endl;
	for (int i = 0; i < rows; i++)     
    {   
        for (int j =0; j < cols; j++) 
        {   
			if(data_ED[i*step+j]>150)
				{data_ED[i*step+j]=1;
			m++;}
			else data_ED[i*step+j]=0;
		}
	}
	//vector< vector <uchar>> result(rows-2);
    for (int i = 0; i < rows-1; i++)     
    {   
        for (int j = 0; j < cols-1; j++) 
        {   
				if(data_ED[i*step+j]==1)
				{
					sum=(abs(data_ED[(i-1)*step+(j-1)]-data_ED[i*step+(j-1)]))+
					abs((data_ED[i*step+(j-1)]-data_ED[(i+1)*step+(j-1)]))+
					abs((data_ED[(i+1)*step+(j-1)]-data_ED[(i+1)*step+j]))+
					abs((data_ED[(i+1)*step+j]-data_ED[(i+1)*step+(j+1)]))+
					abs((data_ED[(i+1)*step+(j+1)]-data_ED[i*step+(j+1)]))+
					abs((data_ED[i*step+(j+1)]-data_ED[(i-1)*step+(j+1)]))+
					abs((data_ED[(i-1)*step+(j+1)]-data_ED[(i-1)*step+j]))+
					abs((data_ED[(i-1)*step+j]-data_ED[(i-1)*step+(j-1)]));

			/*sum=data_ED[(i-1)*step+(j-1)]+data_ED[(i-1)*step+j]+data_ED[(i-1)*step+(j+1)]+data_ED[i*step+(j+1)]+
					data_ED[(i+1)*step+(j+1)]+data_ED[(i+1)*step+j]+data_ED[(i+1)*step+(j-1)]+data_ED[i*step+(j-1)];*/
					//cout<<sum<<" "<<endl;
				}
				if(data_ED[(i+1)*step+j]==1&&sum ==2)
				{
					breakpoint.push_back(Point(j,i));
					//参数为：承载的图像、圆心、半径、颜色、粗细、线型
					circle(dst, Point(j,i),1,255,2,8,0);
					//cvCircle( EDLine, centerpoint ,3 , CV_RGB(0,255,0),1, 8, 3 );
				 }
				
			
		}
    } 
        imshow("src",src);
		imshow("dst",dst);
        waitKey();

        return 0;
}