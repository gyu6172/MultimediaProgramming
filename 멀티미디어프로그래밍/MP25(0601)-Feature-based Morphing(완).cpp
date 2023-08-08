#include <opencv2/opencv.hpp>

struct Line {
	CvPoint P;
	CvPoint Q;
};

IplImage* dst;
IplImage* src1;
IplImage* src2;
IplImage* src1Cpy;
IplImage* src2Cpy;

Line A[100];
int numA = 0;
Line B[100];
int numB = 0;

void drawLine(IplImage* img, Line l[], int num, CvScalar c) {
	for (int i = 0; i < num; i++) {
		cvLine(img, l[i].P, l[i].Q, c, 3);
	}
}

void myMouse1(int event, int x, int y, int flag, void*) {
	if (event == CV_EVENT_LBUTTONDOWN) {
		A[numA].P = cvPoint(x, y);
	}
	if (event == CV_EVENT_LBUTTONUP) {
		A[numA].Q = cvPoint(x, y);
		numA++;
		printf("Num Line A = %d\n", numA);
		cvCopy(src1, src1Cpy);
		drawLine(src1Cpy, A, numA, cvScalar(0, 0, 255));
		//cvLine(src, A.P, A.Q, cvScalar(0, 0, 255), 3);
		cvShowImage("src1", src1Cpy);
	}
}

void myMouse2(int event, int x, int y, int flag, void*) {
	if (event == CV_EVENT_LBUTTONDOWN) {
		B[numB].P = cvPoint(x, y);
	}
	if (event == CV_EVENT_LBUTTONUP) {
		B[numB].Q = cvPoint(x, y);
		numB++;
		printf("Num Line B = %d\n", numB);
		cvCopy(src2, src2Cpy);
		drawLine(src2Cpy, B, numB, cvScalar(255, 0, 0));
		//cvLine(dst, B.P, B.Q, cvScalar(255, 0, 0), 3);
		cvShowImage("src2", src2Cpy);
	}
}

float getTransformed(Line A, Line B, float x1, float y1, float* x2, float* y2) {
	float u;	//A의 P와 Q 관점에서 (x1,y1)가 얼마나 가있는지
	float v;	//A의 P와 Q 관점에서 (x1,y1)가 수직방향으로 얼마나 가있는지

	float XPx = x1 - A.P.x;
	float XPy = y1 - A.P.y;

	float QPx = A.Q.x - A.P.x;
	float QPy = A.Q.y - A.P.y;

	float QPlen = sqrt(QPx * QPx + QPy * QPy);
	u = (XPx * QPx + XPy * QPy) / (QPlen * QPlen);	//u = dot(X-P, Q-P)/|Q-P|^2



	float perQPx = -QPy;
	float perQPy = QPx;
	v = (XPx * perQPx + XPy * perQPy) / (QPlen);

	float nQPx = B.Q.x - B.P.x;
	float nQPy = B.Q.y - B.P.y;
	float nPerQPx = -nQPy;
	float nPerQPy = nQPx;
	float nPerQPlen = sqrt(nQPx * nQPx + nQPy * nQPy);

	*x2 = B.P.x + u * nQPx + v * nPerQPx / nPerQPlen;
	*y2 = B.P.y + u * nQPy + v * nPerQPy / nPerQPlen;

	float dist = abs(v);
	if (u < 0) {
		//dist = P와의 거리
		dist = sqrt(XPx * XPx + XPy * XPy);
	}
	else if (u > 1) {
		//dist = Q와의 거리
		float XQx = x1 - A.Q.x;
		float XQy = y1 - A.Q.y;
		dist = sqrt(XQx * XQx + XQy * XQy);
	}
	float w = QPlen / (dist + 0.0001f);
	return w;
}

