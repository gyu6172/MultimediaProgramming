#include <opencv2/opencv.hpp>

//교수님 좋아하시는 분야의 논문집(ACM)과 학회(siggraph) 소개

float getDiff(CvScalar a, CvScalar b) {
	float sum = 0;
	for (int k = 0; k < 3; k++) {
		sum += (a.val[k] - b.val[k]) * (a.val[k] - b.val[k]);
	}
	return sum;
}

int main() {

	IplImage* src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);

	CvScalar pal[] = { cvScalar(0,0,0),
						cvScalar(255,255,255),
		//cvScalar(128,128,128),
		//cvScalar(50,50,50),
		//cvScalar(100,100,100),
		//cvScalar(200,200,200),
		cvScalar(0,0,255),
		cvScalar(0,255,0),
		cvScalar(255,0,0),
		cvScalar(255,255,0),
		cvScalar(0,255,255),
		cvScalar(255,0,255),
	};
	int numPal = 2;
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);

			int min_ind = 0;
			float min_diff = FLT_MAX;
			for (int i = 0; i < numPal; i++) {
				float diff = getDiff(pal[i], f);
				if (diff < min_diff) {
					min_diff = diff;
					min_ind = i;
				}
			}


			CvScalar g = pal[min_ind];
			cvSet2D(dst, y, x, g);

			CvScalar d;
			for (int k = 0; k < 3; k++) {
				d.val[k] = f.val[k] - g.val[k];
			}

			if (x < size.width - 1) {
				CvScalar f1 = cvGet2D(src, y, x + 1);
				for (int k = 0; k < 3; k++) {
					f1.val[k] += 7 / 16.0f * d.val[k];
				}
				cvSet2D(src, y, x + 1, f1);
			}

			if (y >= size.height - 1)continue;
			if (x > 0) {
				CvScalar f2 = cvGet2D(src, y + 1, x - 1);
				for (int k = 0; k < 3; k++) {
					f2.val[k] += 3 / 16.0f * d.val[k];
				}
				cvSet2D(src, y + 1, x - 1, f2);
			}

			CvScalar f3 = cvGet2D(src, y + 1, x);
			for (int k = 0; k < 3; k++) {
				f3.val[k] += 5 / 16.0f * d.val[k];
			}
			cvSet2D(src, y + 1, x, f3);

			if (x < size.width - 1) {

				CvScalar f4 = cvGet2D(src, y + 1, x + 1);
				for (int k = 0; k < 3; k++) {
					f4.val[k] += 1 / 16.0f * d.val[k];
				}
				cvSet2D(src, y + 1, x + 1, f4);
			}
		}
	}

	cvShowImage("src", src);
	cvShowImage("dst", dst);
	cvWaitKey();

	return 0;
}