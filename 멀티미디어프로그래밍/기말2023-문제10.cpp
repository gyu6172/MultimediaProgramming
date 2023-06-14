#include <opencv2/opencv.hpp>
#define PI 3.14159265

IplImage* src;
IplImage* dst;
IplImage* cpy;

CvPoint pts[100];
int pts_cnt=0;

void myMouse(int event, int x, int y, int flag, void*) {
	if (event == CV_EVENT_LBUTTONDOWN) {
		cvCircle(dst, cvPoint(x,y), 5, cvScalar(0,255,0), -1);
		pts[pts_cnt] = cvPoint(x,y);
		pts_cnt++;
		if (pts_cnt > 1) {
			cvLine(dst, pts[pts_cnt-2], pts[pts_cnt-1], cvScalar(0,255,0), 1);
		}
		cvShowImage("dst", dst);
	}
}

void moveCircle(IplImage* src, IplImage* dst, CvPoint pts[100], int pcnt) {
	int nIdx = 0;
	
	while (nIdx< pcnt-1) {
		CvPoint P = pts[nIdx];
		CvPoint Q = pts[nIdx+1];

		int tx = Q.x - P.x;
		int ty = Q.y - P.y;
		CvPoint R = P;

		float alpha = 0.0f;
		float beta = 0.0f;

		while (true) {
			cvCopy(src, dst);

			float rx = (1-alpha)*P.x + alpha*Q.x;
			float ry = (1-beta)*P.y + beta*Q.y;
			R = cvPoint(rx, ry);
			cvCircle(dst, R, 5, cvScalar(0,255,0), -1);

			cvShowImage("dst",dst);
			cvWaitKey(10);

			if (alpha > 1.0f && beta > 1.0f) {
				break;
			}

			alpha += 1/100.0f;
			beta += 1/100.0f;
		}


		nIdx++;
	}
}

int main() {
	src = cvLoadImage("C:\\tmp\\lena.png");
	dst = cvCreateImage(cvGetSize(src), 8, 3);
	cpy = cvCreateImage(cvGetSize(src), 8, 3);
	cvCopy(src, dst);

	cvShowImage("dst",dst);
	cvSetMouseCallback("dst", myMouse);
	cvWaitKey();
	moveCircle(src, dst, pts, pts_cnt);
	cvWaitKey();
}