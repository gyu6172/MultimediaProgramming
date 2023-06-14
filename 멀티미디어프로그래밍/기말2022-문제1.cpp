#include <opencv2/opencv.hpp>
IplImage* src;
CvPoint p1, p2;
void myMouse(int event, int x, int y, int flag, void*) {
	if (event == CV_EVENT_LBUTTONDOWN) {
		p1 = cvPoint(x, y);
		p2 = cvPoint(x, y);
	}
	if (event == CV_EVENT_MOUSEMOVE) {
		if (flag == CV_EVENT_FLAG_LBUTTON) {
			p2 = cvPoint(x, y);
			cvLine(src, p1, p2, cvScalar(255, 255, 255), 3);
			cvShowImage("src",src);
			p1 = p2;
		}
	}
}
int main() {
	src = cvLoadImage("C:\\tmp\\lena.png");
	cvShowImage("src", src);
	cvSetMouseCallback("src", myMouse);
	cvWaitKey();
}