#include <opencv2/opencv.hpp>
void sample1(IplImage *src);
void sample2(IplImage *src);
void sample3(IplImage *src);
void sample4(IplImage *src);
void sample5(IplImage *src);
int main() {
	IplImage *src = cvLoadImage("C:\\tmp\\lena.png");

	cvShowImage("src",src);
	sample1(src);
	sample2(src);
	sample3(src);
	sample4(src);
	sample5(src);
	cvWaitKey();

	return 0;
}

void sample1(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage *dst = cvCreateImage(size, 8, 3);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src,y,x);

			cvSet2D(dst, size.height-y-1, x, f);
		}
	}
	cvShowImage("sample1",dst);
}

void sample2(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);

	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);

			cvSet2D(dst, y, size.width-x-1, f);
		}
	}
	cvShowImage("sample2", dst);
}

void sample3(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f;
			if (y < size.height / 2) {
				f = cvGet2D(src, y + size.height/2, x);
			}
			else {
				f = cvGet2D(src, y - size.height / 2, x);
			}
			cvSet2D(dst, y, x, f);
		}
	}
	cvShowImage("sample3", dst);
}

void sample4(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f;
			if (x < size.width/ 2) {
				f = cvGet2D(src, y, x + size.width/2);
			}
			else {
				f = cvGet2D(src, y, x - size.width / 2);
			}
			cvSet2D(dst, y, x, f);
		}
	}
	cvShowImage("sample4", dst);
}

void sample5(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			if (x < size.width / 2) {
				if (y < size.height / 2) {
					cvSet2D(dst, size.height/2-y, size.width/2-x, f);
				}
				else {
					cvSet2D(dst, 1.5*size.height-y-1, size.width/2-x, f);
				}
			}
			else {
				if (y < size.height / 2) {
					cvSet2D(dst, size.height/2 - y, 1.5*size.width-x-1, f);
				}
				else {
					cvSet2D(dst, 1.5*size.height-y-1, 1.5 * size.width - x - 1, f);
				}
			}
		}
	}
	cvShowImage("sample5", dst);
}
