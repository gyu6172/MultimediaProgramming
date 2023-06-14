#include <opencv2/opencv.hpp>
#define PI 3.141592

IplImage* src;
IplImage* canvas;
IplImage* dst;

CvPoint p1, p2;

void myMouse(int event, int x, int y, int flag, void*) {
	if (event == CV_EVENT_LBUTTONDOWN) {
		p1 = cvPoint(x, y);
		p2 = cvPoint(x, y);
	}
	if (event == CV_EVENT_MOUSEMOVE) {
		if (flag == CV_EVENT_FLAG_LBUTTON) {
			p2 = cvPoint(x, y);
			cvLine(canvas, p1, p2, cvScalar(255, 255, 255), 30);
			
			p1 = p2;

			for (int y = p2.y-15; y < p2.y+15; y++) {
				for (int x = p2.x-15; x < p2.x+15; x++) {
					if (x<0 || x>dst->width - 1) continue;
					if (y<0 || y>dst->height - 1) continue;

					CvScalar f = cvGet2D(canvas, y, x);
					CvScalar g = cvGet2D(src, y, x);
					if (f.val[0] == 255) {
						cvSet2D(dst, y, x, g);
					}
				}
			}

			cvShowImage("dst", dst);
		}
	}
}

int main() {
	src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);
	dst = cvCreateImage(size, 8, 3);
	canvas = cvCreateImage(size, 8, 3);
	cvSet(dst, cvScalar(0, 0, 0));
	cvSet(canvas, cvScalar(0, 0, 0));

	cvShowImage("dst", dst);
	cvSetMouseCallback("dst", myMouse);
	cvWaitKey();
}