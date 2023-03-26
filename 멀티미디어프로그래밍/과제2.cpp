#include <opencv2/opencv.hpp>
void drawImgGraphVertical(int arr[], CvSize size, const char* str);
void drawImgGraphHorizontal(int arr[], CvSize size, const char* str);
int* getImgInfoVertical(IplImage* img, char c);
int* getImgInfoHorizontal(IplImage* img, char c);
int findBoundaryVertical(IplImage* img, int startY, int dy);
int findBoundaryHorizontal(IplImage* img, int startX, int dx);
void drawVerticalLine(IplImage* img, CvScalar color, int y);
void drawHorizontalLine(IplImage* img, CvScalar color, int x);

CvScalar BLUE = cvScalar(255, 0, 0);
CvScalar GREEN = cvScalar(0, 255, 0);
CvScalar RED = cvScalar(0, 0, 255);
CvScalar WHITE = cvScalar(255, 255, 255);
CvScalar BLACK = cvScalar(0, 0, 0);

#define HORIZONTALVARIANCELIMIT 250
#define VERTICALVARIANCELIMIT 350
#define VERTICALAVGLIMIT 50
#define SIZE 1000

typedef struct ImgInfo {
	int *rows;
	int *cols;
}ImgInfo;

int main() {
	//이미지 입력
	//printf("Input File Name : ");
	//char str[100];
	//scanf("%s",str);
	//IplImage *src = cvLoadImage(str);

	IplImage* src = cvLoadImage("C:\\tmp\\pg1.jpg");
	CvSize size = cvGetSize(src);

	int topBoundary, bottomBoundary;
	topBoundary = findBoundaryVertical(src, 2, 1);
	bottomBoundary = findBoundaryVertical(src, size.height - 2, -1);

	CvSize imgSize = size;
	CvSize newSrcSize=size;

	newSrcSize.height = bottomBoundary;
	newSrcSize.height -= topBoundary;

	imgSize.height = newSrcSize.height / 3;
	
	IplImage* bImg = cvCreateImage(imgSize, 8, 3);
	IplImage* gImg = cvCreateImage(imgSize, 8, 3);
	IplImage* rImg = cvCreateImage(imgSize, 8, 3);
	IplImage* dst = cvCreateImage(imgSize, 8, 3);

	ImgInfo bImgInfo;
	ImgInfo gImgInfo;
	ImgInfo rImgInfo;

	IplImage* newSrc = cvCreateImage(newSrcSize, 8, 3);
	for (int y = topBoundary; y < bottomBoundary; y++) {
		for (int x = 0; x < newSrcSize.width; x++) {
			CvScalar color = cvGet2D(src, y, x);

			cvSet2D(newSrc, y-topBoundary, x, color);
		}
	}

	//구버전
	for (int y = 0; y < imgSize.height; y++) {
		for (int x = 0; x < imgSize.width; x++) {
			CvScalar bc = cvGet2D(newSrc, y, x);
			CvScalar gc = cvGet2D(newSrc, y + newSrcSize.height / 3, x);
			CvScalar rc = cvGet2D(newSrc, y + newSrcSize.height / 3 * 2, x);

			cvSet2D(bImg, y, x, bc);
			cvSet2D(gImg, y, x, gc);
			cvSet2D(rImg, y, x, rc);
		}
	}

	bImgInfo.cols = getImgInfoVertical(bImg, 'b');
	rImgInfo.cols = getImgInfoVertical(gImg, 'g');
	gImgInfo.cols = getImgInfoVertical(rImg, 'r');

	bImgInfo.rows = getImgInfoHorizontal(bImg, 'b');
	rImgInfo.rows = getImgInfoHorizontal(gImg, 'g');
	gImgInfo.rows = getImgInfoHorizontal(rImg, 'r');
	
	drawImgGraphVertical(bImgInfo.cols, imgSize, "BlueVerticalGraph");
	drawImgGraphVertical(gImgInfo.cols, imgSize, "GreenVerticalGraph");
	drawImgGraphVertical(rImgInfo.cols, imgSize, "RedVerticalGraph");
	
	drawImgGraphHorizontal(bImgInfo.rows, imgSize, "BlueHorizontalGraph");
	drawImgGraphHorizontal(gImgInfo.rows, imgSize, "GreenHorizontalGraph");
	drawImgGraphHorizontal(rImgInfo.rows, imgSize, "RedHorizontalGraph");



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

void drawImgGraphVertical(int arr[], CvSize size, const char* str)
{
	IplImage *graph = cvCreateImage(size, 8, 3);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			cvSet2D(graph, y, x, WHITE);
		}
	}
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < arr[y]; x++) {
			cvSet2D(graph, y, x, BLACK);
		}
	}
	cvShowImage(str, graph);

}
void drawImgGraphHorizontal(int arr[], CvSize size, const char* str)
{
	IplImage* graph = cvCreateImage(size, 8, 3);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			cvSet2D(graph, y, x, WHITE);
		}
	}
	for (int x = 0; x < size.width; x++) {
		for (int y = 0; y<arr[x]; y++) {
			cvSet2D(graph, y, x, BLACK);
		}
	}
	cvShowImage(str, graph);

}

int* getImgInfoVertical(IplImage* img, char c)
{
	int *arr = (int*)malloc(sizeof(int)*SIZE);
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
		//printf("%d행 %c:%.2f\n", y, c, variance);
		arr[y]=(int)avg;
	}

	return arr;
}

int* getImgInfoHorizontal(IplImage* img, char c)
{
	int* arr = (int*)malloc(sizeof(int) * SIZE);
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
		//printf("%d열 %c:%.2f\n", x, c, variance);
		arr[x]=(int)avg;
	}
	return arr;
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

		if (abs(avg - nextAvg) > VERTICALAVGLIMIT) {
			printf("%d번행 차이 : %f\n", y, abs(avg-nextAvg));
			return y;
		}

		//double variance = 0.0;
		//double nextVariance = 0.0;
		//for (int x = 0; x < cvGetSize(img).width; x++) {
		//	variance += pow(avg - cvGet2D(img, y, x).val[0], 2);
		//	nextVariance += pow(nextAvg - cvGet2D(img, y + dy, x).val[0], 2);
		//}
		//variance = sqrt(variance);
		//nextVariance = sqrt(nextVariance);
		//if (abs(variance - nextVariance) > VERTICALVARIANCELIMIT) {
		//	maxdiff = abs(variance - nextVariance);
		//	maxDiffY = y;
		//	printf("%d번행 차이 : %d\n", maxDiffY, maxdiff);
		//	return maxDiffY;
		//}
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
