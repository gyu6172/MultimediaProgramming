#include <opencv2/opencv.hpp>
#define PI 3.14159265
IplImage* src;
IplImage* dst;
IplImage* copy;
IplImage* tmpImg;

CvPoint p1;
CvPoint p2;

void initMatrix(float M[][3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == j) M[i][j] = 1.0f;
			else M[i][j] = 0.0f;
		}
	}
}

void setScaleMatrix(float M[][3], float sx, float sy) {
	initMatrix(M);
	M[0][0] = sx;
	M[1][1] = sy;
}

void setRotateMatrix(float M[][3], float degree) {
	initMatrix(M);
	M[0][0] = cos(degree);
	M[0][1] = -sin(degree);
	M[1][0] = sin(degree);
	M[1][1] = cos(degree);
}

void setTranslatingMatrix(float M[][3], int tx, int ty) {
	initMatrix(M);
	M[0][2] = tx;
	M[1][2] = ty;
}
void applyInverseAffineTransform(IplImage* src, IplImage* dst, float IM[][3]) {
	cvSet(dst, cvScalar(0, 0, 0));
	//역변형
	for (int y2 = 0; y2 < dst->height; y2++) {
		for (int x2 = 0; x2 < dst->width; x2++) {
			int w2 = 1.0f;

			int x1 = IM[0][0] * x2 + IM[0][1] * y2 + IM[0][2] * w2;
			int y1 = IM[1][0] * x2 + IM[1][1] * y2 + IM[1][2] * w2;
			int w1 = IM[2][0] * x2 + IM[2][1] * y2 + IM[2][2] * w2;

			x1 /= w1;
			y1 /= w1;

			if (x1<0 || x1>src->width - 1) continue;
			if (y1<0 || y1>src->height - 1) continue;

			CvScalar f = cvGet2D(src, y1, x1);
			cvSet2D(dst, y2, x2, f);
		}
	}
}
void applyAffineTransform(IplImage* src, IplImage* dst, float M[][3]) {
	cvSet(dst, cvScalar(0, 0, 0));
	for (int y1 = 0; y1 < src->height; y1++) {
		for (int x1 = 0; x1 < src->width; x1++) {
			int w1 = 1.0f;

			int x2 = M[0][0] * x1 + M[0][1] * y1 + M[0][2] * w1;
			int y2 = M[1][0] * x1 + M[1][1] * y1 + M[1][2] * w1;
			int w2 = M[2][0] * x1 + M[2][1] * y1 + M[2][2] * w1;

			x2 /= w2;
			y2 /= w2;

			if (x2<0 || x2>dst->width - 1) continue;
			if (y2<0 || y2>dst->height - 1) continue;

			CvScalar f = cvGet2D(src, y1, x1);
			cvSet2D(dst, y2, x2, f);
		}
	}
}
void setMultiplyMatrix(float M[][3], float A[][3], float B[][3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			M[i][j] = 0.0f;

			for (int k = 0; k < 3; k++) {
				M[i][j] += A[i][k] * B[k][j];

			}
		}
	}
}

//dst의 mid좌표에 src이미지를 반지름이 r인 원으로 theta만큼 돌려서 그림
void myCircle(IplImage* src, IplImage* dst, CvPoint mid, int r, int theta) {
	CvPoint lt = cvPoint(mid.x - r, mid.y - r);
	CvPoint rb = cvPoint(mid.x + r, mid.y + r);

	float dx = rb.x - lt.x+0.000001;
	float dy = rb.y - lt.y+0.000001;

	for (int y = lt.y; y <= rb.y; y++) {
		for (int x = lt.x; x <= rb.x; x++) {

			float nx = float(x - lt.x) / dx * (src->width - 1);
			float ny = float(y - lt.y) / dy * (src->height - 1);

			if (nx<0 || nx>src->width - 1) continue;
			if (ny<0 || ny>src->height - 1) continue;
			if (x<=0 || x>dst->width - 1) continue;
			if (y<=0 || y>dst->height - 1) continue;

			float snx = nx / (src->width - 1) * 2 - 1;
			float sny = ny / (src->height - 1) * 2 - 1;

			float x1 = cos(theta) * snx - sin(theta) * sny;
			float y1 = sin(theta) * snx + cos(theta) * sny;

			x1 = (x1 + 1) * (src->width - 1) / 2;
			y1 = (y1 + 1) * (src->height- 1) / 2;

			if (snx * snx + sny * sny < 1) {

				CvScalar f = cvGet2D(src, y1, x1);
				
				cvSet2D(dst, y, x, f);
			}
		}
	}
	cvShowImage("dst", dst);
}

//1. scale 조정
//2. 이미지 중앙을 (0,0)으로 옮김
//3. rotation
//4. 이미지 중앙을 mid로 옮김
void myRectangle(IplImage* src, IplImage* dst, CvPoint mid, int r, int theta) {
	CvPoint lt = cvPoint(mid.x - r, mid.y - r);
	CvPoint rb = cvPoint(mid.x + r, mid.y + r);

	float dx = rb.x - lt.x + 0.0001;
	float dy = rb.y - lt.y + 0.0001;

	for (int y = lt.y; y <= rb.y; y++) {
		for (int x = lt.x; x <= rb.x; x++) {

			//nx : 0~src.width-1
			float nx = float(x - lt.x) / dx * (src->width - 1);
			float ny = float(y - lt.y) / dy * (src->height - 1);

			//sx : -1 ~ 1 (in Src)
			float sx = nx / (src->width - 1) * 2 - 1;
			float sy = ny / (src->height - 1) * 2 - 1;

			//rx : 돌아간 x in src
			float rx = cos(theta) * sx - sin(theta) * sy;
			float ry = sin(theta) * sx + cos(theta) * sy;

			//x1 : 0~src.width-1
			int x1 = (rx + 1) * (src->width - 1) / 2;
			int y1 = (ry + 1) * (src->height - 1) / 2;

			if (x1<0 || x1>src->width - 1) continue;
			if (y1<0 || y1>src->height - 1) continue;

			CvScalar f = cvGet2D(src, y1, x1);

			cvSet2D(dst, y, x, f);
		}
	}

	cvShowImage("dst", dst);
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
			float theta = atan2(p1.y - p2.y, p2.x - p1.x);
			//printf("theta=%.2f\n", theta * 180 / PI);
			
			myCircle(src, dst, p1, r, theta);
			//myRectangle(src, dst, p1, r, theta);

			
		}
	}

}
int main() {
	src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);
	dst = cvCreateImage(size, 8, 3);
	copy = cvCreateImage(size, 8, 3);
	tmpImg = cvCreateImage(size, 8, 3);
	cvSet(dst, cvScalar(255, 255, 255));

	cvShowImage("dst", dst);
	cvSetMouseCallback("dst", myMouse);
	cvWaitKey();

}