#include <opencv2/opencv.hpp>
float getDistance(CvScalar a, CvScalar b) {
	float sum=0.0;
	for (int k = 0; k < 3; k++) {
		sum += pow(a.val[k]-b.val[k], 2);
	}
	return sum;
}

int main() {
	IplImage *src1 = cvLoadImage("c:\\tmp\\hw2_1.jpg");
	IplImage *src2 = cvLoadImage("c:\\tmp\\hw2_2.jpg");
	CvSize size = cvGetSize(src1);

	IplImage *dst = cvCreateImage(size, 8, 3);

	int h = size.height;
	int w = size.width;

	int min_u=0;
	int min_v=0;
	float min_avg = FLT_MAX;

	for (int v = -h / 4; v < h / 4; v++) {
		for (int u = -w / 4; u < w / 4; u++) {
			float sum = 0.0;
			int count = 0;
			for (int y = 0; y < h; y++) {
				for (int x = 0; x < w; x++) {
					if(x+u<0 || x+u>w-1)continue;
					if(y+v<0 || y+v>h-1)continue;
					CvScalar f1 = cvGet2D(src1, y, x);
					CvScalar f2 = cvGet2D(src2, y + v, x + u);
					sum += getDistance(f1, f2);
					count++;
				}
			}
			float avg = sum / count;
			if (avg < min_avg) {
				min_avg = avg;
				min_u = u;
				min_v = v;
				printf("u=%d, v=%d, avg_error = %f\n", u,v,avg);
			}
		}
	}




	cvShowImage("src1", src1);
	cvShowImage("src2", src2);
	cvShowImage("dst", dst);

	cvWaitKey();

	return 0;
}