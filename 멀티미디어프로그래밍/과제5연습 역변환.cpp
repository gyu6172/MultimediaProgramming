#include <opencv2/opencv.hpp>
#include "MatrixInverse.h"
IplImage* src;
IplImage* dst;

int W, H;

int main() {
	src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);
	dst = cvCreateImage(size, 8, 3);
	cvSet(dst, cvScalar(0, 0, 0));

	W = size.width;
	H = size.height;

	float A[8][9];

	CvPoint p1 = cvPoint(1, 1);		//go to q1
	CvPoint p2 = cvPoint(1, H - 1);	//go to q2
	CvPoint p3 = cvPoint(W - 1, H - 1);	//go to q3
	CvPoint p4 = cvPoint(W - 1, 1);	//go to q4

	CvPoint q1 = cvPoint(50, 50);
	CvPoint q2 = cvPoint(50, 300);
	CvPoint q3 = cvPoint(300, 500);
	CvPoint q4 = cvPoint(300, 200);

	//���ڰ� Ŀ���ϱ� ������ �����ؾ� �ϳ�?

	for (int i = 0; i < 4; i++) {
		CvPoint p,q;
		switch (i) {
			case 0:
				p = p1;
				q = q1;
				break;
			case 1:
				p = p2;
				q = q2;
				break;
			case 2:
				p = p3;
				q = q3;
				break;
			case 3:
				p = p4;
				q = q4;
				break;
		}

		A[2 * i][0] = float(-p.x) / (W-1);
		A[2 * i][1] = float(-p.y) / (H-1);
		A[2 * i][2] = -1;
		A[2 * i][3] = 0;
		A[2 * i][4] = 0;
		A[2 * i][5] = 0;
		A[2 * i][6] = float(q.x) / (W-1) * float(p.x) / (W-1);
		A[2 * i][7] = float(q.x) / (W-1) * float(p.y) / (H-1);
		A[2 * i][8] = float(q.x) / (W-1);

		A[2 * i + 1][0] = 0;
		A[2 * i + 1][1] = 0;
		A[2 * i + 1][2] = 0;
		A[2 * i + 1][3] = float(-p.x) / (W-1);
		A[2 * i + 1][4] = float(-p.y) / (H-1);
		A[2 * i + 1][5] = -1;
		A[2 * i + 1][6] = float(q.y) / (H-1) * float(p.x) / (W-1);
		A[2 * i + 1][7] = float(q.y) / (H-1) * float(p.y) / (H-1);
		A[2 * i + 1][8] = float(q.y) / (H-1);
		
	}

	printf("�ѤѤѤѤѤѤѤѤѤѤѤѤ�A�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�\n");
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 9; j++) {
			printf("%f\t", A[i][j]);
		}
		printf("\n");
	}
	printf("�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�\n");

	float transA[9][8];
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 8; j++) {
			transA[i][j] = A[j][i];
		}
	}

	printf("�ѤѤѤѤѤѤѤѤѤѤѤѤ�transA�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�\n");
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 8; j++) {
			printf("%f\t", transA[i][j]);
		}
		printf("\n");
	}
	printf("�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�\n");

	float AAt[8][8];
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			AAt[i][j] = 0.0f;
			for (int k = 0; k < 9; k++) {
				AAt[i][j] += A[i][k]*transA[k][j];
			}
		}
	}

	float invAAt[8][8];
	InverseMatrixGJ8(AAt, invAAt);

	printf("�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�AAt�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�\n");
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			printf("%.1f\t", AAt[i][j]);
		}
		printf("\n");
	}
	
	printf("�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�invAAt�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�\n");
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			printf("%.1f\t", invAAt[i][j]);
		}
		printf("\n");

	}

	float tmp[8][8];
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			tmp[i][j] = 0.0f;
			for (int k = 0; k < 8; k++) {
				tmp[i][j] += AAt[i][k] * invAAt[k][j];
			}
		}
	}
	printf("�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�tmp�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�\n");
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			printf("%.1f\t", tmp[i][j]);
		}
		printf("\n");
	}
	printf("�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�\n");




	float pseudoInvA[9][8];
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 8; j++) {
			pseudoInvA[i][j] = 0.0f;

			for (int k = 0; k < 8; k++) {
				pseudoInvA[i][j] += transA[i][k] * invAAt[k][j];
			}
		}
	}

	float M[3][3];
	for (int i = 0; i < 9; i++) {
		float rst = 0.0f;
		for (int j = 0; j < 8; j++) {
			rst += pseudoInvA[i][j];
		}
		M[i/3][i%3] = rst;
	}

	printf("�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�pseudoInvA�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�\n");
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 8; j++) {
			printf("%.1f\t", pseudoInvA[i][j]);
		}
		printf("\n");
	}

	printf("�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�M�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�\n");
	for (int i = 0; i < 9; i++) {
		printf("%.3f ", M[i/3][i%3]);
	}

	for (int y1 = 0; y1 < H; y1++) {
		for (int x1 = 0; x1 < W; x1++) {

			float x2 = M[0][0] * (float(x1) / (W - 1)) + M[0][1] * (float(y1) / (H - 1)) + M[0][2];
			float y2 = M[1][0] * (float(x1) / (W - 1)) + M[1][1] * (float(y1) / (H - 1)) + M[1][2];
			float w2 = M[2][0] * (float(x1) / (W - 1)) + M[2][1] * (float(y1) / (H - 1)) + M[2][2];

			x2 /= w2;
			y2 /= w2;

			x2 *= (W - 1);
			y2 *= (H - 1);

			//printf("x2:%.2f, y2:%.2f, w2:%.2f\n", x2, y2, w2);

			if (x2<0 || x2>W - 1) continue;
			if (y2<0 || y2>H - 1) continue;

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