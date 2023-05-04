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
	int colsCnt;	//�������� ���� ����
	int rowsCnt;	//�������� ���� ����
	int width;		//���� �ϳ��� ���� ����(�ʺ�)
	int height;		//���� �ϳ��� ���� ����(����)
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