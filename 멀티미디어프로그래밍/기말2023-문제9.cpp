#include <opencv2/opencv.hpp>

IplImage* src;
IplImage* dst;

CvScalar myGet2D(IplImage* src, float y, float x) {
	int j1 = int(x);
	int i1 = int(y);
	int j2 = x + 1;
	int i2 = y + 1;

	float a = x - j1;
	float b = y - i1;

	CvScalar f1, f2, f3, f4;
	f1 = cvGet2D(src, i1, j1);
	f2 = cvGet2D(src, i1, j2);
	f3 = cvGet2D(src, i2, j1);
	f4 = cvGet2D(src, i2, j2);

	CvScalar f;
	for (int k = 0; k < 3; k++) {
		f.val[k] = (1 - a) * (1 - b) * f1.val[k] +
			a * (1 - b) * f2.val[k] +
			(1 - a) * b * f3.val[k] +
			a * b * f4.val[k];
	}

	return f;
}

void setZoom(IplImage* src, IplImage* dst, int mag) {
	int w = dst->width;
	int h = dst->height;
	for (int y2 = 0; y2 < h; y2++) {
		for (int x2 = 0; x2 < w; x2++) {

			float x1 = (x2-w/2)/mag + w/2;
			float y1 = (y2-h/2)/mag + h/2;

			if (x1<0 || x1>src->width - 1) continue;
			if (y1<0 || y1>src->height - 1) continue;

			CvScalar f = myGet2D(src, y1, x1);
			cvSet2D(dst, y2, x2, f);

		}
	}
}

int main() {
	src = cvLoadImage("C:\\tmp\\lena.png");
	dst = cvCreateImage(cvGetSize(src), 8, 3);

	setZoom(src, dst, 5);

	cvShowImage("dst",dst);
	cvWaitKey();

}