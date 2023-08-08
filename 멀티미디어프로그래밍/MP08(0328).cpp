#include <opencv2/opencv.hpp>
void drawHistogram(const char* name, int histo[][3], int hmax) {
	IplImage *img = cvCreateImage(cvSize(256,500), 8, 3);
	cvSet(img,cvScalar(0, 0, 0));

	for (int k = 0; k < 3; k++) {
		for (int x = 0; x < 256; x++) {
			int h = histo[x][k] / float(hmax) * img->height;
			if (h > img->height) h = img->height;
			for (int y = 0; y < h; y++) {
				CvScalar c = cvGet2D(img, img->height-1-y,x);
				c.val[k] = 255;
				cvSet2D(img, img->height - 1 - y, x, c);
			}
		}
	}

	cvShowImage(name, img);
}
int main() {
	IplImage* src = cvLoadImage("C:\\tmp\\histogram2.jpg");

	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);

	int histo[256][3] = { 0 };

	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar c = cvGet2D(src, y, x);
			for (int k = 0; k < 3; k++) {
				histo[(int)c.val[k]][k]++;
			}

		}
	}

	drawHistogram("histo", histo, 3500);

	int cumul[256][3] = {0};
	cumul[0][0] = histo[0][0];
	cumul[0][1] = histo[0][1];
	cumul[0][2] = histo[0][2];
	for (int l = 1; l < 256; l++) {
		for (int k = 0; k < 3; k++) {
			cumul[l][k] += cumul[l-1][k] + histo[l][k];
		}
	}

	int total = size.height * size.width;
	drawHistogram("cumul", cumul, size.width * size.height);

	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			CvScalar g;

			for (int k = 0; k < 3; k++) {
				g.val[k] = cumul[(int)f.val[k]][k]/float(total)*255;
			}

			cvSet2D(dst, y, x, g);
		}
	}

	cvShowImage("src", src);
	cvShowImage("dst", dst);
	cvWaitKey();

	//cvSaveImage("C:\\tmp\\result1.jpg", dst);

	return 0;
}