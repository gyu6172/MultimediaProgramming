#include <opencv2/opencv.hpp>
CvScalar BLACK = cvScalar(0,0,0);
int main() {
	IplImage *paint = cvCreateImage(cvSize(1000,700), 8, 3);
	IplImage *palette = cvLoadImage("C:\\tmp\\palette.bmp");

	cvSet(paint, cvScalar(255,255,255));

	for (int y = 0; y < palette->height; y++) {
		for (int x = 0; x < palette->width; x++) {
			CvScalar f = cvGet2D(palette, y, x);

			cvSet2D(paint, y, x, f);
		}
	}
	for (int y = 0; y < palette->height; y++) {
		cvSet2D(paint, y, palette->width, BLACK);
	}
	for (int x = 0; x < paint->width; x++) {
		cvSet2D(paint, palette->height, x, BLACK);
	}


	cvShowImage("±×¸²ÆÇ",paint);
	cvWaitKey();
}