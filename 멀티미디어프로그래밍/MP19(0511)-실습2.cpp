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

void applyInverseAffineTransform(IplImage* src, IplImage* dst, float IM[][3]) {
	cvSet(dst, cvScalar(0, 0, 0));
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
}

IplImage* src, *dst;
CvPoint pt1, pt2;
float theta = 0.0f;
float scale = 1.0f;

void myMouse(int event, int x, int y, int flags, void*) {
	if (event == CV_EVENT_LBUTTONDOWN) {
		pt1 = cvPoint(x,y);
		
	}
	if (event == CV_EVENT_LBUTTONUP) {
		pt2 = cvPoint(x, y);
		float phi1 = atan2(pt1.y, pt1.x);
		float phi2 = atan2(pt2.y, pt2.x);

		float len1 = sqrt(pt1.x * pt1.x + pt1.y * pt1.y);
		float len2 = sqrt(pt2.x * pt2.x + pt2.y * pt2.y);
		scale *= len2/len1;

		theta += (phi2 - phi1)/PI * 180.0f;
		float IR[3][3], IS[3][3];
		float IM[3][3];
		setRotation(IR, -theta);
		setScale(IS, 1/scale, 1/scale);
		setMultiply(IM, IS, IR);
		applyInverseAffineTransform(src, dst, IM);
		cvShowImage("dst", dst);
	}
}

int main() {
	src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);
	dst = cvCreateImage(size, 8, 3);
	cvSet(dst, cvScalar(0, 0, 0));

	float theta = 0;
	float IM[3][3];

	setRotation(IM, -theta);
	applyInverseAffineTransform(src, dst, IM);
	cvShowImage("dst", dst);
	cvSetMouseCallback("dst", myMouse);
	cvWaitKey();


}