#include <opencv2/opencv.hpp>
int main() {
	
	//동적할당으로 이중배열을 만들어야 크게 만들 수 있다.

	IplImage* src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);
	IplImage* mid = cvCreateImage(size, 8, 3);
	IplImage* dst = cvCreateImage(size, 8, 3);

	int Kn = 10;
	int total = 2 * Kn + 1;
	for (int y = Kn; y < size.height - Kn; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar g = cvScalar(0, 0, 0);
			for (int v = -Kn; v <= Kn; v++) {
				CvScalar f = cvGet2D(src, y + v, x);
				for (int k = 0; k < 3; k++) {
					g.val[k] += f.val[k] / total;
				}

			}
			cvSet2D(mid, y, x, g);
			
		}
	}
	
	cvShowImage("src", src);
	cvShowImage("mid", mid);
	cvWaitKey();

	for (int y = Kn; y < size.height - Kn; y++) {
		for (int x = Kn; x < size.width-Kn; x++) {
			CvScalar g = cvScalar(0, 0, 0);
			for (int u =-Kn; u <= Kn; u++) {
				CvScalar f = cvGet2D(src, y, x+u);
				for (int k = 0; k < 3; k++) {
					g.val[k] += f.val[k] / total;
				}

			}
			cvSet2D(dst, y, x, g);

		}
	}
	cvShowImage("dst", dst);
	cvWaitKey();

	return 0;
}