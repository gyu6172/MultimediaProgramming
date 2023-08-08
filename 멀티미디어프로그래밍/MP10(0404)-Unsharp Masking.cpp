#include <opencv2/opencv.hpp>
int main() {
	IplImage *src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);
	IplImage *dst = cvCreateImage(size, 8, 3);
	IplImage *blur = cvCreateImage(size, 8, 3);
	IplImage *diff = cvCreateImage(size, 8, 3);

	//src - blur = diff
	//blur + diff = src
	//cvSmooth(src, blur, CV_GAUSSIAN, 51);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			int bri = (f.val[0] + f.val[1] + f.val[2])/3;
			CvScalar g = cvScalar(bri,bri,bri);

			cvSet2D(blur, y, x, g);
		}
	}

	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			CvScalar b = cvGet2D(blur, y, x);

			CvScalar d;
			for (int k = 0; k < 3; k++) {
				d.val[k] = f.val[k] - b.val[k] + 128;
			}
			cvSet2D(diff, y, x, d);
		}
	}

	for (float alpha = 0.0f; alpha <= 3.0f; alpha += 0.1f) {
		for (int y = 0; y < size.height; y++) {
			for (int x = 0; x < size.width; x++) {
				CvScalar b = cvGet2D(blur, y, x);
				CvScalar d = cvGet2D(diff, y, x);

				CvScalar g;
				for (int k = 0; k < 3; k++) {
					g.val[k] = b.val[k] + alpha * (d.val[k] - 128);
				}
				cvSet2D(dst, y, x, g);
			}
		}
		cvShowImage("dst", dst);
		printf("alpha = %.2f\n",alpha);
		cvWaitKey();
	}
	



	cvShowImage("src",src);
	
	cvShowImage("blur",blur);
	cvShowImage("diff",diff);
	cvWaitKey();

	return 0;
}