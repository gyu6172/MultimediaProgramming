#include <opencv2/opencv.hpp>
#define PI 3.14159265

IplImage* src;
IplImage* dst;
IplImage* cpy;

CvPoint P, Q;

void rotationImage(IplImage* src, IplImage* dst, float time) {
	cvSet(dst, cvScalar(0,0,0));
	int w = dst->width;
	int h = dst->height;

	float px = float(P.x) / w * 2 - 1;
	float py = float(P.y) / h * 2 - 1;
	float qx = float(Q.x) / w * 2 - 1;
	float qy = float(Q.y) / h * 2 - 1;

	for (int y2 = 0; y2 < h; y2++) {
		for (int x2 = 0; x2 < w; x2++) {

			float nx = float(x2) / (w - 1) * 2 - 1;
			float ny = float(y2) / (h - 1) * 2 - 1;

			float r = sqrt(nx*nx + ny*ny);

			float degree = time*(2-r);

			int x1 = cos(degree) * (x2 - w / 2) + sin(degree) * (y2 - h / 2) + w / 2;
			int y1 = -sin(degree) * (x2 - w / 2) + cos(degree) * (y2 - h / 2) + h / 2;

			if (x1<0 || x1>src->width - 1) continue;
			if (y1<0 || y1>src->height - 1) continue;

			CvScalar f = cvGet2D(src, y1, x1);
			cvSet2D(dst, y2, x2, f);

		}
	}
}

int main() {
	src = cvLoadImage("C:\\tmp\\lena.png");
	dst = cvCreateImage(cvGetSize(src), 8, 3);
	cpy = cvCreateImage(cvGetSize(src), 8, 3);
	cvCopy(src, dst);

	float t = 0.0f;

	while (true) {
		rotationImage(src, dst, t);
		t += 0.1f;
		cvShowImage("dst", dst);
		cvWaitKey(1);
	}

	cvWaitKey();
}