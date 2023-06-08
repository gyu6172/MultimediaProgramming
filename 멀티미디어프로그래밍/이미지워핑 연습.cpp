#include <opencv2/opencv.hpp>
#define PI 3.1415926535897932
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

void setRotateMatrix(float M[][3], float theta) {
	initMatrix(M);
	float radian = theta * (PI/180);
	M[0][0] = cos(radian);
	M[0][1] = -sin(radian);
	M[1][0] = sin(radian);
	M[1][1] = cos(radian);
}

void setTranslatingMatrix(float M[][3], int tx, int ty) {
	initMatrix(M);
	M[0][2] = tx;
	M[1][2] = ty;
}

void copyMatrix(float M[][3], float src[][3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			M[i][j] = src[i][j];
		}
	}
}

int main() {
	IplImage* src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);
	int w = size.width;
	int h = size.height;
	IplImage* dst = cvCreateImage(size, 8, 3);
	cvSet(dst, cvScalar(0,0,0));

	//Scailing : x는 1.4배, y는 0.7배
	float S[3][3];
	setScaleMatrix(S, 1.4, 0.7);

	//Rotating : 30도 만큼 돌리기
	float R[3][3];
	setRotateMatrix(R, 30);

	//Translating : x는 +20, y는 +50
	float T[3][3];
	setTranslatingMatrix(T, 20, 30);
	
	float M[3][3];
	copyMatrix(M, T);

	for (int y1 = 0; y1 < h; y1++) {
		for (int x1 = 0; x1 < w; x1++) {
			int w1 = 1;

			int x2 = M[0][0]*x1 + M[0][1]*y1 + M[0][2]*w1;
			int y2 = M[1][0]*x1 + M[1][1] * y1 + M[1][2] * w1;
			int w2 = M[2][0] * x1 + M[2][1] * y1 + M[2][2] * w1;

			x2 /= w2;
			y2 /= w2;

			if(x2<0 || x2>w-1) continue;
			if(y2<0 || y2>h-1) continue;

			CvScalar f = cvGet2D(src, y1, x1);
			cvSet2D(dst, y2, x2, f);
		}
	}


	cvShowImage("src",src);
	cvShowImage("dst",dst);
	cvWaitKey();
}