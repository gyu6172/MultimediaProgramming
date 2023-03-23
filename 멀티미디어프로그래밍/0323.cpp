#include <opencv2/opencv.hpp>

CvScalar gammaCorrection(CvScalar in, float gamma) {
	CvScalar out;
	for (int k = 0; k < 3; k++) {
		out.val[k] = pow(in.val[k] / 255.0f, gamma) * 255.0f;
	}

	return out;
}

int main() {
	IplImage* src = cvLoadImage("C:\\tmp\\111.jpg");
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);

	float gamma = 1.0f;
	while (true) {
		cvShowImage("dst", dst);
		for (int y = 0; y < size.height; y++) {
			for (int x = 0; x < size.width; x++) {
				CvScalar f = cvGet2D(src, y, x);
				CvScalar g = gammaCorrection(f, gamma);
				cvSet2D(dst, y, x, g);
			}
		}
		int key = cvWaitKey();
		if (key == '1') {
			gamma += 0.1f;
		}
		else if (key == '2') {
			gamma -= 0.1f;
		}
		else if (key == 'q') {
			break;
		}
		printf("gamma = %f\n",gamma);
	}
}

