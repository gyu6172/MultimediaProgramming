#include <opencv2/opencv.hpp>
CvScalar RED = cvScalar(0,0,255);
CvScalar GREEN = cvScalar(0,255,0);
CvScalar BLUE = cvScalar(255,0,0);

IplImage* canvas;
IplImage* cpy;

CvPoint p1;

void myMouse(int event, int x, int y, int flags, void*) {

	if (event == CV_EVENT_MOUSEMOVE) {
		if (flags == (CV_EVENT_FLAG_LBUTTON | CV_EVENT_FLAG_SHIFTKEY)) {
			printf("d");
		}
	}
	//printf("event=%d, flag=%d, (%d, %d)\n",event,flags,x,y);
	//if (event == CV_EVENT_LBUTTONDOWN) {
	//	p1 = cvPoint(x,y);
	//	cvCopy(canvas, cpy);
	//	printf("copy\n");
	//}
	//if (event == CV_EVENT_MOUSEMOVE) {
	//	if (flags == CV_EVENT_FLAG_LBUTTON) {
	//		CvPoint p2 = cvPoint(x,y);
	//		cvLine(canvas, p1, p2, RED, 2);
	//		cvShowImage("canvas", canvas);
	//		p1 = p2;
	//	}

	//	if (flags == CV_EVENT_FLAG_MBUTTON) {
	//		cvCopy(cpy, canvas);
	//		CvPoint p2 = cvPoint(x, y);
	//		cvRectangle(canvas, p1, p2, GREEN, 3);
	//		cvShowImage("canvas", canvas);
	//	}

	//	if (flags == CV_EVENT_FLAG_RBUTTON) {
	//		cvCopy(cpy, canvas);
	//		CvPoint p2 = cvPoint(x, y);
	//		float r = sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
	//		cvCircle(canvas, p1, r, BLUE, 3);
	//		cvShowImage("canvas", canvas);
	//	}
	//}
}

int main() {
	canvas = cvCreateImage(cvSize(800,600), 8, 3);
	cpy = cvCreateImage(cvSize(800,600), 8, 3);
	cvSet(canvas, cvScalar(255,255,255));
	cvShowImage("canvas",canvas);
	cvSetMouseCallback("canvas",myMouse);
	cvWaitKey();
}