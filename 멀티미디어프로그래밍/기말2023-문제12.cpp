#include <opencv2/opencv.hpp>

IplImage* src;
IplImage* dst;
IplImage* cpy;

CvPoint P, Q;

void doSmearing(IplImage* src, IplImage* dst, CvPoint P, CvPoint Q, int radius) {

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

			cvCircle(dst, Q, 50, cvScalar(0,0,255), 3);

			cvShowImage("dst", dst);
		}
	}
	if (event == CV_EVENT_LBUTTONUP) {
		Q = cvPoint(x, y);
		cvCopy(cpy, dst);

		doSmearing(src, dst, P, Q, 50);

		cvShowImage("dst", dst);
	}
}

int main() {
	src = cvLoadImage("C:\\tmp\\lena.png");
	dst = cvCreateImage(cvGetSize(src), 8, 3);
	cpy = cvCreateImage(cvGetSize(src), 8, 3);
	cvCopy(src, dst);

	cvShowImage("dst", dst);
	cvSetMouseCallback("dst", myMouse);
	cvWaitKey();
}