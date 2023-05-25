#include <opencv2/opencv.hpp>
IplImage* src;
IplImage* dst;

CvPoint pt1;
CvPoint pt2;

//십자 모양 사각형이 따라오는 코드
void doMorphing1(IplImage* src, IplImage* dst, CvPoint P, CvPoint Q) {

	cvSet(dst, cvScalar(0,0,0));

	float w = src->width;
	float h = src->height;

	for (float y1 = 0.0f; y1 < src->height; y1+=0.2f) {
		for (float x1 = 0.0f; x1 < src->width; x1+=0.2f) {

			float alpha, u0, u1;
			float beta, v0,v1;
			if (x1 < P.x) {
				alpha = x1 / (float)P.x;
				u0 = 0;
				u1 = Q.x - P.x;
			}
			else {
				alpha = (x1 - P.x)/(float)(w - P.x);
				u0 = Q.x - P.x;
				u1 = 0;
			}

			if (y1 < P.y) {
				beta = y1 / (float)P.y;
				v0 = 0;
				v1 = Q.y - P.y;
			}
			else {
				beta = (y1 - P.y) / (float)(h - P.y);
				v0 = Q.y - P.y;
				v1 = 0;
			}

			float u = (1-alpha)*u0 + alpha*u1;
			float v = (1-beta)*v0 + beta*v1;

			int x2 = x1+u;
			int y2 = y1+v;

			if(x2<0 || x2>dst->width-1)continue;
			if(y2<0 || y2>dst->height-1)continue;

			CvScalar f = cvGet2D(src,y1,x1);
			cvSet2D(dst, y2, x2, f);
		}
	}
}

void doMorphing2(IplImage* src, IplImage* dst, CvPoint P, CvPoint Q) {

	cvSet(dst, cvScalar(0, 0, 0));

	float w = src->width;
	float h = src->height;

	for (float y1 = 0.0f; y1 < src->height; y1 += 0.2f) {
		for (float x1 = 0.0f; x1 < src->width; x1 += 0.2f) {

			float a=0;
			float b=0;
			float pu[4]={0.0f};
			float pv[4]={0.0f};

			if (x1 < P.x && y1 < P.y) {
				a = x1 / P.x;
				b = y1 / P.y;
				pu[3] = Q.x-P.x;
				pv[3] = Q.y-P.y;
			}
			else if (x1 >= P.x && y1 < P.y) {
				a = (x1-P.x) / (w-P.x);
				b = y1 / P.y;
				pu[2] = Q.x - P.x;
				pv[2] = Q.y - P.y;
			}
			else if (x1 < P.x && y1 >= P.y) {
				a = x1 / P.x;
				b = (y1-P.y) / (h-P.y);
				pu[1] = Q.x - P.x;
				pv[1] = Q.y - P.y;
			}
			else if (x1 >= P.x && y1 >= P.y) {
				a = (x1 - P.x) / (w - P.x);
				b = (y1 - P.y) / (h - P.y);
				pu[0] = Q.x - P.x;
				pv[0] = Q.y - P.y;
			}

			float u = (1-a)*(1-b)*pu[0] +
						a*(1-b)*pu[1] +
						(1-a)*b*pu[2] +
						a*b*pu[3];

			float v = (1 - a) * (1 - b) * pv[0] +
						a * (1 - b) * pv[1] +
						(1 - a) * b * pv[2] +
						a * b * pv[3];

			int x2 = x1 + u;
			int y2 = y1 + v;

			if (x2<0 || x2>dst->width - 1)continue;
			if (y2<0 || y2>dst->height - 1)continue;

			CvScalar f = cvGet2D(src, y1, x1);
			cvSet2D(dst, y2, x2, f);
		}
	}
}

void myMouse(int event, int x, int y, int flag, void*) {
	if (event == CV_EVENT_LBUTTONDOWN) {
		pt1 = cvPoint(x,y);
	}
	if (event == CV_EVENT_LBUTTONUP) {
		pt2 = cvPoint(x,y);
		doMorphing2(src, dst, pt1, pt2);
		cvShowImage("img",dst);
		cvCopy(dst, src);
		
	}
}

int main() {
	src = cvLoadImage("C://tmp//lena.png");
	CvSize size = cvGetSize(src);

	dst = cvCreateImage(size, 8, 3);


	cvShowImage("img",src);
	cvSetMouseCallback("img",myMouse);
	cvWaitKey();

	return 0;
}