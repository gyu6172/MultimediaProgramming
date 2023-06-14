#include <opencv2/opencv.hpp>
IplImage* dst;

CvPoint pts[3];
int cnt=0;

bool isIn(CvPoint pts[3], CvPoint p) {

}

void drawTriangle(IplImage* dst, CvPoint pts[3], int cnt) {
	for (int y = 0; y < dst->height; y++) {
		for (int x = 0; x < dst->width; x++) {
			if (isIn(pts, cvPoint(x, y))) {
				cvSet2D(dst, y, x, cvScalar(255,0,0));
			}
		}
	}
}
void myMouse(int event, int x, int y, int flag, void*) {
	if (event == CV_EVENT_LBUTTONDOWN) {
		cvCircle(dst, cvPoint(x, y), 3, cvScalar(0,0,0), -1);
		pts[cnt] = cvPoint(x,y);
		cnt++;
		if (cnt > 1) {
			cvLine(dst, pts[cnt-2], pts[cnt-1], cvScalar(0,0,0), 2);
		}
		if (cnt == 3) {
			drawTriangle(dst, pts, cnt);
			cnt = 0;
			cvLine(dst, pts[2], pts[0], cvScalar(0, 0, 0), 2);
		}
		cvShowImage("dst",dst);
	}
}

int main() {
	dst = cvCreateImage(cvSize(800,600), 8, 3);
	cvSet(dst, cvScalar(255,255,255));

	cvShowImage("dst",dst);
	cvSetMouseCallback("dst", myMouse);
	cvWaitKey();
}