#include <stdio.h>
#include <opencv2/opencv.hpp>
int main() {
	IplImage* img1 = cvLoadImage("C:\\tmp\\세종대학교.jpg");
	if (img1 == nullptr) {
		printf("Image Load Error!");
		return -1;
	}

	CvSize size = cvGetSize(img1);
	int w = size.width;
	int h = size.height;

	cvShowImage("Hello Pixel", img1);
	cvWaitKey();

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			CvScalar color = cvGet2D(img1, y, x);

			color.val[0] = 255 - color.val[0];
			color.val[1] = 255 - color.val[1];
			color.val[2] = 255 - color.val[2];
			cvSet2D(img1, y, x, color);
		}
	}


	cvShowImage("Hello Pixel", img1);
	cvWaitKey();




	return 0;
}