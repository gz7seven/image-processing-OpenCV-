#include "cv.h"
#include "highgui.h"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
        Mat src = imread("5.jpg");
        Mat dst;

        //参数是按顺序写的

        //高斯滤波
        //src:输入图像
        //dst:输出图像
        //Size(5,5)模板大小，为奇数
        //x方向方差
        //Y方向方差
        GaussianBlur(src,dst,Size(5,5),0,0);
        imwrite("gauss.jpg",dst);
        
        //中值滤波
        //src:输入图像
        //dst::输出图像
        //模板宽度，为奇数
        medianBlur(src,dst,3);
        imwrite("med.jpg",dst);
        
        //均值滤波
        //src:输入图像
        //dst:输出图像
        //模板大小
        //Point(-1,-1):被平滑点位置，为负值取核中心
        blur(src,dst,Size(3,3),Point(-1,-1));
        imwrite("mean.jpg",dst);

        //双边滤波
        //src:输入图像
        //dst:输入图像
        //滤波模板半径
        //颜色空间标准差
        //坐标空间标准差
        bilateralFilter(src,dst,5,10.0,2.0);//这里滤波没什么效果，不明白
        imwrite("bil.jpg",dst);

        waitKey();

        return 0;
}