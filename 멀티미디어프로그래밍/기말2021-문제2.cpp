#include <opencv2/opencv.hpp>
IplImage* src;
IplImage* dst;
IplImage* copy;

CvPoint p1;
CvPoint p2;
//dst의 mid좌표에 src이미지를 반지름이 r인 원으로 theta만큼 돌려서 그림
void myCircle(IplImage* src, IplImage* dst, CvPoint mid, int r, int theta) {
	int srcw = src->width;
	int srch = src->height;


	for (int y1 = 0; y1 < srch; y1++) {
		for (int x1 = 0; x1 < srcw; x1++) {
			float nx = float(x1) / (srcw-1) * 2*r-r;
			float ny = float(y1) / (srch-1)* 2*r-r;
			if (sqrt(nx * nx + ny * ny) < r) {

			}
		}
	}
}
void myMouse(int event, int x, int y, int flag, void*) {
	if (event == CV_EVENT_LBUTTONDOWN) {
		p1 = cvPoint(x, y);
		cvCopy(dst, copy);
	}
	if (event == CV_EVENT_MOUSEMOVE) {
		if (flag == CV_EVENT_FLAG_LBUTTON) {
			cvCopy(copy, dst);
			p2 = cvPoint(x, y);
			int r = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2))/2;
			int theta = atan2(p2.y - p1.y, p2.x - p1.x);
			assert(p2.x > p1.x && p2.y && p1.y);
			myCircle(src, dst, p1, r, theta);
			cvShowImage("dst", dst);
		}
	}

}
int main() {
	src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);
	dst = cvCreateImage(size, 8, 3);
	copy = cvCreateImage(size, 8, 3);
	cvSet(dst, cvScalar(255, 255, 255));

	cvShowImage("dst", dst);
	cvSetMouseCallback("dst", myMouse);
	cvWaitKey();

}