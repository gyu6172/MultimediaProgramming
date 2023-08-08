#include <opencv2/opencv.hpp>
int main() {

	//동적할당으로 이중배열을 만들어야 크게 만들 수 있다.

	IplImage* src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);
	IplImage* mid = cvCreateImage(size, 8, 3);
	IplImage* dst = cvCreateImage(size, 8, 3);
	
	cvSmooth(src, dst, CV_BILATERAL, 0, 0, 50, 10);

	cvShowImage("src", src);
	cvShowImage("dst", dst);
	cvWaitKey();

	return 0;
}