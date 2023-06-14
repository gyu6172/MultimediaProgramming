#include <opencv2/opencv.hpp>
#define PI 3.141592

IplImage* src;
IplImage* dst;

void initMatrix(float M[][3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == j) {
				M[i][j] = 1.0f;
			}
			else {
				M[i][j] = 0.0f;
			}
		}
	}
}
void setTranslation(float M[][3], int tx, int ty) {
	initMatrix(M);
	M[0][2] = tx;
	M[1][2] = ty;
}
void setRotation(float M[][3], float theta) {
	initMatrix(M);
	M[0][0] = cos(theta);	M[0][1] = -sin(theta);
	M[1][0] = sin(theta);	M[1][1] = cos(theta);
}
void setMultiplyMatrix(float M[][3], float A[][3], float B[][3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			M[i][i] = 0.0f;
			for (int k = 0; k < 3; k++) {
				M[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

void applyReverseAffineTransform(IplImage* src, IplImage* dst, float M[][3]) {
	for (int y2 = 0; y2 < dst->height; y2++) {
		for (int x2 = 0; x2 < dst->width; x2++) {
			int w2 = 1;

			int x1 = M[0][0] * x2 + M[0][1] * y2 + M[0][2] * w2;
			int y1 = M[1][0] * x2 + M[1][1] * y2 + M[1][2] * w2;
			int w1 = M[2][0] * x2 + M[2][1] * y2 + M[2][2] * w2;
			
			x1 /= w1;
			y1 /= w1;

			if (x1<0 || x1>src->width - 1) continue;
			if (y1<0 || y1>src->height - 1) continue;

			CvScalar f = cvGet2D(src, y1, x1);
			cvSet2D(dst, y2, x2, f);
		}
	}
}

CvPoint P;
CvPoint Q;
float dtheta = 0.0;
void myMouse(int event, int x, int y, int flag, void*) {
	int w = dst->width;
	int h = dst->height;
	if (event == CV_EVENT_LBUTTONDOWN) {
		P = cvPoint(x, y);
		Q = cvPoint(x, y);
	}
	if (event == CV_EVENT_LBUTTONUP) {
		cvSet(dst, cvScalar(0, 0, 0));
		Q = cvPoint(x, y);

		float px = float(P.x) / (w - 1) * 2-1;
		float py = float(P.y) / (h - 1) * 2-1;

		float qx = float(Q.x) / (w - 1) * 2-1;
		float qy = float(Q.y) / (h - 1) * 2-1;

		dtheta += (atan2(qy, qx) - atan2(py, px));

		printf("theta = % .2f\n", dtheta);

		//x` = cos()*x - sin()*y
		//y` = sin()*x + cos()*y

		//for (int y2 = 0; y2 < dst->height; y2++) {
		//	for (int x2 = 0; x2 < dst->width; x2++) {

		//		float nx = float(x2) / (dst->width - 1) * 2 - 1;
		//		float ny = float(y2) / (dst->height - 1) * 2 - 1;
		//		
		//		float x1 = cos(-dtheta) * nx - sin(-dtheta) * ny;
		//		float y1 = sin(-dtheta) * nx + cos(-dtheta) * ny;
		//		
		//		x1 = (x1 + 1) / 2 * (dst->width - 1);
		//		y1 = (y1 + 1) / 2 * (dst->height - 1);

		//		if (x1<0 || x1>src->width - 1) continue;
		//		if (y1<0 || y1>src->height - 1) continue;

		//		CvScalar f = cvGet2D(src, y1, x1);
		//		cvSet2D(dst, y2, x2, f);
		//	}
		//}
		//cvShowImage("dst", dst);

		float IM[3][3], T1[3][3], T2[3][3], R[3][3], IM1[3][3];

		//B = T1 * R * T2 *A
		//M = T1 * R * T2
		//IM = T2 * R * T1
		
		setMultiplyMatrix(IM1, T1, R);
		setMultiplyMatrix(IM, )
		

		cvShowImage("dst", dst);
	}
}

int main() {
	src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);
	dst = cvCreateImage(size, 8, 3);
	cvCopy(src, dst);

	cvShowImage("dst", dst);
	cvSetMouseCallback("dst", myMouse);
	cvWaitKey();
}