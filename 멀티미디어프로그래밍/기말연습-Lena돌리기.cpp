#include <opencv2/opencv.hpp>
#define PI 3.14159265

int main() {
	IplImage* src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);

	int w = size.width;
	int h = size.height;

	float degree = 0.0f;
	float sx = 0.3f;
	float sy = 0.3f;
	float tx = w/4;
	float ty = h/4;

	float dtheta = PI/100;
	float dsx = 0.05f;
	float dsy = 0.05f;
	float dtx = 10;
	float dty = 5;

	while (true) {
		cvSet(dst, cvScalar(0,0,0));
		
		for (int y2 = 0; y2 < h; y2++) {
			for (int x2 = 0; x2 < w; x2++) {

				int x1 = cos(degree) * (x2 - tx) / sx + sin(degree) * (y2 - ty) / sy + tx;
				int y1 = -sin(degree) * (x2 - tx) / sx + cos(degree) * (y2 - ty) / sy + ty;

				if (x1<0 || x1>w - 1) continue;
				if (y1<0 || y1>h - 1) continue;

				CvScalar f = cvGet2D(src, y1, x1);
				cvSet2D(dst, y2, x2, f);

			}
		}

		degree += dtheta;

		sx += dsx;
		sy += dsy;
		if (sx > 0.5f || sx<0.1f) {
			dsx *= -1;
		}
		if (sy > 0.5f || sy < 0.1f) {
			dsy *= -1;
		}
		
		tx += dtx;
		ty += dty;
		if (tx > 450 || tx < 50) {
			dtx *= -1;
		}
		if (ty > 450 || ty < 50) {
			dty *= -1;
		}

		cvShowImage("dst", dst);
		cvWaitKey(10);
	}



}