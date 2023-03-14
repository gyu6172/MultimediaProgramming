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
			double blue = color.val[0];	//blue
			double green = color.val[1];	//green
			double red = color.val[2];		//red

			color.val[0] = red;
			color.val[2] = blue;
			cvSet2D(img1, y, x, color);
		}
	}


	cvShowImage("Hello Pixel", img1);
	cvWaitKey();








	return 0;
}