#include <opencv2/opencv.hpp>

IplImage* dst;
IplImage* cpy;
CvPoint pts[3];
int pts_idx = 0;

bool isLeft(CvPoint p, CvPoint q, CvPoint r) {
	//p->q를 이은 벡터 왼쪽에 r이 있는가?

	CvPoint pq = cvPoint(q.x-p.x, q.y-p.y);
	CvPoint pr = cvPoint(r.x-p.x, r.y-p.y);
	int cross = pq.x*pr.y - pq.y*pr.x;

	if(cross<0)return false;
	else return true;

}

bool isIn(CvPoint p, CvPoint pts[3]) {
	if(isLeft(pts[0], pts[1], p)) return false;
	if(isLeft(pts[1], pts[2], p)) return false;
	if(isLeft(pts[2], pts[0], p)) return false;
	return true;
}

void drawTriangle(IplImage* dst, CvPoint pts[3]) {

	for (int x = 0; x < dst->width; x++) {
		for (int y = 0; y < dst->height; y++) {

			if (isIn(cvPoint(x,y), pts)){
				
				cvSet2D(dst, y, x, cvScalar(255,255,255));
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
			cvLine(dst, pts[0], pts[2], cvScalar(0, 0, 0), 3);

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