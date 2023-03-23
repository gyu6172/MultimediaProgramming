#include <opencv2/opencv.hpp>
int main() {
	//이미지 입력
	//printf("Input File Name : ");
	//char str[100];
	//scanf("%s",str);

	IplImage *src = cvLoadImage("C:\\tmp\\pg3.jpg");
	CvSize size = cvGetSize(src);
	CvSize imgSize = size;

	imgSize.height = size.height/3;

	IplImage *bImg = cvCreateImage(imgSize, 8, 3);
	IplImage* gImg = cvCreateImage(imgSize, 8, 3);
	IplImage* rImg = cvCreateImage(imgSize, 8, 3);
	IplImage *dst = cvCreateImage(imgSize, 8, 3);



	//Blue
	for (int y = 0; y < imgSize.height; y++) {
		for (int x = 0; x < imgSize.width; x++) {
			CvScalar color = cvGet2D(src, y, x);
			cvSet2D(bImg, y, x, color);
		}
	}
	//Green
	for (int y = 0; y < imgSize.height; y++) {
		for (int x = 0; x < imgSize.width; x++) {
			CvScalar color = cvGet2D(src, y+size.height/3, x);
			cvSet2D(gImg, y, x, color);
		}
	}
	//Red
	for (int y = 0; y < imgSize.height; y++) {
		for (int x = 0; x < imgSize.width; x++) {
			CvScalar color = cvGet2D(src, y+size.height/3*2, x);
			cvSet2D(rImg, y, x, color);
		}
	}

	//dst
	for (int y = 0; y < imgSize.height; y++) {
		for (int x = 0; x < imgSize.width; x++) {
			CvScalar b = cvGet2D(bImg, y, x);
			CvScalar g = cvGet2D(gImg, y, x);
			CvScalar r = cvGet2D(rImg, y, x);
			CvScalar color;
			color.val[0] = b.val[0];
			color.val[1] = g.val[1];
			color.val[2] = r.val[2];
			cvSet2D(dst, y, x, color);
		}
	}

	//cvShowImage("bImg", bImg);
	//cvShowImage("gImg", gImg);
	//cvShowImage("rImg", rImg);
	cvShowImage("dst", dst);


	cvWaitKey();


}