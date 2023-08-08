#include <opencv2/opencv.hpp>
int main() {
	
	IplImage *src = cvLoadImage("C:\\tmp\\lena.jpg");		//source
	CvSize size = cvGetSize(src);
	int w = size.width;
	int h = size.height;
	if (src == nullptr) {
		printf("Image Load Error!");
		return -1;
	}
	IplImage *dst = cvCreateImage(cvGetSize(src), 8, 3);	//destination

	CvScalar c = cvScalar(200,41,61);

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			CvScalar f = cvGet2D(src, y, x);
			float bri = (c.val[0] + c.val[1] + c.val[2])/3/255.0f;

			/*CvScalar g = cvScalar(avg,avg,avg);
			if (x < w / 2) {
				g = f;
			}*/

			CvScalar g;
			for (int k = 0; k < 3; k++) {
				g.val[k] = f.val[k]*bri;
			}

			cvSet2D(dst, y, x, g);
		}
	}




	cvShowImage("src", src);
	cvShowImage("dst", dst);
	cvWaitKey();

	return 0;
}