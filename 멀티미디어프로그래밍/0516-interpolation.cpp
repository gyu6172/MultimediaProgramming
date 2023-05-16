#include <opencv2/opencv.hpp>

CvScalar myGet2D(IplImage* img, float y, float x) {
	int j1 = (int)x;
	int i1 = (int)y;
	float a = x-j1;
	float b = y-i1;

	int j2 = j1+1;		if(j2>img->width-1) j2 = img->width-1;
	int i2 = i1+1;		if(i2>img->height-1) i2 = img->height-1;

	CvScalar f1 = cvGet2D(img, i1,j1);
	CvScalar f2 = cvGet2D(img, i1,j2);
	CvScalar f3 = cvGet2D(img, i2,j1);
	CvScalar f4 = cvGet2D(img, i2,j2);

	CvScalar f = cvScalar(0,0,0);

	for (int k = 0; k < 3; k++) {
		f.val[k] = (1-a)*(1-b)*f1.val[k]+
					(a)*(1-b)*f2.val[k]+
					(1-a)*(b)*f3.val[k]+
					(a)*(b)*f4.val[k];
	}


	return f;
}

int main() {
	IplImage* src = cvLoadImage("C:\\tmp\\lena.png");
	IplImage* dst = cvCreateImage(cvGetSize(src),8,3);

	float sx = 10;
	float sy = 10;

	int cx = src->width/2;
	int cy = src->height/2;

	for (int y2 = 0; y2 < dst->height; y2++) {
		for (int x2 = 0; x2 < dst->width; x2++) {

			float x1 = (x2-cx)/sx + cx;
			float y1 = (y2-cy)/sy + cy;

			//x2 = sx * (x1-cx) + cx
			//y2 = sy * (y1-cy) + cy

			if(x1<0 || x1>src->width-1) continue;
			if(y1<0 || y1>src->height-1) continue;

			//CvScalar f = cvGet2D(src,y1,x1);
			CvScalar f = myGet2D(src, y1, x1);
			cvSet2D(dst,y2,x2,f);
		}
	}

	cvShowImage("src",src);
	cvShowImage("dst",dst);
	cvWaitKey();
}