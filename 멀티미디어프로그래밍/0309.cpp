#include <opencv2/opencv.hpp>

int main() {
	IplImage *img = cvLoadImage("C:\\tmp\\sample.jpg");
	if (img == nullptr) {
		printf("File Not Found!");
		return -1;
	}

	printf("w = %d\nh = %d\n",img->width, img->height);

	cvShowImage("Hello Image1", img);
	cvShowImage("Hello Image2", img);



	char input1 = cvWaitKey(1000);
	cvDestroyWindow("Hello Image2");
	char input2 = cvWaitKey();

	printf("input = %c",input2);


	cvSaveImage("C:\\tmp\\sample.bmp",img);

	cvReleaseImage(&img);

	return 0;
}