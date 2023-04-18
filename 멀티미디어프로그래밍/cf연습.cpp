#include <opencv2/opencv.hpp>
void drawHistogram(IplImage* histo, int arr[][3]);
int main() {
	IplImage* src = cvLoadImage("C:\\tmp\\toodark1.jpg");
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);

	int histo[256][3] = { 0, };

	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			for (int k = 0; k < 3; k++) {
				histo[int(cvGet2D(src, y, x).val[k])][k] ++;
			}
		}
	}
	//CvSize histoSize = cvSize(256, 5000);
	//IplImage* blueHisto = cvCreateImage(histoSize, 8, 3);
	//cvSet(blueHisto, cvScalar(0, 0, 0));
	//for (int x = 0; x < histoSize.width; x++) {
	//	for (int y = 0; y < histo[x][0]; y++) {
	//		cvSet2D(blueHisto, y, x, cvScalar(255, 0, 0));
	//	}
	//}
	//IplImage* greenHisto = cvCreateImage(histoSize, 8, 3);
	//cvSet(greenHisto, cvScalar(0, 0, 0));
	//for (int x = 0; x < histoSize.width; x++) {
	//	for (int y = 0; y < histo[x][1]; y++) {
	//		cvSet2D(greenHisto, y, x, cvScalar(0, 255, 0));
	//	}
	//}
	//IplImage* redHisto = cvCreateImage(histoSize, 8, 3);
	//cvSet(redHisto, cvScalar(0, 0, 0));
	//for (int x = 0; x < histoSize.width; x++) {
	//	for (int y = 0; y < histo[x][2]; y++) {
	//		cvSet2D(redHisto, y, x, cvScalar(0, 0, 255));
	//	}
	//}



	int cumul[256][3] = { 0, };
	for (int y = 0; y < 256; y++) {
		for (int k = 0; k < 3; k++) {
			if (y == 0) {
				cumul[y][k] = histo[y][k];
			}
			else {
				cumul[y][k] += cumul[y-1][k] + histo[y][k];
			}
		}
	}

	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			CvScalar g = cvScalar(0,0,0);

			for (int k = 0; k < 3; k++) {
				g.val[k] = (cumul[int(f.val[k])][k]/float(size.height*size.width))*255;
			}

			cvSet2D(dst, y, x, g);
		}
	}

	cvShowImage("src", src);
	cvShowImage("dst", dst);
	cvWaitKey();
}

void drawHistogram(IplImage* histo, int arr[][3])
{
	cvSet(histo, cvScalar(0, 0, 0));
	for (int x = 0; x < cvGetSize(histo).width; x++) {
		for (int y = 0; y < arr[x][0]; y++) {
			cvSet2D(histo, y, x, cvScalar(255, 255, 255));
		}
	}
}
