#include <opencv2/opencv.hpp>
float getDiff(CvScalar f, CvScalar g) {
	float sum = 0.0f;
	for (int k = 0; k < 3; k++) {
		sum += (f.val[k]-g.val[k])*(f.val[k] - g.val[k]);
	}
	return sum;
}

int main() {
	IplImage *src = cvLoadImage("C:\\tmp\\lena.png");
	IplImage *dst = cvCreateImage(cvGetSize(src), 8, 3);

	CvScalar colors[2] = {cvScalar(255,255,255), cvScalar(0,0,0)};
	int numColors = 2;


	for (int y1 = 0; y1 < src->height; y1++) {
		for (int x1 = 0; x1 < src->width; x1++) {
			
			CvScalar f = cvGet2D(src, y1, x1);
			
			float minDiff = FLT_MAX;
			int idx=0;
			for (int k = 0; k < numColors; k++) {
				if (minDiff > getDiff(f, colors[k])) {
					minDiff = getDiff(f, colors[k]);
					idx = k;
				}
			}

			CvScalar g = colors[idx];
			
			cvSet2D(dst, y1, x1, g);

			CvScalar d;
			for (int k = 0; k < 3; k++) {
				d.val[k] = f.val[k]-g.val[k];
			}

			if (x1 < src->width - 1) {
				CvScalar f1 = cvGet2D(src, y1, x1 + 1);
				for (int k = 0; k < 3; k++) {
					f1.val[k] += 7 / 16.0f * d.val[k];
				}
				cvSet2D(src, y1, x1 + 1, f1);
			}
			
			if (y1 < src->height - 1 && x1>0) {
				CvScalar f2 = cvGet2D(src, y1+1, x1-1);
				for (int k = 0; k < 3; k++) {
					f2.val[k] += 3 / 16.0f * d.val[k];
				}
				cvSet2D(src, y1+1, x1 - 1, f2);
			}

			if (y1 < src->height - 1) {
				CvScalar f3 = cvGet2D(src, y1+1, x1);
				for (int k = 0; k < 3; k++) {
					f3.val[k] += 5 / 16.0f * d.val[k];
				}
				cvSet2D(src, y1+1, x1, f3);

			}

			if (y1 < src->height - 1 && x1 < src->width - 1) {
				CvScalar f4 = cvGet2D(src, y1 + 1, x1 + 1);
				for (int k = 0; k < 3; k++) {
					f4.val[k] += 1 / 16.0f * d.val[k];
				}
				cvSet2D(src, y1 + 1, x1 + 1, f4);
			}


		}
	}
	
	cvShowImage("src",src);
	cvShowImage("dst",dst);
	cvWaitKey();

}