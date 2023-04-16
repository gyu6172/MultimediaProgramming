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

void sample21(IplImage* src)
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

void sample22(IplImage* src)
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
					g.val[k] = 255*2 - f.val[k]*2;
				}
			}

			cvSet2D(dst, y, x, g);
		}
	}
	cvShowImage("sample12", dst);
}

void sample23(IplImage* src)
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

void sample24(IplImage* src)
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
