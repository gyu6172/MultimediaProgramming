#include <opencv2/opencv.hpp>
IplImage* src;
IplImage* dst;

CvPoint P, Q;

void doMorphing1(IplImage* src, IplImage* dst, CvPoint P, CvPoint Q) {
	cvSet(dst, cvScalar(0,0,0));

	int w = src->width;
	int h = src->height;

	for (float y1 = 0.0f; y1 < h; y1+=0.2f) {
		for (float x1 = 0.0f; x1 < w; x1+=0.2f) {

			float u0,u1;
			float v0,v1;

			float alpha, beta;

			if (x1 < P.x) {
				alpha = float(x1)/P.x;
				u0 = 0;
				u1 = Q.x-P.x;
			}
			else {
				alpha = float(x1-P.x)/(w-P.x);
				u0 = Q.x-P.x;
				u1 = 0;
			}
			if (y1 < P.y) {
				beta = float(y1)/P.y;
				v0 = 0;
				v1 = Q.y-P.y;
			}
			else {
				beta = float(y1-P.y)/(h-P.y);
				v0 = Q.y - P.y;
				v1 = 0;
			}

			float u = (1-alpha)*u0 + alpha*u1;
			float v = (1-beta)*v0 + beta*v1;

			int x2 = x1 + u;
			int y2 = y1 + v;

			if(x2<0 || x2>dst->width-1) continue;
			if(y2<0 || y2>dst->height-1) continue;

			CvScalar f = cvGet2D(src, y1, x1);
			cvSet2D(dst, y2, x2, f);
		}
	}
}

void doMorphing2(IplImage* src, IplImage* dst, CvPoint P, CvPoint Q) {
	cvSet(dst, cvScalar(0, 0, 0));

	int w = src->width;
	int h = src->height;

	float a,b;
	float pu[4] = {0.0f};
	float pv[4] = {0.0f};

	for (float y1 = 0.0f; y1 < h; y1 += 0.2f) {
		for (float x1 = 0.0f; x1 < w; x1 += 0.2f) {

			if (x1 < P.x && y1 < P.y) {
				a = float(x1)/P.x;
				b = float(y1)/P.y;


			}
			else if (x1 >= P.x && y1 < P.y) {

			}
			else if (x1 < P.x && y1 >= P.y) {

			}
			else if (x1 >= P.x && y1 >= P.y) {

			}
			

			float u =	(1-a)*(1-b)*pu[2] +
						a*(1-b)*pu[3] +
						(1-a)*b*pu[0] +
						a*b*pu[1];
			float v =	(1 - a) * (1 - b) * pv[2] +
						a * (1 - b) * pv[3] +
						(1 - a) * b * pv[0] +
						a * b * pv[1];

			int x2 = x1 + u;
			int y2 = y1 + v;

			if (x2<0 || x2>dst->width - 1) continue;
			if (y2<0 || y2>dst->height - 1) continue;

			CvScalar f = cvGet2D(src, y1, x1);
			cvSet2D(dst, y2, x2, f);
		}
	}
}
void myMouse(int event, int x, int y, int flag, void*) {
	if (event == CV_EVENT_LBUTTONDOWN) {
		P = cvPoint(x, y);
		Q = cvPoint(x, y);
	}
	if (event == CV_EVENT_LBUTTONUP) {
		Q = cvPoint(x, y);

		doMorphing1(src, dst, P, Q);

		cvShowImage("dst",dst);

		cvCopy(dst, src);
	}
}

int main() {
	src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);
	dst = cvCreateImage(size, 8, 3);
	cvCopy(src, dst);

	cvShowImage("dst",dst);
	cvSetMouseCallback("dst", myMouse);
	cvWaitKey();
}