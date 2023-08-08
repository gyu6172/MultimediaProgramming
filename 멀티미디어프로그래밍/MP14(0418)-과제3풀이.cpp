#include <opencv2/opencv.hpp>

void mySmooth(IplImage* src, IplImage* dst, int k) {
	//1. build SAT(Summed-Area-Table)
	//2. mean Filtering

	int height = cvGetSize(src).height;
	int width = cvGetSize(src).width;

	//1. buildSAT
	CvScalar **SAT = new CvScalar*[height];
	for (int i = 0; i < height; i++) {
		SAT[i] = new CvScalar[width];
	}

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			CvScalar A = (y>0)? SAT[y-1][x] : cvScalar(0,0,0);
			CvScalar B = (x>0)? SAT[y][x-1] : cvScalar(0, 0, 0);
			CvScalar C = (y>0 && x>0)? SAT[y-1][x-1] : cvScalar(0, 0, 0);

			CvScalar f = cvGet2D(src, y, x);
			for (int k = 0; k < 3; k++) {
				SAT[y][x].val[k] = A.val[k] + B.val[k] - C.val[k] + f.val[k];
			}
		}
	}

	//2. mean Filtering
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			CvScalar g = cvScalar(0,0,0);
			int x1 = x-k-1;	if(x1 < 0) x1 = -1;
			int y1 = y-k-1; if(y1 < 0) y1 = -1;
			int x2 = x+k;	if(x2 > width-1) x2 = width-1;
			int y2 = y+k;	if(y2 > height-1) y2 = height-1;

			CvScalar A = cvScalar(0,0,0);
			CvScalar B = cvScalar(0,0,0);
			CvScalar C = cvScalar(0,0,0);
			CvScalar D = cvScalar(0,0,0);

			A = SAT[y2][x2];

			if(y1>=0) B = SAT[y1][x2];
			if(x1>=0) C = SAT[y2][x1];
			if(x1>=0 && y1>=0) D = SAT[y1][x1];

			for (int k = 0; k < 3; k++) {
				g.val[k] = (A.val[k] - B.val[k] - C.val[k] + D.val[k])/((x2-x1)*(y2-y1));
			}

			cvSet2D(dst, y, x, g);
		}
	}


	//3. memory release
	for (int i = 0; i < height; i++) {
		delete SAT[i];
	}
	delete[] SAT;

}

int main() {
	IplImage *src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);
	IplImage *dst = cvCreateImage(size, 8, 3);

	mySmooth(src, dst, 50);

	cvShowImage("src", src);
	cvShowImage("dst", dst);
	cvWaitKey();
	return 0;
}