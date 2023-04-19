#include <opencv2/opencv.hpp>
void sample10(IplImage* src);
void sample11(IplImage* src);
void sample12(IplImage* src);
void sample13(IplImage* src);
void sample14(IplImage* src);
int main() {
	IplImage* src = cvLoadImage("C:\\tmp\\lenaTest.jpg");

	cvShowImage("src", src);
	sample10(src);
	sample11(src);
	sample12(src);
	sample13(src);
	sample14(src);
	cvWaitKey();

	return 0;
}

void sample10(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src,y,x);
			if (y < size.height / 3) {
				f.val[0] = 0;
				f.val[1] = 0;
			}
			else if (y < size.height / 3 * 2) {
				f.val[1] = 0;
				f.val[2] = 0;
			}
			else {
				f.val[0] = 0;
				f.val[2] = 0;
			}
			cvSet2D(dst, y, x, f);
		}
	}
	cvShowImage("sample10", dst);
}

void sample11(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			if (y < size.height / 2) {
				if (x < size.width / 2) {
					f.val[0] = 0;
					f.val[1] = 0;
				}
				else {
					f.val[0] = 0;
					f.val[2] = 0;
				}
			}
			else {
				if (x < size.width / 2) {
					f.val[1] = 0;
					f.val[2] = 0;
				}
				else {
					int bri = f.val[0]+f.val[1]+f.val[2];
					f = cvScalar(bri/3, bri/3, bri/3);
				}
			}
			cvSet2D(dst, y, x, f);
		}
	}
	cvShowImage("sample11", dst);
}

void sample12(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			
			CvScalar g = cvScalar(0,0,0);
			for (int k = 0; k < 3; k++) {
				if (f.val[k] < 128) {
					g.val[k] = f.val[k]*2;
				}
				else {
					g.val[k] = 2*(255 - f.val[k]);
				}
			}

			cvSet2D(dst, y, x, g);
		}
	}
	cvShowImage("sample12", dst);
}

void sample13(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);

			CvScalar g = f;
			for (int k = 0; k < 3; k++) {
				g.val[k] += (float(x)/(size.width-1))*255;
			}

			cvSet2D(dst, y, x, g);
		}
	}
	cvShowImage("sample13", dst);
}

void sample14(IplImage* src)
{
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);

			CvScalar g;
			for (int k = 0; k < 3; k++) {
				if (f.val[k] < 128) {
					g.val[k] = 255 - f.val[k] * 2;
				}
				else {
					g.val[k] = f.val[k] * 2 - 255;
				}
			}

			cvSet2D(dst, y, x, g);
		}
	}
	cvShowImage("sample14", dst);
}
