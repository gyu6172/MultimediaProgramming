#include <stdio.h>
#include <opencv2/opencv.hpp>
int main() {
	IplImage* img1 = cvLoadImage("C:\\tmp\\세종대학교.jpg");
	IplImage* img2 = cvLoadImage("C:\\tmp\\sample.jpg");
	if (img1 == nullptr || img2 == nullptr) {
		printf("Image Load Error!");
		return -1;
	}

	CvSize size = cvGetSize(img1);
	int w = size.width;
	int h = size.height;

	cvShowImage("Hello Pixel", img2);
	cvWaitKey();

	//for (int y = 0; y < h; y++) {
	//	for (int x = 0; x < w; x++) {
	//		CvScalar color = cvGet2D(img1, y, x);

	//		color.val[0] = 255 - color.val[0];
	//		color.val[1] = 255 - color.val[1];
	//		color.val[2] = 255 - color.val[2];
	//		cvSet2D(img1, y, x, color);
	//	}
	//}

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			if (x > img2->width - 1 || y > img2->height - 1) continue;
			CvScalar c1 = cvGet2D(img1, y, x);
			CvScalar c2 = cvGet2D(img2, y, x);

			CvScalar c;
			c.val[0] = (c1.val[0] + c2.val[0]) / 2;
			c.val[1] = (c1.val[1] + c2.val[1]) / 2;
			c.val[2] = (c1.val[2] + c2.val[2]) / 2;

			cvSet2D(img2, y, x, c);

		}
	}

	cvShowImage("Hello Pixel", img2);
	cvWaitKey();




	return 0;
}