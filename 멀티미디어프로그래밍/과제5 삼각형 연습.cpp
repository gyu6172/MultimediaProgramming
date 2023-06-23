#include <opencv2/opencv.hpp>
#include "MatrixInverse.h"
IplImage* src;
IplImage* dst;


float determinant(float matrix[][8], int N) {
	int i, j, k;
	float det = 0;
	float submatrix[8][8];

	// 2x2 행렬일 경우
	if (N == 2) {
		return (matrix[0][0] * matrix[1][1]) - (matrix[1][0] * matrix[0][1]);
	}
	// 그 이상의 행렬일 경우
	else {
		for (i = 0; i < N; i++) {
			int subi = 0; // 부분 행렬의 행 인덱스
			for (j = 1; j < N; j++) {
				int subj = 0; // 부분 행렬의 열 인덱스
				for (k = 0; k < N; k++) {
					if (k == i) {
						continue;
					}
					submatrix[subi][subj] = matrix[j][k];
					subj++;
				}
				subi++;
			}
			det = det + (matrix[0][i] * pow(-1, i) * determinant(submatrix, N - 1));
		}
	}
	return det;
}

int W, H;

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

int main() {
	src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);
	dst = cvCreateImage(size, 8, 3);
	cvSet(dst, cvScalar(0, 0, 0));

	W = size.width;
	H = size.height;

	CvPoint p1 = cvPoint(1, 1);		//go to q1
	CvPoint p2 = cvPoint(1, H - 1);	//go to q2
	CvPoint p3 = cvPoint(W - 1, H - 1);	//go to q3
	CvPoint p4 = cvPoint(W - 1, 1);	//go to q4

	CvPoint q1 = cvPoint(50, 50);
	CvPoint q2 = cvPoint(50, 300);
	CvPoint q3 = cvPoint(300, 500);
	CvPoint q4 = cvPoint(300, 200);

	float A[3][3] = {{p1.x, p2.x, p4.x},{p1.y, p2.y, p4.y},{1, 1, 1}};
	float B[3][3] = {{q1.x, q2.x, q4.x},{q1.y, q2.y, q4.y},{1, 1, 1}};
	float invA[3][3];
	float M[3][3];

	float invB[3][3];
	InverseMatrixGJ3(B, invB);

	setMultiply(M, A, invB);

	for (int y2 = 0; y2 < H; y2++) {
		for (int x2 = 0; x2 < W; x2++) {

			float x1 = M[0][0] * x2 + M[0][1] * y2 + M[0][2];
			float y1 = M[1][0] * x2 + M[1][1] * y2 + M[1][2];
			float w1 = M[2][0] * x2 + M[2][1] * y2 + M[2][2];

			x1 /= w1;
			y1 /= w1;

			//printf("x2:%.2f, y2:%.2f, w2:%.2f\n", x2, y2, w2);

			if (x1<0 || x1>W - 1) continue;
			if (y1<0 || y1>H - 1) continue;

			CvScalar f = cvGet2D(src, y1, x1);
			cvSet2D(dst, y2, x2, f);
		}
	}

	cvLine(dst, q1, q2, cvScalar(255, 0, 0));
	cvLine(dst, q2, q3, cvScalar(255, 0, 0));
	cvLine(dst, q3, q4, cvScalar(255, 0, 0));
	cvLine(dst, q4, q1, cvScalar(255, 0, 0));

	cvShowImage("src", src);
	cvShowImage("dst", dst);
	cvWaitKey();


}