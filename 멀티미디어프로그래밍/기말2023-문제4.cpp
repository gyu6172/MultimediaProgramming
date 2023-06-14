#include <opencv2/opencv.hpp>
IplImage* dst;
IplImage* cpy;
CvPoint p;
int r = 1;
int dr = 1;
void myMouse(int event, int x, int y, int flag, void*) {
	cvCopy(cpy, dst);
	cvCircle(dst, cvPoint(x, y), r, cvScalar(255, 255, 255), -1);
	r += dr;
	if (r >= 10 || r<=1) dr *= -1;

	cvShowImage("dst", dst);
}
int main() {
	dst = cvLoadImage("C:\\tmp\\lena.png");
	cpy = cvCreateImage(cvGetSize(dst), 8, 3);
	cvCopy(dst, cpy);
	cvSet(dst, cvScalar(255, 255, 255));
	cvShowImage("dst", dst);
	cvSetMouseCallback("dst", myMouse);
	cvWaitKey();
}