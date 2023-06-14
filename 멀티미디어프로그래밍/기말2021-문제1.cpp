#include <opencv2/opencv.hpp>
IplImage* src;
IplImage* dst;
IplImage* copy;

CvPoint p1;
CvPoint p2;
void myMouse(int event, int x, int y, int flag, void*) {
	if (event == CV_EVENT_LBUTTONDOWN) {
		p1 = cvPoint(x, y);
		cvCopy(dst, copy);
	}
	if (event == CV_EVENT_MOUSEMOVE) {
		if (flag == CV_EVENT_FLAG_LBUTTON) {
			cvCopy(copy, dst);
			p2 = cvPoint(x, y);
			cvRectangle(dst, p1, p2, cvScalar(0, 0, 0), 2);
			cvShowImage("dst", dst);
		}
	}
	if (event==CV_EVENT_LBUTTONUP) {
		cvCopy(copy, dst);
		for (int i = p1.y; i <= p2.y; i++) {
			for (int j = p1.x; j <= p2.x; j++) {
				CvScalar f = cvGet2D(dst, i, j);
				for (int k = 0; k < 3; k++) {
					f.val[k] += 50;
				}
				cvSet2D(dst, i, j, f);
			}
		}
		cvShowImage("dst", dst);
	}
	
}
int main() {
	src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);
	dst = cvCreateImage(size, 8, 3);
	copy = cvCreateImage(size, 8, 3);
	cvCopy(src, dst);
	cvCopy(src, copy);

	cvShowImage("dst", dst);
	cvSetMouseCallback("dst", myMouse);
	cvWaitKey();

}