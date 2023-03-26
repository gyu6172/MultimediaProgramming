#include <opencv2/opencv.hpp>
double calculateImg(IplImage* src1, IplImage* src2);
void printImgInfoVertical(IplImage* img, char c);
void printImgInfoHorizontal(IplImage* img, char c);
int findBoundaryVertical(IplImage* img, int startY, int dy);
int findBoundaryHorizontal(IplImage* img, int startX, int dx);
void drawVerticalLine(IplImage* img, CvScalar color, int y);
void drawHorizontalLine(IplImage* img, CvScalar color, int x);

CvScalar BLUE = cvScalar(255, 0, 0);
CvScalar GREEN = cvScalar(0, 255, 0);
CvScalar RED = cvScalar(0, 0, 255);

#define HORIZONTALVARIANCELIMIT 250
#define VERTICALVARIANCELIMIT 350

int main() {
	//이미지 입력
	//printf("Input File Name : ");
	//char str[100];
	//scanf("%s",str);
	//IplImage *src = cvLoadImage(str);

	IplImage* src = cvLoadImage("C:\\tmp\\pg2.jpg");
	CvSize size = cvGetSize(src);

	CvSize imgSize = size;

	imgSize.height = size.height / 3;

	IplImage* bImg = cvCreateImage(imgSize, 8, 3);
	IplImage* gImg = cvCreateImage(imgSize, 8, 3);
	IplImage* rImg = cvCreateImage(imgSize, 8, 3);
	IplImage* dst = cvCreateImage(imgSize, 8, 3);

	int bHeight = findBoundaryVertical(src, cvGetSize(src).height / 3 - 200, 1);
	int gHeight = findBoundaryVertical(src, cvGetSize(src).height / 3 * 2 - 200, 1);
	int rHeight = findBoundaryVertical(src, cvGetSize(src).height - 200, 1);

	int bWidth = findBoundaryHorizontal(src, 100, -1);
	int gWidth = findBoundaryHorizontal(src, 100, -1);
	int rWidth = findBoundaryHorizontal(src, 100, -1);

	//imgSize.height = MIN(bHeight, abs(bHeight-gHeight));
	//imgSize.height = MIN(imgSize.height, abs(gHeight-rHeight));

	//원본 이미지 3등분하기
	// 
	//진화된 버전
	for (int y = imgSize.height - 1, d = 0; y >= 0; y--, d++) {
		for (int x = 0; x < imgSize.width; x++) {
			if (bHeight - d >= 0 && bWidth + x < size.width && gWidth + x < size.width && rWidth + x < imgSize.width) {
				CvScalar bc = cvGet2D(src, bHeight - d, x + bWidth);
				CvScalar gc = cvGet2D(src, gHeight - d, x + gWidth);
				CvScalar rc = cvGet2D(src, rHeight - d, x + rWidth);

				cvSet2D(bImg, y, x, bc);
				cvSet2D(gImg, y, x, gc);
				cvSet2D(rImg, y, x, rc);
			}
		}
	}



	//구버전
	//for (int y = 0; y < imgSize.height; y++) {
	//	for (int x = 0; x < imgSize.width; x++) {
	//		CvScalar bc = cvGet2D(src, y, x);
	//		CvScalar gc = cvGet2D(src, y + size.height / 3, x);
	//		CvScalar rc = cvGet2D(src, y + size.height / 3 * 2, x);

	//		cvSet2D(bImg, y, x, bc);
	//		cvSet2D(gImg, y, x, gc);
	//		cvSet2D(rImg, y, x, rc);
	//	}
	//}


	//dst
	for (int y = 0; y < imgSize.height; y++) {
		for (int x = 0; x < imgSize.width; x++) {
			CvScalar b = cvGet2D(bImg, y, x);
			CvScalar g = cvGet2D(gImg, y, x);
			CvScalar r = cvGet2D(rImg, y, x);
			CvScalar color;
			color.val[0] = b.val[0];
			color.val[1] = g.val[0];
			color.val[2] = r.val[0];
			cvSet2D(dst, y, x, color);
		}
	}

	cvShowImage("bImg", bImg);
	cvShowImage("gImg", gImg);
	cvShowImage("rImg", rImg);
	cvShowImage("dst", dst);
	//cvShowImage("src", src);


	cvWaitKey();


}

