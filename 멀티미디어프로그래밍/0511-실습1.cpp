#include <opencv2/opencv.hpp>
#define PI 3.1415926535897932
void setIdentity(float M[][3]) {
	M[0][0] = 1.0f;	M[0][1] = 0.0f;	M[0][2] = 0.0f;
	M[1][0] = 0.0f;	M[1][1] = 1.0f;	M[1][2] = 0.0f;
	M[2][0] = 0.0f;	M[2][1] = 0.0f;	M[2][2] = 1.0f;
}
void setScale(float M[][3], float sx, float sy) {
	setIdentity(M);
	M[0][0] = sx;
	M[1][1] = sy;
}
void setRotation(float M[][3], float angle) {
	setIdentity(M);
	float rad = angle / 360.0f * 2 * PI;
	M[0][0] = cos(rad);	M[0][1] = -sin(rad);
	M[1][0] = sin(rad);	M[1][1] = cos(rad);
}
void setMultiply(float M[][3], float A[][3], float B[][3]) {
	//1. 각각 대입
	/*M[0][0] = A[0][0] * B[0][0] + A[0][1] * B[1][0];
	M[0][1] = A[0][0] * B[0][1] + A[0][1] * B[1][1];
	M[1][0] = A[1][0] * B[0][0] + A[1][1] * B[1][0];
	M[1][1] = A[1][0] * B[0][1] + A[1][1] * B[1][1];*/

	//2. 일반화하여 for문으로 대입(행렬의 크기가 증가할때 사용하기 좋다.)
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			M[i][j] = 0.0f;
			for (int k = 0; k < 3; k++) {
				M[i][j] += A[i][k] * B[k][j];

			}
		}
	}
}
void setTranslate(float M[][3], float tx, float ty) {
	setIdentity(M);
	M[0][2] = tx;
	M[1][2] = ty;
}
void setInverse(float IM[][2], float M[][2]) {
	float a = M[0][0];
	float b = M[0][1];
	float c = M[1][0];
	float d = M[1][1];

	float det = a * d - b * c;

	IM[0][0] = d / det;		IM[0][1] = -c / det;
	IM[1][0] = -b / det;	IM[1][1] = a / det;

}

int main() {
	IplImage* src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvSize(800, 800);
	IplImage* dst = cvCreateImage(size, 8, 3);
	cvSet(dst, cvScalar(0, 0, 0));

	float theta = 0;
	float cx = src->width/2;
	float cy = src->height/2;
	
	float sc = 1.0f;

	//M = Tc2 * R * Tc1
	while (true) {
		float S[3][3], R[3][3], TC1[3][3], TC2[3][3];
		setTranslate(TC1, cx, cy);
		setScale(S, 1/sc, 1/sc);
		setRotation(R, -theta);
		setTranslate(TC2, -cx, -cy);

		//IM = Tc1 * R * Tc2
		float IM[3][3], IM1[3][3], IM2[3][3];
		setMultiply(IM1, TC1, S);
		setMultiply(IM2, R, TC2);
		setMultiply(IM, IM1, IM2);


		for (int y2 = 0; y2 < dst->height; y2++) {
			for (int x2 = 0; x2 < dst->width; x2++) {
				float w2 = 1.0f;

				float x1 = IM[0][0] * x2 + IM[0][1] * y2 + IM[0][2] * w2;
				float y1 = IM[1][0] * x2 + IM[1][1] * y2 + IM[1][2] * w2;
				float w1 = IM[2][0] * x2 + IM[2][1] * y2 + IM[2][2] * w2;

				//"니가 얻은 좌표는 w1배 뻥튀기 되어 있는 좌표 였어."
				x1 /= w1;
				y1 /= w1;

				if (x1<0 || x1>src->width - 1)	continue;
				if (y1<0 || y1>src->height - 1)	continue;

				CvScalar f = cvGet2D(src, y1, x1);
				cvSet2D(dst, y2, x2, f);
			}
		}


		cvShowImage("dst", dst);
		cvWaitKey(30);
		cvSet(dst, cvScalar(0,0,0));
		theta += 10;
		sc = 0.5 * cos(theta / 360.0 * 2 * 3.141592 * 2) + 1;
	}

}