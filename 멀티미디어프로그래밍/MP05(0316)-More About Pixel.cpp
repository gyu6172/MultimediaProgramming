#include <opencv2/opencv.hpp>
int main() {
	CvSize size;
	size.width = 500;
	size.height = 400;

	IplImage *img = cvCreateImage(size, 8,3);

	CvScalar c1 = cvScalar(0,255,0);
	CvScalar c2 = cvScalar(0, 0, 255);

	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			float nx = float(x)/(size.width-1);
			float ny = y;

			float a = nx;	//0ÀÌ¸é ÆÄ¶û, 1ÀÌ¸é »¡°­

			CvScalar c = cvScalar(0,0,0);

			for (int i = 0; i < 3; i++) {
				c.val[i] = a*c1.val[i] + (1-a)*c2.val[i];
			}


			float b = 255.0f * nx;
			float g = 0;
			float r = 255.0f-b;

			cvSet2D(img, y, x, c);
		}
	}

	cvShowImage("image", img);
	cvWaitKey();

	return 0;
}