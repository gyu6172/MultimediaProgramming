#include <opencv2/opencv.hpp>
IplImage* src;
IplImage* dst;
IplImage* cpy;
CvPoint p;

void setZoom(IplImage* src, IplImage* dst, CvPoint mid, int width, int height, int mag) {

	CvPoint lt = cvPoint(mid.x-width/2, mid.y-height/2);
	CvPoint rb = cvPoint(mid.x+width/2, mid.y+height/2);

	for (int y2 = lt.y; y2 <= rb.y; y2++) {
		for (int x2 = lt.x; x2 <= rb.x; x2++) {

			if (x2<0 || x2>dst->width - 1) continue;
			if (y2<0 || y2>dst->height - 1) continue;

			float x1 = float(x2-mid.x)/mag+mid.x;
			float y1 = float(y2-mid.y)/mag+mid.y;

			CvScalar f1,f2,f3,f4;
			
			if(x1<0 || x1>src->width-1) continue;
			if(y1<0 || y1>src->height-1) continue;


			CvScalar f = cvGet2D(src, y1, x1);
			cvSet2D(dst, y2, x2, f);
		}
	}
}

void myMouse(int event, int x, int y, int flag, void*) {
	if (event == CV_EVENT_MOUSEMOVE) {
		cvCopy(cpy, dst);
		setZoom(src, dst, cvPoint(x, y), 100, 100, 2.0f);
		cvShowImage("dst", dst);
	}
	
}
int main() {
	src = cvLoadImage("C:\\tmp\\lena.png");
	dst = cvLoadImage("C:\\tmp\\lena.png");
	cpy = cvCreateImage(cvGetSize(dst), 8, 3);
	cvCopy(dst, cpy);
	cvSet(dst, cvScalar(255, 255, 255));
	cvShowImage("dst", dst);
	cvSetMouseCallback("dst", myMouse);
	cvWaitKey();
}