void doMorphing(IplImage* src, IplImage* dst, Line A[], Line B[], int num) {
	for (int y1 = 0; y1 < src->height; y1++) {
		for (int x1 = 0; x1 < src->width; x1++) {
			float x_sum = 0.0f;
			float y_sum = 0.0f;
			float w_sum = 0.0f;
			for (int i = 0; i < num; i++) {
				float x2, y2;
				float w = getTransformed(A[i], B[i], x1, y1, &x2, &y2);
				x_sum += w * x2;
				y_sum += w * y2;
				w_sum += w;
			}

			x_sum /= w_sum;
			y_sum /= w_sum;

			if (x_sum<0 || x_sum>dst->width - 1) continue;
			if (y_sum<0 || y_sum>dst->height - 1) continue;

			CvScalar f = cvGet2D(src, y1, x1);
			cvSet2D(dst, y_sum, x_sum, f);
		}
	}
}

void doMorphing2(IplImage* src, IplImage* dst, Line A[], Line B[], int num) {
	for (int y2 = 0; y2 < dst->height; y2++) {
		for (int x2 = 0; x2 < dst->width; x2++) {
			float x_sum = 0.0f;
			float y_sum = 0.0f;
			float w_sum = 0.0f;
			for (int i = 0; i < num; i++) {
				float x1, y1;
				float w = getTransformed(B[i], A[i], x2, y2, &x1, &y1);
				x_sum += w * x1;
				y_sum += w * y1;
				w_sum += w;
			}

			x_sum /= w_sum;
			y_sum /= w_sum;

			if (x_sum<0 || x_sum>src->width - 1) continue;
			if (y_sum<0 || y_sum>src->height - 1) continue;

			CvScalar f = cvGet2D(src, y_sum, x_sum);
			cvSet2D(dst, y2, x2, f);
		}
	}
}

int main() {

	src1 = cvLoadImage("C:\\tmp\\코알라.jpg");
	src2 = cvLoadImage("C:\\tmp\\2.jpg");
	CvSize size = cvGetSize(src1);
	dst = cvCreateImage(size, 8, 3);
	src1Cpy = cvCreateImage(size, 8, 3);
	src2Cpy = cvCreateImage(size, 8, 3);

	cvShowImage("src1", src1);
	cvSetMouseCallback("src1", myMouse1);

	cvShowImage("src2", src2);
	cvSetMouseCallback("src2", myMouse2);
	cvWaitKey();

	IplImage* mid1 = cvCreateImage(size, 8, 3);
	IplImage* mid2 = cvCreateImage(size, 8, 3);

	Line M[100];
	int num = numA;
	for (float alpha = 0.0f; alpha <= 1.0f; alpha += 0.05f){
		for (int i = 0; i < num; i++) {
			M[i].P.x = A[i].P.x * (1 - alpha) + B[i].P.x * alpha;
			M[i].P.y = A[i].P.y * (1 - alpha) + B[i].P.y * alpha;
			M[i].Q.x = A[i].Q.x * (1 - alpha) + B[i].Q.x * alpha;
			M[i].Q.y = A[i].Q.y * (1 - alpha) + B[i].Q.y * alpha;
		}

		doMorphing2(src1, mid1, A, M, num);
		doMorphing2(src2, mid2, B, M, num);
		//cvShowImage("mid1", mid1);
		//cvShowImage("mid2", mid2);
		//cvWaitKey();

		for (int y = 0; y < dst->height; y++) {
			for (int x = 0; x < dst->height; x++) {
				CvScalar f1 = cvGet2D(mid1, y, x);
				CvScalar f2 = cvGet2D(mid2, y, x);

				CvScalar g;
				for (int k = 0; k < 3; k++) {
					g.val[k] = (1 - alpha) * f1.val[k] + alpha * f2.val[k];
				}

				cvSet2D(dst, y, x, g);
			}
		}

		cvShowImage("dst", dst);
		cvWaitKey();
		printf("alpha=%.2f\n", alpha);
		
	}
	//doMorphing2(src, dst, A, B, numA);
	//drawLine(dst, B, numB, cvScalar(255, 0, 0));
	//cvShowImage("dst", dst);
	//cvWaitKey();


	return 0;
}