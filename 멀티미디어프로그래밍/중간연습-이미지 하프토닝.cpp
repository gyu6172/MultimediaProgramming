#include <opencv2/opencv.hpp>
float getDiff(CvScalar f, CvScalar g) {
	float sum=0;
	for (int k = 0; k < 3; k++) {
		sum += pow((f.val[k] - g.val[k]),2);
	}
	return sum;
}

void thresholdingImage(IplImage *src, CvScalar threshArr[], int size);
void patterningImage(IplImage *src, int patternArr[], int arrSize);
void ditheringImage(IplImage *src, CvScalar threshArr[], int size);

int main() {
	IplImage *src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);

	IplImage *ditheringImg = cvCreateImage(size, 8, 3);

	cvShowImage("src", src);

	CvScalar thresholding[5] = { cvScalar(0,0,0),
							cvScalar(255,255,255),
							cvScalar(128,128,128),
							cvScalar(64,64,64),
							cvScalar(192,192,192),
							};
	int threshCnt = 5;
	thresholdingImage(src, thresholding, threshCnt);


	int pattern[9]={1,8,0,6,2,5,7,3,4};
	int patternSize = 9;
	patterningImage(src, pattern, patternSize);

	ditheringImage(src, thresholding, 5);

	cvWaitKey();
}

void thresholdingImage(IplImage* src, CvScalar threshArr[], int size)
{
	int height = cvGetSize(src).height;
	int width= cvGetSize(src).width;

	IplImage* threshImg = cvCreateImage(cvGetSize(src), 8, 3);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			CvScalar g;

			float minDiff = FLT_MAX;
			int minDiffIdx = 0;
			for (int idx = 0; idx < size; idx++) {
				if (minDiff > getDiff(f, threshArr[idx])) {
					minDiff = getDiff(f, threshArr[idx]);
					minDiffIdx = idx;
				}
			}

			g = threshArr[minDiffIdx];

			cvSet2D(threshImg, y, x, g);
		}
	}
	cvShowImage("thresh", threshImg);
}

void patterningImage(IplImage* src, int patternArr[], int arrSize)
{

	CvSize size = cvGetSize(src);
	IplImage* patternImg = cvCreateImage(size, 8, 3);

	int height = size.height;
	int width = size.width;

	cvSet(patternImg, cvScalar(0,0,0));

	for (int y = 0; y < height-2; y += 3) {
		for (int x = 0; x < width-2; x += 3) {

			CvScalar f = cvGet2D(src,y,x);

			for (int k = 0; k < 3; k++) {
				//0 <= paintCnt <= 10

				int paintCnt = (f.val[k]/255.0f)*(arrSize+1);

				if (paintCnt > arrSize) {
					paintCnt = arrSize;
				}
				for (int idx = 0; idx < paintCnt; idx++) {
					int nx = x + patternArr[idx] % 3;
					int ny = y + patternArr[idx] / 3;

					CvScalar g = cvGet2D(patternImg,ny,nx);
					g.val[k] = 255;

					cvSet2D(patternImg, ny, nx, g);
					
				}
			}
		}
	}

	cvShowImage("pattern", patternImg);
}

void ditheringImage(IplImage* src, CvScalar threshArr[], int size)
{
	int height = cvGetSize(src).height;
	int width = cvGetSize(src).width;

	IplImage* ditherImg = cvCreateImage(cvGetSize(src), 8, 3);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			CvScalar g;

			float minDiff = FLT_MAX;
			int minDiffIdx = 0;
			for (int idx = 0; idx < size; idx++) {
				if (minDiff > getDiff(f, threshArr[idx])) {
					minDiff = getDiff(f, threshArr[idx]);
					minDiffIdx = idx;
				}
			}

			g = threshArr[minDiffIdx];
			cvSet2D(ditherImg, y, x, g);

			//d : f�� g�� ��Ⱚ�� ���̸� �����ϴ� ����
			CvScalar d = cvScalar(0,0,0);
			for (int k = 0; k < 3; k++) {
				d.val[k] = f.val[k] - g.val[k];
			}

			if (x + 1 < width) {
				CvScalar h = cvGet2D(src, y, x+1);
				for (int k = 0; k < 3; k++) {
					h.val[k] += d.val[k]/16*7;
				}
				cvSet2D(src, y, x+1, h);
			}
			if (x - 1 > 0 && y+1<height) {
				CvScalar h = cvGet2D(src, y+1, x - 1);
				for (int k = 0; k < 3; k++) {
					h.val[k] += d.val[k] / 16 * 3;
				}
				cvSet2D(src, y + 1, x - 1, h);
			}
			if (y + 1 < height) {
				CvScalar h = cvGet2D(src, y+1, x);
				for (int k = 0; k < 3; k++) {
					h.val[k] += d.val[k] / 16 * 5;
				}
				cvSet2D(src, y + 1, x, h);
			}
			if (x + 1 < width && y + 1 < height) {
				CvScalar h = cvGet2D(src, y+1, x + 1);
				for (int k = 0; k < 3; k++) {
					h.val[k] += d.val[k] / 16 * 1;
				}
				cvSet2D(src, y + 1, x + 1, h);
			}

		}
	}
	cvShowImage("dithering", ditherImg);
}
