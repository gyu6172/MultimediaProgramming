#include <opencv2/opencv.hpp>
void sample20(IplImage* src);
void sample21(IplImage* src);
void sample22(IplImage* src);
void sample23(IplImage* src);
void sample24(IplImage* src);
void sample25(IplImage* src);
#define PI 3.1415926535897932
int main() {
	IplImage* src = cvLoadImage("C:\\tmp\\lena.png");

	cvShowImage("src", src);
	sample20(src);
	sample21(src);
	sample22(src);
	sample23(src);
	sample24(src);
	sample25(src);
	cvWaitKey();

	return 0;
}

void sample20(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			CvScalar g;
			float nx = (float(x) / (size.width - 1)) * 20 - 10;
			float ny = (float(y) / (size.height - 1)) * 20 - 10;
			if (int(sqrt(nx * nx + ny * ny)) % 2 == 0) {
				g = f;
			}
			else {
				g = cvScalar(0, 0, 0);
			}
			cvSet2D(dst, y, x, g);
		}
	}
	cvShowImage("sample20", dst);
}

void sample21(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			CvScalar g;
			float nx = (float(x) / (size.width - 1)) * 20;
			float ny = (float(y) / (size.height - 1)) * 20;
			if (nx < 10) {
				if (ny < 10) {
					if (int(nx + ny) % 2 == 0) {
						g = cvScalar(0, 0, 0);
					}
					else {
						g = f;
					}
				}
				else {
					if (int(ny - nx + 20) % 2 == 1) {
						g = cvScalar(0, 0, 0);
					}
					else {
						g = f;
					}
				}
			}
			else {
				if (ny < 10) {
					if (int(ny - nx + 20) % 2 == 0) {
						g = cvScalar(0, 0, 0);
					}
					else {
						g = f;
					}
				}
				else {
					if (int(nx + ny) % 2 == 1) {
						g = cvScalar(0, 0, 0);
					}
					else {
						g = f;
					}
				}
			}

			cvSet2D(dst, y, x, g);
		}
	}
	cvShowImage("sample21", dst);
}

void sample22(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);
	cvSet(dst, cvScalar(0, 0, 0));
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			float nx = (float(x) / (size.width - 1)) * 4;
			float ny = (float(y) / (size.height - 1)) * 2 * PI;

			if (0 < x + sin(ny) / 4 * (size.width - 1) && x + sin(ny) / 4 * (size.width- 1) < size.width) {
				cvSet2D(dst, y, x+sin(ny)/4*(size.width-1), f);
			}

		}
	}
	cvShowImage("sample22", dst);
}

void sample23(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);
	cvSet(dst, cvScalar(0, 0, 0));
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			float nx = (float(x) / (size.width - 1)) * 2 * PI;
			float ny = (float(y) / (size.height - 1)) * 4;

			if (0 < y + sin(nx) / 4 * (size.height - 1) && y + sin(nx) / 4 * (size.height - 1) < size.height) {
				cvSet2D(dst, y + sin(nx) / 4 * (size.height - 1), x, f);
			}

		}
	}
	cvShowImage("sample23", dst);
}

void sample24(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			CvScalar g;
			float nx = (float(x) / (size.width - 1)) * 10-5;
			float ny = (float(y) / (size.height - 1)) * 2*PI;
			if (int(-2.5 * -sin(ny) - nx + 20) % 2 == 0) {
				g = f;
			}
			else {
				g = cvScalar(0, 0, 0);
			}
			cvSet2D(dst, y, x, g);
		}
	}
	cvShowImage("sample24", dst);
}

void sample25(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			CvScalar g;
			float nx = (float(x) / (size.width - 1)) * 2 * PI-PI;
			float ny = (float(y) / (size.height - 1)) * 10-5;
			if (int(-2.5*sin(nx)-ny+20)%2==0) {
				g = f;
			}
			else {
				g = cvScalar(0,0,0);
			}
			cvSet2D(dst, y, x, g);
		}
	}
	cvShowImage("sample25", dst);
}
