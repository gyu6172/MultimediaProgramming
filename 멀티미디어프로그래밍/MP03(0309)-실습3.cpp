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

	int idx = 0;
	while (true) {
		cvShowImage("Image", imgs[idx]);
		int key = cvWaitKey();
		if (key == '1') {
			idx=0;
		}
		else if (key == '2') {
			idx=1;
		}
		else if (key == 'q') {
			break;
		}
	}

	cvReleaseImage(&imgs[0]);
	cvReleaseImage(&imgs[1]);
}