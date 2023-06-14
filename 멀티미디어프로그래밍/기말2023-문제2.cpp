#include <opencv2/opencv.hpp>
IplImage* canvas;
IplImage* cpy;
CvPoint p1, p2;

CvScalar colors[2] = {cvScalar(255, 255, 255), cvScalar(0,0,0)};
int idx = 0;
void myMouse(int event, int x, int y, int flag, void*) {
	if (event == CV_EVENT_LBUTTONDOWN) {
		p1 = cvPoint(x, y);
		cvCopy(canvas, cpy);
	}
	if (event == CV_EVENT_MOUSEMOVE) {
		if (flag == CV_EVENT_FLAG_LBUTTON) {
			cvCopy(cpy, canvas);
			p2 = cvPoint(x, y);
			cvRectangle(canvas, p1, p2, colors[idx], -1);
			cvShowImage("canvas", canvas);
		}
	}
	if (event == CV_EVENT_RBUTTONDOWN) {
		idx++;
		idx = idx % 2;
	}
}
int main() {
	canvas = cvCreateImage(cvSize(800, 600), 8, 3);
	cpy = cvCreateImage(cvSize(800, 600), 8, 3);
	cvSet(canvas, cvScalar(255, 0, 0));
	cvCopy(canvas, cpy);
	cvShowImage("canvas", canvas);
	cvSetMouseCallback("canvas", myMouse);
	cvWaitKey();
}