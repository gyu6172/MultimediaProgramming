#include <opencv2/opencv.hpp>
#define PI 3.1415926535897932

IplImage* src;
IplImage* dst;

CvPoint p1,p2;

float theta = 0.0f;
float sx = 1.0f;
float sy = 1.0f;

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
	float radian = theta * (PI / 180);
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

void setMultiply(float M[][3], int* x, int* y, int* w, int ex, int ey, int ew) {
	*x = M[0][0] * ex + M[0][1] * ey + M[0][2] * ew;
	*y = M[1][0] * ex + M[1][1] * ey + M[1][2] * ew;
	*w = M[2][0] * ex + M[2][1] * ey + M[2][2] * ew;
}

void setInverseMatrix(float matrix[3][3], float inverse[3][3]) {
	float determinant = 0;

	// ����� ��Ľ� ���
	for (int i = 0; i < 3; i++) {
		determinant += (matrix[0][i] * (matrix[1][(i + 1) % 3] * matrix[2][(i + 2) % 3] - matrix[1][(i + 2) % 3] * matrix[2][(i + 1) % 3]));
	}

	// ��Ľ��� 0�̸� ������� �������� ����
	if (determinant == 0) {
		printf("������� �������� �ʽ��ϴ�.\n");
		return;
	}

	// ����� ���
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			inverse[i][j] = ((matrix[(j + 1) % 3][(i + 1) % 3] * matrix[(j + 2) % 3][(i + 2) % 3]) - (matrix[(j + 1) % 3][(i + 2) % 3] * matrix[(j + 2) % 3][(i + 1) % 3])) / determinant;
		}
	}
}

void applyInverseAffineTransform(IplImage* src, IplImage* dst, float IM[][3]) {
	cvSet(dst, cvScalar(0,0,0));
	//������
	for (int y2 = 0; y2 < dst->height; y2++) {
		for (int x2 = 0; x2 < dst->width; x2++) {
			int w2 = 1.0f;

			int x1;
			int y1;
			int w1;
			setMultiply(IM, &x1, &y1, &w1, x2, y2, w2);

			x1 /= w1;
			y1 /= w1;

			if (x1<0 || x1>src->width - 1) continue;
			if (y1<0 || y1>src->height - 1) continue;

			CvScalar f = cvGet2D(src, y1, x1);
			cvSet2D(dst, y2, x2, f);
		}
	}
}

void rotationImage(int event, int x, int y, int flag, void*) {
	if (event == CV_EVENT_LBUTTONDOWN) {
		p1 = cvPoint(x,y);
		p2 = cvPoint(x,y);
	}
	if (event == CV_EVENT_LBUTTONUP) {
		p2 = cvPoint(x,y);
		
		int w = src->width;
		int h = src->height;

		float px = p1.x/float(w)*2-1;
		float py = p1.y/float(h)*2-1;
		float qx = p2.x/float(w)*2-1;
		float qy = p2.y/float(h)*2-1;
		

		//atan2 : arctan() �������� ����
		float degree_p = atan2(py, px);
		float degree_q = atan2(qy, qx);

		float d_theta = (degree_q - degree_p)/PI*180.0f;
		theta += d_theta;


		//���� : ���콺 �巡�׸� �ϸ� �������� ��ŭ ������, �����ϵ� �����ϱ�
		float middleT[3][3];
		setTranslatingMatrix(middleT, -w / 2, -h / 2);

		float middleR[3][3];
		setRotateMatrix(middleR, theta);

		float middleT1[3][3];
		setTranslatingMatrix(middleT1, w / 2, h / 2);

		float M[3][3], M1[3][3], M2[3][3];
		float IM[3][3];

		//B = T1 * R * T * A
		setMultiplyMatrix(M1, middleT, middleR);
		setMultiplyMatrix(M, middleT1, M1);
		setInverseMatrix(M, IM);

		//M = T1 * R * T
		//IM = T * R * T1
		//setRotateMatrix(middleR, -theta);
		//setMultiplyMatrix(M1, middleT1, middleR);
		//setMultiplyMatrix(IM, M1, middleT);

		applyInverseAffineTransform(src, dst, IM);

		cvShowImage("dst",dst);

	}
}

int main() {
	src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);
	int w = size.width;
	int h = size.height;
	dst = cvCreateImage(size, 8, 3);
	cvSet(dst, cvScalar(0, 0, 0));

	float IM[3][3];

	setRotateMatrix(IM, -theta);
	applyInverseAffineTransform(src, dst, IM);
	cvShowImage("dst", dst);
	cvSetMouseCallback("dst", rotationImage);
	cvWaitKey();

}