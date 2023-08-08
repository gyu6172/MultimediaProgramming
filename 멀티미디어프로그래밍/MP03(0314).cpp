#include <stdio.h>
#include <opencv2/opencv.hpp>
int main() {
	IplImage *img1 = cvLoadImage("C:\\tmp\\sample.jpg");
	//int w = img1->width;
	//int h = img1->height;

	CvSize size = cvGetSize(img1);
	int w = size.width;
	int h = size.height;

	CvScalar c;
	//0~255 소수점은 버림.
	//c.val[0] : blue
	//c.val[1] : green
	//c.val[2] : red

	int x=100;
	int y=100;
	c = cvGet2D(img1, y, x);

	printf("b = %f, g = %f, r = %f, opacicy=%f",c.val[0], c.val[1], c.val[2], c.val[3]);


	c.val[0] = 0;
	c.val[1] = 0;
	c.val[2] = 255;
	c = cvScalar(0,0,255);	//bgr


	for (int i = 100; i < 200; i++) {
		for (int j = 100; j < 200; j++) {
			cvSet2D(img1, i, j, c);
			cvShowImage("Hello Pixel", img1);
			cvWaitKey(1);
		}
	}
	


	cvShowImage("Hello Pixel", img1);
	cvWaitKey();



	


	return 0;
}