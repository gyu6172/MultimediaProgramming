#include <stdio.h>
#include <opencv2/opencv.hpp>

int main() {
	IplImage *img;

	img = cvLoadImage("c:\\tmp\\sample.jpg");

	cvShowImage("Hello OpenCV!",img);

	cvWaitKey();
	return 0;
}