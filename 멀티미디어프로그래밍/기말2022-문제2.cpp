#include <opencv2/opencv.hpp>
IplImage* src;
IplImage* dst;
IplImage* cpy;

CvPoint p1, p2;
CvPoint lt, rb;
void myMouse(int event, int x, int y, int flag, void*) {
	if (event == CV_EVENT_LBUTTONDOWN) {
		p1 = cvPoint(x, y);
		p2 = cvPoint(x, y);
		cvCopy(dst, cpy);
	}
	if (event == CV_EVENT_MOUSEMOVE) {
		if (flag == CV_EVENT_FLAG_LBUTTON) {
			cvCopy(cpy, dst);
			p2 = cvPoint(x, y);
			cvRectangle(dst, p1, p2, cvScalar(0, 0, 255), 3);
			cvShowImage("dst", dst);
		}
	}
	if (event == CV_EVENT_LBUTTONUP) {
		cvCopy(cpy, dst);

		lt = cvPoint(MIN(p1.x, p2.x), MIN(p1.y, p2.y));
		rb = cvPoint(MAX(p1.x, p2.x), MAX(p1.y, p2.y));

		int dx = rb.x - lt.x;
		int dy = rb.y - lt.y;

		int w = src->width;
		int h = src->height;

		for (int y = lt.y; y <= rb.y; y++) {
			for (int x = lt.x; x <= rb.x; x++) {
				float nx = float(x - lt.x) / dx * (w-1);
				float ny = float(y - lt.y) / dy * (h-1);
				
				CvScalar f = cvGet2D(src, ny, nx);
				cvSet2D(dst, y, x, f);
			}
		}

		cvShowImage("dst", dst);
	}
}
int main() {
	src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);
	dst = cvCreateImage(size, 8, 3);
	cpy = cvCreateImage(size, 8, 3);
	cvSet(dst, cvScalar(255, 255, 255));
	cvShowImage("dst", dst);
	cvSetMouseCallback("dst", myMouse);
	cvWaitKey();
}