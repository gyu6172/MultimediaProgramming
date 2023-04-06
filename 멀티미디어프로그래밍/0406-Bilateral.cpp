#include <opencv2/opencv.hpp>
int main() {

	//�����Ҵ����� ���߹迭�� ������ ũ�� ���� �� �ִ�.

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