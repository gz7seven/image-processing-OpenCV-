#include <cv.h>  
#include <highgui.h>  
int main()  
{  
    double degree = 45; // rotate 45 degree  
    double angle = degree  * CV_PI / 180.; // 角度变弧度 
    double a = sin(angle), b = cos(angle); // 正弦余弦角  
  
    // 加载源图像  
    IplImage *imgSrc = cvLoadImage("12.bmp");  
    int w_src = imgSrc->width;  
    int h_src = imgSrc->height;  
    cvNamedWindow ("src", 1);  
    cvShowImage ("src", imgSrc);  
    //w_dst和h_dst确定输出图像大小 
    int w_dst = int(h_src * fabs(a) + w_src * fabs(b));  
    int h_dst = int(w_src * fabs(a) + h_src * fabs(b));  
  
    // WarpAffine映射矩阵,存储在statck数组
    double map[6];  
    CvMat map_matrix = cvMat(2, 3, CV_64FC1, map);  
  
    // 所需的旋转中心
    CvPoint2D32f pt = cvPoint2D32f(w_src / 2, h_src / 2); 
	//cv2DRotationMatrix：源图像的旋转中心，源图像旋转的角度，正值表示逆时针旋转(坐标原点假设在图像左上角)，
	//等向比例因子，用于返回的2X3矩阵。 
    cv2DRotationMatrix(pt, degree, 1.0, &map_matrix);  
  
    // 调整dst旋转中心的中心, 否则你只会得到结果的一部分  
    map[2] += (w_dst - w_src) / 2;  
    map[5] += (h_dst - h_src) / 2;  
    // 目标图像
    IplImage *imgDst = cvCreateImage(cvSize(w_dst, h_dst), 8, 3);  
	//变换图像，并用黑色填充其余值 
    cvWarpAffine(  
        imgSrc,   
        imgDst,  
        &map_matrix,  
        CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS,  
        cvScalarAll(0)  
        );  
    cvNamedWindow( "dst_big", 1 );  
    cvShowImage( "dst_big", imgDst);  
   // 释放imgSrc imgDst
    cvWaitKey(0);  
    cvReleaseImage(&imgSrc);  
    cvReleaseImage(&imgDst);  
    return 0;  
}  