#include <opencv2/opencv.hpp>
void sample6(IplImage* src);
void sample7(IplImage* src);
void sample8(IplImage* src);
void sample9(IplImage* src);
int main() {
	IplImage* src = cvLoadImage("C:\\tmp\\lena.png");

	cvShowImage("src", src);
	sample6(src);
	sample7(src);
	sample8(src);
	sample9(src);
	cvWaitKey();

	return 0;
}

void sample6(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			float nx = ((float)x / (size.width - 1)) * 2 - 1;
			float ny = ((float)y / (size.height - 1)) * 2 - 1;

			if (nx + ny < -1 || nx + ny>1 || ny - nx < -1 || ny - nx>1) {
				int bri = f.val[0] + f.val[1] + f.val[2];
				f = cvScalar(bri / 3, bri / 3, bri / 3);
			}
			cvSet2D(dst, y, x, f);
		}
	}
	cvShowImage("sample6", dst);
}

void sample7(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			float nx = ((float)x / (size.width - 1)) * 2 - 1;
			float ny = ((float)y / (size.height - 1)) * 2 - 1;

			if (pow(nx + 1, 2) + pow(ny + 1, 2) > 1 &&
				pow(nx - 1, 2) + pow(ny + 1, 2) > 1 &&
				pow(nx + 1, 2) + pow(ny - 1, 2) > 1 &&
				pow(nx - 1, 2) + pow(ny - 1, 2) > 1
				) {
				int bri = f.val[0] + f.val[1] + f.val[2];
				f = cvScalar(bri / 3, bri / 3, bri / 3);
			}

			cvSet2D(dst, y, x, f);
		}
	}
	cvShowImage("sample7", dst);
}

void sample8(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			float nx = ((float)x / (size.width - 1)) * 2 - 1;
			float ny = ((float)y / (size.height - 1)) * 2 - 1;

			if (pow(nx, 2) + pow(ny, 2) > 1) {
				int bri = f.val[0] + f.val[1] + f.val[2];
				f = cvScalar(bri / 3, bri / 3, bri / 3);
			}

			cvSet2D(dst, y, x, f);
		}
	}
	cvShowImage("sample8", dst);
}

void sample9(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			float gray = (f.val[0] + f.val[1] + f.val[2]) / 3.0f;
			CvScalar df;
			for (int k = 0; k < 3; k++) {
				df.val[k] = (f.val[k] - gray);
			}
			for (int k = 0; k < 3; k++) {
				f.val[k] -= x*(df.val[k] / (size.width-1));
			}
			cvSet2D(dst, y, x, f);
		}
	}
	cvShowImage("sample9", dst);
}
