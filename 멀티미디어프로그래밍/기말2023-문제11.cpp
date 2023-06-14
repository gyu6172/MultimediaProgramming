#include <opencv2/opencv.hpp>

IplImage* src;
IplImage* dst;
IplImage* cpy;

CvPoint P,Q;

void myRectangle(IplImage* src, IplImage* dst, CvPoint p1, CvPoint p2) {
	CvPoint lt = cvPoint(MIN(p1.x, p2.x), MIN(p1.y, p2.y));
	CvPoint rb = cvPoint(MAX(p1.x, p2.x), MAX(p1.y, p2.y));

	int w = src->width;
	int h = src->height;

	float dw = rb.x - lt.x+0.00001;
	float dh = rb.y - lt.y+0.00001;

	float sx = float(dw)/w;
	float sy = float(dh)/h;

	for (int x2 = lt.x; x2 <= rb.x; x2++) {
		for (int y2 = lt.y; y2 <= rb.y; y2++) {

			float x1 = float(x2-lt.x)/sx;
			float y1 = float(y2-lt.y)/sy;

			if(x1<0 || x1>src->width-1) continue;
			if(y1<0 || y1>src->height-1) continue;

			CvScalar f = cvGet2D(src, y1, x1);
			cvSet2D(dst, y2, x2, f);
		}
	}
}

void myMouse(int event, int x, int y, int flag, void*) {
	if (event == CV_EVENT_LBUTTONDOWN) {
		P = cvPoint(x, y);
		cvCopy(dst, cpy);
	}
	if (event == CV_EVENT_MOUSEMOVE) {
		if (flag == CV_EVENT_FLAG_LBUTTON) {
			Q = cvPoint(x, y);
			cvCopy(cpy, dst);

			myRectangle(src, dst, P, Q);

			cvShowImage("dst",dst);
		}
	}
}

int main() {
	src = cvLoadImage("C:\\tmp\\lena.png");
	dst = cvCreateImage(cvGetSize(src), 8, 3);
	cpy = cvCreateImage(cvGetSize(src), 8, 3);
	cvSet(dst, cvScalar(255,255,255));

	cvShowImage("dst", dst);
	cvSetMouseCallback("dst", myMouse);
	cvWaitKey();
}