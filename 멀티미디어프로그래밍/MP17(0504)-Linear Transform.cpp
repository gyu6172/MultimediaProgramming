#include <opencv2/opencv.hpp>
#define PI 3.1415926535897932
void setRotation(float M[][2], float theta) {
	float rad = theta * (2 * PI / 360);
	M[0][0] = cos(rad);
	M[0][1] = -sin(rad);
	M[1][0] = sin(rad);
	M[1][1] = cos(rad);
}
void setScale(float M[][2], float sx, float sy) {
	M[0][0] = sx;	M[0][1] = 0;
	M[1][0] = 0;	M[1][1] = sy;
}
void setMultiply(float M[][2], float A[][2], float B[][2]) {
	M[0][0] = A[0][0]*B[0][0] + A[0][1]*B[1][0];
	M[0][1] = A[0][0]*B[0][1] + A[0][1]*B[1][1];
	M[1][0] = A[1][0]*B[0][0] + A[1][1]*B[1][0];
	M[1][1] = A[1][0]*B[0][1] + A[1][1]*B[1][1];
	
}
int main() {
	IplImage *src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvSize(800,800);
	IplImage *dst = cvCreateImage(size, 8, 3);
	cvSet(dst, cvScalar(0,0,0));

	float theta = 30;	//degree(각도)	근본 없음
						//radian(라디안)	근본 있음

	float M[2][2];
	//theta만큼 그림을 회전시킬 수 있는 행렬 M을 채워준다.
	//setRotation(M, theta);
	
	//그림을 가로, 세로로 늘리거나 줄일 수 있는 행렬 M을 구하는 함수.
	//setScale(M, 1.5, 0.5);

	float A[2][2], B[2][2];
	setRotation(A, theta);
	setScale(B, 1.5, 0.5);

	setMultiply(M, A, B);	//M = A x B;

	//정변형
	for (int y1 = 0; y1 < src->height; y1++) {
		for (int x1 = 0; x1 < src->width; x1++) {
			float x2 = x1*M[0][0] + y1*M[0][1];
			float y2 = x1*M[1][0] + y1*M[1][1];

			if(x2<0 || x2>dst->width-1)continue;
			if(y2<0 || y2>dst->height-1)continue;

			CvScalar f = cvGet2D(src,y1,x1);
			cvSet2D(dst, y2, x2, f);
		}
	}



	//역변형
	//for (int y2 = 0; y2 < dst->height - 1; y2++) {
	//	for (int x2 = 0; x2 < dst->width - 1; x2++) {
	//		float x1 = x2 * cos(rad) + y2 * sin(rad);
	//		float y1 = -x2 * sin(rad) + y2 * cos(rad);

	//		if (x1<0 || x1>src->width - 1) continue;
	//		if (y1<0 || y1>src->height - 1) continue;

	//		CvScalar f = cvGet2D(src, y1, x1);
	//		cvSet2D(dst, y2, x2, f);
	//	}
	//}


	cvShowImage("src",src);
	cvShowImage("dst",dst);
	cvWaitKey();
	return 0;
}