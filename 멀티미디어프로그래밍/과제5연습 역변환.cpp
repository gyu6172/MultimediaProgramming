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

	//숫자가 커지니까 비율로 접근해야 하나?

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

	printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡAㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 9; j++) {
			printf("%f\t", A[i][j]);
		}
		printf("\n");
	}
	printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");

	float transA[9][8];
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 8; j++) {
			transA[i][j] = A[j][i];
		}
	}

	printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡtransAㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 8; j++) {
			printf("%f\t", transA[i][j]);
		}
		printf("\n");
	}
	printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");

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

	printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡAAtㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			printf("%.1f\t", AAt[i][j]);
		}
		printf("\n");
	}
	
	printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡinvAAtㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
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
	printf("det(AAt) : %.2f\n", determinant(AAt, 8));
	printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡtmpㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			printf("%.1f\t", tmp[i][j]);
		}
		printf("\n");
	}
	printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");




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

	printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡpseudoInvAㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 8; j++) {
			printf("%.1f\t", pseudoInvA[i][j]);
		}
		printf("\n");
	}

	printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡMㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			printf("%.3f ", M[i][j]);
		}
		printf("\n");
	}

	printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡA * A+ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			float rst = 0.0f;
			for (int k = 0; k < 9; k++) {
				rst += A[i][k]*pseudoInvA[k][j];
			}
			printf("%.3f\t",rst);
		}
		printf("\n");
	}

	for (int y1 = 0; y1 < H; y1++) {
		for (int x1 = 0; x1 < W; x1++) {

			float x2 = M[0][0] * (float(x1) / (W - 1)) + M[0][1] * (float(y1) / (H - 1)) + M[0][2];
			float y2 = M[1][0] * (float(x1) / (W - 1)) + M[1][1] * (float(y1) / (H - 1)) + M[1][2];
			float w2 = M[2][0] * (float(x1) / (W - 1)) + M[2][1] * (float(y1) / (H - 1)) + M[2][2];

			//x2 /= w2;
			//y2 /= w2;

			x2 *= -(W - 1);
			y2 *= -(H - 1);

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