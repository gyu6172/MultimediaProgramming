#include <opencv2/opencv.hpp>

IplImage* dst;
IplImage* cpy;
CvPoint pts[3] = {cvPoint(0,0), cvPoint(0,0), cvPoint(0,0)};
int pts_idx = 0;

bool isIn(CvPoint p, CvPoint pts[3]) {
	//y = (y2-y1)/(x2-x1)*(x-x1)+y1

}

void drawTriangle(IplImage* dst, CvPoint pts[3]) {
	CvPoint lt = cvPoint(0, 0);
	CvPoint rb = cvPoint(0, 0);
	for (int k = 0; k < 3; k++) {
		lt.x = MIN(lt.x, pts[k].x);
		lt.y = MIN(lt.y, pts[k].y);
		rb.x = MAX(rb.x, pts[k].x);
		rb.y = MAX(rb.y, pts[k].y);
	}

	for (int x = lt.x; x <= rb.x; x++) {
		for (int y = lt.y; y <= rb.y; y++) {
			

			if (isIn(cvPoint(x,y), pts)){
				
				cvSet2D(dst, y, x, cvScalar(0,0,0));
			}
		}
	}

}

void myMouse(int event, int x, int y, int flag, void*) {
	if (event == CV_EVENT_LBUTTONDOWN) {
		pts[pts_idx] = cvPoint(x, y);
		pts_idx++;
		pts_idx = pts_idx % 3;
		cvCopy(dst, cpy);
	}
	if (pts_idx > 0) {
		if (event == CV_EVENT_MOUSEMOVE) {
			cvCopy(cpy, dst);
			cvLine(dst, pts[pts_idx - 1], cvPoint(x, y), cvScalar(0, 0, 0), 3);
			cvShowImage("dst", dst);
		}
	}
	if (event == CV_EVENT_LBUTTONUP) {
		if (pts_idx == 0) {
			cvLine(dst, pts[pts_idx], pts[2], cvScalar(0, 0, 0), 3);

			drawTriangle(dst, pts);

			cvShowImage("dst", dst);
		}
	}
}

int main() {
	dst = cvLoadImage("C:\\tmp\\lena.png");
	cpy = cvCloneImage(dst);

	cvShowImage("dst", dst);
	cvSetMouseCallback("dst", myMouse);
	cvWaitKey();
}