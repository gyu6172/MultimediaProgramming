#include <opencv2/opencv.hpp>
int main() {
	IplImage* src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);

	int h = size.height;
	int w = size.width;

	CvScalar** srcArr = (CvScalar**)malloc(sizeof(CvScalar*) * h);
	for (int y = 0; y < h; y++) {
		srcArr[y] = (CvScalar*)malloc(sizeof(CvScalar) * w);
	}
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			srcArr[y][x] = cvGet2D(src, y, x);
		}
	}

	CvScalar** srcSAT = (CvScalar**)malloc(sizeof(CvScalar*) * h);
	for (int y = 0; y < h; y++) {
		srcSAT[y] = (CvScalar*)malloc(sizeof(CvScalar) * w);
	}

	//SAT[y][x] = SAT[y-1][x] + SAT[y][x-1] - SAT[y-1][x-1] + Arr[y][x]
	srcSAT[0][0] = srcArr[0][0];
	for (int k = 0; k < 3; k++) {
		for (int x = 1; x < w; x++) {
			srcSAT[0][x].val[k] = srcSAT[0][x-1].val[k] + srcArr[0][x].val[k];
		}
		for (int y = 1; y < h; y++) {
			srcSAT[y][0].val[k] = srcSAT[y-1][0].val[k] + srcArr[y][0].val[k];
		}
		for (int y = 1; y < h; y++) {
			for (int x = 1; x < w; x++) {
				srcSAT[y][x].val[k] = srcSAT[y - 1][x].val[k] + srcSAT[y][x - 1].val[k] - srcSAT[y - 1][x - 1].val[k] + srcArr[y][x].val[k];
			}
		}
	}

	int k = 50;
	int ksize = 2 * k + 1;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			CvScalar g;
			for (int d = 0; d < 3; d++) {
				int kw=ksize, kh=ksize;
				int xk=x+k, yk=y+k;
				if (x + k >= w) {
					kw -= xk-(w-1);
					xk=w-1;
				}
				if (y + k >= h) {
					kh -= yk-(h-1);
					yk=h-1;
				}
				g.val[d] += srcSAT[yk][xk].val[d];	

				if (x-k-1 >= 0) {
					g.val[d] -= srcSAT[yk][x - k - 1].val[d];
				}
				else {
					kw -= k-x;
				}
				if (y-k-1 >= 0) {
					g.val[d] -= srcSAT[y - k - 1][xk].val[d];
				}
				else {
					kh -= k-y;
				}
				if (x - k - 1 >= 0 && y - k - 1 >= 0) {
					g.val[d] += srcSAT[y-k-1][x-k-1].val[d];
				}
				g.val[d] /= (kw*kh);
			}
			cvSet2D(dst, y, x, g);
		}
	}


	cvShowImage("src", src);
	cvShowImage("dst", dst);
	cvWaitKey();


	for (int y = 0; y < h; y++) {
		free(srcArr[y]);
		free(srcSAT[y]);
	}
	free(srcArr);
	free(srcSAT);
	return 0;
}