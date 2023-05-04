#include <opencv2/opencv.hpp>
typedef struct Stroke {
	int radius;
	CvScalar color;
	CvPoint startPoint;
	CvScalar route[];
}Stroke;

int main() {
	IplImage* srcImg = cvLoadImage("C:\\tmp\\lena.png");
	CvSize imgSize = cvGetSize(srcImg);
	IplImage* refImg = cvCreateImage(imgSize, 8, 3);
	IplImage* canvas = cvCreateImage(imgSize, 8, 3);

	cvSmooth(srcImg, refImg, CV_GAUSSIAN, 11);

	cvShowImage("src",srcImg);
	cvShowImage("ref",refImg);
	cvWaitKey();
	
}