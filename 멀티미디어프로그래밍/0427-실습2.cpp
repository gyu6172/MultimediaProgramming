#include <opencv2/opencv.hpp>

IplImage* img;
IplImage *copy;
IplImage* pal;
CvScalar color = cvScalar(0, 0, 0);

CvPoint p1 = cvPoint(0,0);

void myMouse(int event, int x, int y, int flags, void*) {
	if (event == CV_EVENT_LBUTTONDOWN ||
		event == CV_EVENT_MBUTTONDOWN ||
		event == CV_EVENT_RBUTTONDOWN
		) {
		p1 = cvPoint(x,y);
		cvCopy(img, copy);
	}
	
	if ((flags & CV_EVENT_FLAG_LBUTTON) == CV_EVENT_FLAG_LBUTTON && event == CV_EVENT_MOUSEMOVE) 
	{
		CvPoint p2 = cvPoint(x,y);
		printf("Mouse : %d (%d, %d) flag=%d\n", event, x, y, flags);
		//cvSet2D(img, y, x, color);
		cvLine(img, p1, p2, color, 3);
		cvShowImage("image", img);
		p1 = p2;
	}

	if ((flags & CV_EVENT_FLAG_MBUTTON) == CV_EVENT_FLAG_MBUTTON && event == CV_EVENT_MOUSEMOVE) {
		cvCopy(copy, img);
		CvPoint p2 = cvPoint(x, y);
		cvRectangle(img, p1, p2, color, 3);
		cvShowImage("image", img);
	}

	if ((flags & CV_EVENT_FLAG_RBUTTON) == CV_EVENT_FLAG_RBUTTON && event == CV_EVENT_MOUSEMOVE) {
		cvCopy(copy, img);
		CvPoint p2 = cvPoint(x, y);
		float r = sqrt(pow((p2.x-p1.x),2) + pow((p2.y-p1.y),2));
		cvCircle(img, p1, r, color, 3);
		cvShowImage("image", img);
	}
}

void myMouse2(int event, int x, int y, int flags, void*) {
	if (event == CV_EVENT_LBUTTONDOWN) {
		printf("Mouse2 : %d (%d, %d) flag=%d\n", event, x, y, flags);
		color = cvGet2D(pal, y, x);
	}
}

int main() {
	img = cvCreateImage(cvSize(500, 500), 8, 3);
	copy = cvCreateImage(cvSize(500, 500), 8, 3);
	pal = cvLoadImage("C:\\tmp\\palette.bmp");
	cvSet(img, cvScalar(255, 255, 255));

	CvPoint p1 = cvPoint(100, 100);
	CvPoint p2 = cvPoint(200, 200);

	//사각형의 두께를 -1로 주면 내부를 칠할 수 있다.
	cvRectangle(img, p1, p2, cvScalar(255, 0, 0), -1);

	cvLine(img, p1, p2, cvScalar(0, 0, 255), 3);

	cvCircle(img, p1, 50, cvScalar(0, 255, 0), 3);

	cvShowImage("image", img);
	cvShowImage("palette", pal);
	cvSetMouseCallback("image", myMouse);
	cvSetMouseCallback("palette", myMouse2);
	cvWaitKey();

}