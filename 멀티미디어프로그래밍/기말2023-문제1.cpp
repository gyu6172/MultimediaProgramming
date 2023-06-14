#include <opencv2/opencv.hpp>
IplImage* canvas;
CvPoint p1, p2;
void myMouse(int event, int x, int y, int flag, void*) {
	if (event == CV_EVENT_LBUTTONDOWN) {
		p1 = cvPoint(x, y);
	}
	if (event == CV_EVENT_MOUSEMOVE) {
		if (flag == CV_EVENT_FLAG_LBUTTON) {
			p2 = cvPoint(x, y);
			cvLine(canvas, p1, p2, cvScalar(0, 0, 0), 3);
			p1 = p2;
			cvShowImage("canvas", canvas);
		}
	}
}
int main() {
	canvas = cvCreateImage(cvSize(800, 600), 8, 3);
	cvSet(canvas, cvScalar(255, 255, 255));
	cvShowImage("canvas", canvas);
	cvSetMouseCallback("canvas", myMouse);
	cvWaitKey();
}