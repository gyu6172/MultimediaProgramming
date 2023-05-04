#include <opencv2/opencv.hpp>
typedef struct Stroke {
	int radius;
	CvScalar color;
	CvPoint startPoint;
	CvPoint route[];
}Stroke;
typedef struct Circle {
	int radius;
	CvScalar color;
	CvPoint point;
}Circle;
typedef struct Grid{
	int colsCnt;	//격자판의 가로 개수
	int rowsCnt;	//격자판의 세로 개수
	int width;		//격자 하나의 가로 길이(너비)
	int height;		//격자 하나의 세로 길이(높이)
};

void drawCircle(IplImage* img, Circle circle) {
	cvCircle(img, circle.point, circle.radius, circle.color, -1);
}

int main() {
	IplImage* srcImg = cvLoadImage("C:\\tmp\\lena.png");
	CvSize imgSize = cvGetSize(srcImg);
	IplImage* refImg = cvCreateImage(imgSize, 8, 3);
	IplImage* canvas = cvCreateImage(imgSize, 8, 3);

	cvSmooth(srcImg, refImg, CV_GAUSSIAN, 11);

	Grid jitterdGrid;
	jitterdGrid.width = 64;
	jitterdGrid.height = 64;
	jitterdGrid.colsCnt = (imgSize.width) / (jitterdGrid.width);
	jitterdGrid.rowsCnt = (imgSize.height) / (jitterdGrid.height);

	cvShowImage("src",srcImg);
	cvShowImage("ref",refImg);



	cvWaitKey();
	
}