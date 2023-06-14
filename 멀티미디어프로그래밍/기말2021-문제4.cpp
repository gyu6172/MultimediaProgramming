#include <opencv2/opencv.hpp>

void rotateImage(IplImage* src, IplImage* dst, float time) {

	cvSet(dst, cvScalar(0, 0, 0));

	for (int y2 = 0; y2 < dst->height; y2++) {
		for (int x2 = 0; x2 < dst->width; x2++) {

			float nx = float(x2) / (dst->width - 1) * 30 - 15;
			float ny = float(y2) / (dst->height - 1) * 30 - 15;

			float r = sqrt(nx * nx + ny * ny);

			float degree = time*int(r)/5.0f;

			float x1 = cos(degree) * (x2 - dst->width / 2) + sin(degree) * (y2 - dst->height / 2) + dst->width / 2;
			float y1 = -sin(degree) * (x2 - dst->width / 2) + cos(-degree) * (y2 - dst->height / 2) + dst->height / 2;

			if (x1<0 || x1>src->width - 1) continue;
			if (y1<0 || y1>src->height - 1) continue;

			CvScalar f = cvGet2D(src, y1, x1);
			cvSet2D(dst, y2, x2, f);

		}
	}
}

int main() {
	IplImage* src = cvLoadImage("C:\\tmp\\lena.png");
	IplImage* dst = cvCreateImage(cvGetSize(src), 8, 3);
	float t = 0.0f;

	cvShowImage("dst",src);
	cvWaitKey();

	while (true) {

		rotateImage(src, dst, t);

		cvShowImage("dst", dst);
		t += 0.1f;
		cvWaitKey(30);
	}
	cvWaitKey();

}