#include <cv.h>  
#include <highgui.h>  
int main()  
{  
    double degree = 45; // rotate 45 degree  
    double angle = degree  * CV_PI / 180.; // �Ƕȱ仡�� 
    double a = sin(angle), b = cos(angle); // �������ҽ�  
  
    // ����Դͼ��  
    IplImage *imgSrc = cvLoadImage("12.bmp");  
    int w_src = imgSrc->width;  
    int h_src = imgSrc->height;  
    cvNamedWindow ("src", 1);  
    cvShowImage ("src", imgSrc);  
    //w_dst��h_dstȷ�����ͼ���С 
    int w_dst = int(h_src * fabs(a) + w_src * fabs(b));  
    int h_dst = int(w_src * fabs(a) + h_src * fabs(b));  
  
    // WarpAffineӳ�����,�洢��statck����
    double map[6];  
    CvMat map_matrix = cvMat(2, 3, CV_64FC1, map);  
  
    // �������ת����
    CvPoint2D32f pt = cvPoint2D32f(w_src / 2, h_src / 2); 
	//cv2DRotationMatrix��Դͼ�����ת���ģ�Դͼ����ת�ĽǶȣ���ֵ��ʾ��ʱ����ת(����ԭ�������ͼ�����Ͻ�)��
	//����������ӣ����ڷ��ص�2X3���� 
    cv2DRotationMatrix(pt, degree, 1.0, &map_matrix);  
  
    // ����dst��ת���ĵ�����, ������ֻ��õ������һ����  
    map[2] += (w_dst - w_src) / 2;  
    map[5] += (h_dst - h_src) / 2;  
    // Ŀ��ͼ��
    IplImage *imgDst = cvCreateImage(cvSize(w_dst, h_dst), 8, 3);  
	//�任ͼ�񣬲��ú�ɫ�������ֵ 
    cvWarpAffine(  
        imgSrc,   
        imgDst,  
        &map_matrix,  
        CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS,  
        cvScalarAll(0)  
        );  
    cvNamedWindow( "dst_big", 1 );  
    cvShowImage( "dst_big", imgDst);  
   // �ͷ�imgSrc imgDst
    cvWaitKey(0);  
    cvReleaseImage(&imgSrc);  
    cvReleaseImage(&imgDst);  
    return 0;  
}  