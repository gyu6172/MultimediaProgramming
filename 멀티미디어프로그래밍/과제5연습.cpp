#include <opencv2/opencv.hpp>
#include "MatrixInverse.h"
IplImage *src;
IplImage *dst;

int W,H;

int main() {
	src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);
	dst = cvCreateImage(size, 8, 3);
	cvSet(dst, cvScalar(0,0,0));
	
	W = size.width;
	H = size.height;

	float A[8][9], transA[9][8];
	float AAt[8][8], inverseAAt[8][8];
	float pseudoInverseA[9][8];
	float M[3][3];

	CvPoint p1 = cvPoint(0, 0);		//go to q1
	CvPoint p2 = cvPoint(0, H - 1);	//go to q2
	CvPoint p3 = cvPoint(W - 1, H - 1);	//go to q3
	CvPoint p4 = cvPoint(W - 1, 0);	//go to q4

	CvPoint q1 = cvPoint(50, 70);
	CvPoint q2 = cvPoint(50, 400);
	CvPoint q3 = cvPoint(500, 500);
	CvPoint q4 = cvPoint(460, 30);

	for (int i = 0; i < 4; i++) {
		CvPoint p;
		CvPoint q;
		if (i == 0) p = p1; q = q1;
		if (i == 1) p = p2; q = q2;
		if (i == 2) p = p3; q = q3;
		if (i == 3) p = p4; q = q4;
		A[2 * i][0] = -p.x;
		A[2 * i][1] = -p.y;
		A[2 * i][2] = -1;
		A[2 * i][3] = 0;
		A[2 * i][4] = 0;
		A[2 * i][5] = 0;
		A[2 * i][6] = q.x * p.x;
		A[2 * i][7] = q.x * p.y;
		A[2 * i][8] = q.x;

		A[2 * i + 1][0] = 0;
		A[2 * i + 1][1] = 0;
		A[2 * i + 1][2] = 0;
		A[2 * i + 1][3] = -p.x;
		A[2 * i + 1][4] = -p.y;
		A[2 * i + 1][5] = -1;
		A[2 * i + 1][6] = q.y * p.x;
		A[2 * i + 1][7] = q.y * p.y;
		A[2 * i + 1][8] = q.y;
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 9; j++) {
			transA[j][i] = A[i][j];
		}
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			AAt[i][j] = 0.0f;
			for (int k = 0; k < 9; k++) {
				AAt[i][j] += A[i][k] * transA[k][j];

			}
		}
	}

	InverseMatrixGJ8(AAt, inverseAAt);

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 8; j++) {
			pseudoInverseA[i][j] = 0.0f;
			for (int k = 0; k < 8; k++) {
				pseudoInverseA[i][j] += transA[i][k] * inverseAAt[k][j];
			}
		}
	}

	//printf("�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�\n");
	//for (int i = 0; i < 9; i++) {
	//	for (int j = 0; j < 8; j++) {
	//		printf("%.2f ", pseudoInverseA[i][j] * A[i][j]);
	//	}
	//	printf("\n");
	//}
	//printf("�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�\n");

	for (int i = 0; i < 9; i++) {
		M[i / 3][i % 3] = 0.0f;
		for (int j = 0; j < 8; j++) {
			M[i / 3][i % 3] += pseudoInverseA[i][j] * 0.1f;
		}
	}

	//printf("\n�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�\n");

	//for (int i = 0; i < 9; i++) {
	//	printf("%.2f ",M[i/3][i%3]);
	//}
	//printf("\n�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�\n");

	float IM[3][3];
	InverseMatrixGJ3(M, IM);

	for (int y2 = 0; y2 < H; y2++) {
		for (int x2 = 0; x2 < W; x2++) {

			float x1 = IM[0][0] * x2 + IM[0][1] * y2 + IM[0][2];
			float y1 = IM[1][0] * x2 + IM[1][1] * y2 + IM[1][2];
			float w1 = IM[2][0] * x2 + IM[2][1] * y2 + 1;

			x1 /= w1;
			y1 /= w1;

			if (x1<0 || x1>W - 1) continue;
			if (y1<0 || y1>H - 1) continue;

			CvScalar f = cvGet2D(src, y1, x1);
			cvSet2D(dst, y2, x2, f);
		}
	}


	cvLine(dst, q1, q2, cvScalar(255,0,0));
	cvLine(dst, q2, q3, cvScalar(255,0,0));
	cvLine(dst, q3, q4, cvScalar(255,0,0));
	cvLine(dst, q4, q1, cvScalar(255,0,0));

	cvShowImage("src", src);
	cvShowImage("dst", dst);
	cvWaitKey();


}