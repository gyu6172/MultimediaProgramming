//원으로 그라데이션
#include <stdio.h>
#include <math.h>
#include <opencv2/opencv.hpp>
int main() {
	CvSize size;
	size.width = 500;
	size.height = 400;

	IplImage* img = cvCreateImage(size, 8, 3);

	CvScalar c1 = cvScalar(0, 255, 0);
	CvScalar c2 = cvScalar(0, 0, 255);

	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			float nx = float(x) / (size.width - 1) * 2 - 1;		//x:0~500-1, nx:-1~1
			float ny = float(y) / (size.height-1) * 2 - 1;

			float a = sqrt(nx*nx + ny*ny);	//0이면 c1, 1이면 c2

			CvScalar c = cvScalar(0, 0, 0);

			for (int i = 0; i < 3; i++) {
				c.val[i] = a * c1.val[i] + (1 - a) * c2.val[i];
			}

			cvSet2D(img, y, x, c);
		}
	}

	cvShowImage("image", img);
	cvWaitKey();

	return 0;
}