double calculateImg(IplImage* src1, IplImage* src2)
{
	double sum = 0.0;
	for (int y = 0; y < cvGetSize(src1).height; y++) {
		for (int x = 0; x < cvGetSize(src2).width; x++) {
			CvScalar c1 = cvGet2D(src1, y, x);
			CvScalar c2 = cvGet2D(src2, y, x);

			sum += abs(c1.val[0] - c2.val[0]);
		}
	}

	return sum;
}

void printImgInfoVertical(IplImage* img, char c)
{
	for (int y = 0; y < cvGetSize(img).height; y++) {
		double avg = 0.0;
		for (int x = 0; x < cvGetSize(img).width; x++) {
			avg += cvGet2D(img, y, x).val[0];
		}
		avg /= cvGetSize(img).width - 1;
		double variance = 0.0;
		for (int x = 0; x < cvGetSize(img).width; x++) {
			variance += pow(avg - cvGet2D(img, y, x).val[0], 2);
		}
		variance = sqrt(variance);
		printf("%d행 %c:%.2f\n", y, c, variance);
	}
}

void printImgInfoHorizontal(IplImage* img, char c)
{
	for (int x = 0; x < cvGetSize(img).width; x++) {
		double avg = 0.0;
		for (int y = 0; y < cvGetSize(img).height; y++) {
			avg += cvGet2D(img, y, x).val[0];
		}
		avg /= cvGetSize(img).width - 1;
		double variance = 0.0;
		for (int y = 0; y < cvGetSize(img).height; y++) {
			variance += pow(avg - cvGet2D(img, y, x).val[0], 2);
		}
		variance = sqrt(variance);
		printf("%d열 %c:%.2f\n", x, c, variance);
	}
}

int findBoundaryVertical(IplImage* img, int startY, int dy)
{
	int maxdiff = 0;
	int maxDiffY = 0;
	for (int y = startY; y < cvGetSize(img).height - 1; y += dy) {
		double avg = 0.0;
		double nextAvg = 0.0;
		for (int x = 0; x < cvGetSize(img).width; x++) {
			avg += cvGet2D(img, y, x).val[0];
			nextAvg += cvGet2D(img, y + dy, x).val[0];
		}
		avg /= cvGetSize(img).width - 1;
		nextAvg /= cvGetSize(img).width - 1;

		double variance = 0.0;
		double nextVariance = 0.0;
		for (int x = 0; x < cvGetSize(img).width; x++) {
			variance += pow(avg - cvGet2D(img, y, x).val[0], 2);
			nextVariance += pow(nextAvg - cvGet2D(img, y + dy, x).val[0], 2);
		}
		variance = sqrt(variance);
		nextVariance = sqrt(nextVariance);
		if (abs(variance - nextVariance) > VERTICALVARIANCELIMIT) {
			maxdiff = abs(variance - nextVariance);
			maxDiffY = y;
			printf("%d번행 차이 : %d\n", maxDiffY, maxdiff);
			return maxDiffY;
		}
	}
}

int findBoundaryHorizontal(IplImage* img, int startX, int dx)
{
	int maxdiff = 0;
	int maxDiffX = 0;
	for (int x = startX; x > 0; x += dx) {
		double avg = 0.0;
		double nextAvg = 0.0;
		for (int y = 0; y < cvGetSize(img).height; y++) {
			avg += cvGet2D(img, y, x).val[0];
			nextAvg += cvGet2D(img, y, x + dx).val[0];
		}
		avg /= cvGetSize(img).height - 1;
		nextAvg /= cvGetSize(img).height - 1;

		double variance = 0.0;
		double nextVariance = 0.0;
		for (int y = 0; y < cvGetSize(img).height; y++) {
			variance += pow(avg - cvGet2D(img, y, x).val[0], 2);
			nextVariance += pow(nextAvg - cvGet2D(img, y, x + dx).val[0], 2);
		}
		variance = sqrt(variance);
		nextVariance = sqrt(nextVariance);
		if (abs(variance - nextVariance) > HORIZONTALVARIANCELIMIT) {
			maxdiff = abs(variance - nextVariance);
			maxDiffX = x;
			printf("%d번열 차이 : %d\n", maxDiffX, maxdiff);
			return maxDiffX;
		}
	}
}

void drawVerticalLine(IplImage* img, CvScalar color, int y)
{
	for (int x = 0; x < cvGetSize(img).width; x++) {
		cvSet2D(img, y, x, color);
	}
}

void drawHorizontalLine(IplImage* img, CvScalar color, int x)
{
	for (int y = 0; y < cvGetSize(img).height; y++) {
		cvSet2D(img, y, x, color);
	}
}
