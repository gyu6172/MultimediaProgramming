#include <opencv2/opencv.hpp>
#define size 2

int main() {
	IplImage* imgs[size];

	imgs[0] = cvLoadImage("C:\\tmp\\sample.jpg");
	imgs[1] = cvLoadImage("C:\\tmp\\sample2.jpg");
	for (int i = 0; i < size; i++) {
		if (imgs[i] == nullptr) {
			printf("Load Error!");
			return -1;
		}
	}

	cvShowImage("Image1", imgs[0]);
	cvWaitKey(1000);
	cvShowImage("Image1", imgs[1]);
	cvWaitKey(1000);

	cvReleaseImage(&imgs[0]);
	cvReleaseImage(&imgs[1]);
}