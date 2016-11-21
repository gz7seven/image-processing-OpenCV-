#include "cv.h"
#include "highgui.h"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
        Mat src = imread("5.jpg");
        Mat dst;

        //�����ǰ�˳��д��

        //��˹�˲�
        //src:����ͼ��
        //dst:���ͼ��
        //Size(5,5)ģ���С��Ϊ����
        //x���򷽲�
        //Y���򷽲�
        GaussianBlur(src,dst,Size(5,5),0,0);
        imwrite("gauss.jpg",dst);
        
        //��ֵ�˲�
        //src:����ͼ��
        //dst::���ͼ��
        //ģ���ȣ�Ϊ����
        medianBlur(src,dst,3);
        imwrite("med.jpg",dst);
        
        //��ֵ�˲�
        //src:����ͼ��
        //dst:���ͼ��
        //ģ���С
        //Point(-1,-1):��ƽ����λ�ã�Ϊ��ֵȡ������
        blur(src,dst,Size(3,3),Point(-1,-1));
        imwrite("mean.jpg",dst);

        //˫���˲�
        //src:����ͼ��
        //dst:����ͼ��
        //�˲�ģ��뾶
        //��ɫ�ռ��׼��
        //����ռ��׼��
        bilateralFilter(src,dst,5,10.0,2.0);//�����˲�ûʲôЧ����������
        imwrite("bil.jpg",dst);

        waitKey();

        return 0;
}