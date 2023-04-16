#include <opencv2/opencv.hpp>
void sample20(IplImage* src);
void sample21(IplImage* src);
void sample22(IplImage* src);
void sample23(IplImage* src);
void sample24(IplImage* src);
int main() {
	IplImage* src = cvLoadImage("C:\\tmp\\lena.png");

	cvShowImage("src", src);
	sample20(src);
	sample21(src);
	sample22(src);
	sample23(src);
	sample24(src);
	cvWaitKey();

	return 0;
}

void sample20(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);
	for (int y = 0; y < size.height; y+=size.height/16) {
		for (int x = 0; x < size.width; x+=size.width/16) {
			CvScalar g = cvScalar(0,0,0);
			int cnt=0;
			for (int i = y; i < y + size.height / 16 && i < size.height; i++) {
				for (int j = x; j < x + size.width / 16 && j < size.width; j++) {
					CvScalar f = cvGet2D(src, i, j);
					cnt++;
					for (int k = 0; k < 3; k++) {
						g.val[k] += f.val[k];
					}
				}
			}
			for (int k = 0; k < 3; k++) {
				g.val[k] /= cnt;
			}
			for (int i = y; i < y + size.height / 16 && i < size.height; i++) {
				for (int j = x; j < x + size.width / 16 && j < size.width; j++) {
					cvSet2D(dst, i, j, g);
				}
			}
		}
	}
	cvShowImage("sample15", dst);
}

void sample21(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			for (int k = 0; k < 8; k++) {
				if (size.height / 8 * k <= y && y < size.height / 8 * (k + 1)) {
					for (int d = 0; d < 3; d++) {
						f.val[d] += 255 / 8 * k;
					}
				}
			}
			cvSet2D(dst, y, x, f);
		}
	}
	cvShowImage("sample16", dst);
}

void sample22(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			CvScalar g;
			float nx = (float(x) / (size.width - 1)) * 8;
			float ny = (float(y) / (size.height - 1)) * 8;
			if (int(ny) % 2 == 0) {
				if (int(nx) % 2 == 0) {
					g = f;
				}
				else {
					g = cvScalar(0, 0, 0);
				}
			}
			else {
				if (int(nx) % 2 == 0) {
					g = cvScalar(0, 0, 0);
				}
				else {
					g = cvGet2D(src, y, x);
				}
			}
			cvSet2D(dst, y, x, g);
		}
	}
	cvShowImage("sample17", dst);
}

void sample23(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			CvScalar g;
			float nx = (float(x) / (size.width - 1)) * 4;
			float ny = (float(y) / (size.height - 1)) * 4;

			if (int(nx + ny + 0.5) % 2 == 1){
				if (int(ny-nx + 10.5) % 2 == 0) {
					g = f;
				}
				else {
					for (int k = 0; k < 3; k++) {
						g.val[k] = f.val[k]*0.5;
					}
				}
			}
			else {
				if (int(ny - nx + 10.5) % 2 == 0) {
					for (int k = 0; k < 3; k++) {
						g.val[k] = f.val[k] * 0.5;
					}
				}
				else {
					g = f;
				}
			}
			cvSet2D(dst, y, x, g);
		}
	}
	cvShowImage("sample18", dst);
}

void sample24(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			CvScalar g;
			float nx = (float(x) / (size.width - 1)) * 4;
			float ny = (float(y) / (size.height - 1)) * 4;

			if (pow(nx-0.5-int(nx), 2) + pow(ny-0.5-int(ny), 2) < 0.25) {
				g = f;
			}
			else {
				for (int k = 0; k < 3; k++) {
					g.val[k] = f.val[k] * 0.5;
				}
			}

			cvSet2D(dst, y, x, g);
		}
	}
	cvShowImage("sample19", dst);
}
