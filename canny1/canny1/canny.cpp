#include "cv.h"
#include "highgui.h"

using namespace cv;

int main(int argc, char* argv[])
{
        Mat src = imread("lena.jpg");
        Mat dst;
    
        ////����ͼ��
        ////���ͼ��
        ////����ͼ����ɫͨ����
        ////x�������
        ////y�������
        //Sobel(src,dst,src.depth(),1,1);
        //imwrite("sobel.jpg",dst);

        ////����ͼ��
        ////���ͼ��
        ////����ͼ����ɫͨ����
        //Laplacian(src,dst,src.depth());
        //imwrite("laplacian.jpg",dst);

        ////����ͼ��
        ////���ͼ��
        ////��ɫת�Ҷ�
        cvtColor(src,src,CV_BGR2GRAY);  //cannyֻ����Ҷ�ͼ
        //����ͼ��
        //���ͼ��
        //����ֵ
        //����ֵ��opencv�����ǵ���ֵ��3��
        //�ڲ�sobel�˲�����С
      Canny(src,dst,60,180,3);
	  imwrite("canny.jpg",dst);

	threshold(dst, dst,1, 255, THRESH_BINARY); //��ֵ��
	//normalize(dst,dst,1.0,0.0,cv::NORM_MINMAX);
	uchar* data_ED= dst.data;
	vector<Point> breakpoint;
	//uchar* data_breakpoint= breakpoint.data;
	int rows =dst.rows, cols =dst.cols, step =dst.step; 
    int i=1,j=1,sum=0; 
	//cout<<EDImage<<endl;
    for (i = 1; i < rows-1; i++)     
    {   
        for (j = 1; j < cols-1; j++) 
        {   
			 if(data_ED[i*step+j]==255)
			 {	
				sum=(data_ED[(i-1)*step+(j-1)]-data_ED[(i-1)*step+j])+
					(data_ED[(i-1)*step+j]-data_ED[(i-1)*step+(j+1)])+
					(data_ED[(i-1)*step+(j+1)]-data_ED[i*step+(j+1)])+
					(data_ED[i*step+(j+1)]-data_ED[(i+1)*step+(j+1)])+
					(data_ED[(i+1)*step+(j+1)]-data_ED[(i+1)*step+j])+
					(data_ED[(i+1)*step+j]-data_ED[(i+1)*step+(j-1)])+
					(data_ED[(i+1)*step+(j-1)]-data_ED[i*step+(j-1)])+
					(data_ED[i*step+(j-1)]-data_ED[(i-1)*step+(j-1)]);
			/*sum=data_ED[(i-1)*step+(j-1)]+data_ED[(i-1)*step+j]+data_ED[(i-1)*step+(j+1)]+data_ED[i*step+(j+1)]+
					data_ED[(i+1)*step+(j+1)]+data_ED[(i+1)*step+j]+data_ED[(i+1)*step+(j-1)]+data_ED[i*step+(j-1)];*/
			 
				if(sum == 510)
				{
					breakpoint.push_back(Point(j,i));
					//����Ϊ�����ص�ͼ��Բ�ġ��뾶����ɫ����ϸ������
					circle(dst, Point(j,i),1,250,1.5,0);
					//cvCircle( EDLine, centerpoint ,3 , CV_RGB(0,255,0),1, 8, 3 );
				 }
			}
		}
    } 


        imwrite("canny1.jpg",dst);

        imshow("dst",dst);
        waitKey();

        return 0;
}