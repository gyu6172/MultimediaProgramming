#include <opencv2/opencv.hpp>
typedef struct ImgInfo {
	int** arr;
	int* rows;
	int* cols;
}ImgInfo;
void drawImgGraphVertical(int arr[], CvSize size, const char* str);
void drawImgGraphHorizontal(int arr[], CvSize size, const char* str);
void printImgInfo(ImgInfo info);
int** getImgInfo(IplImage *img);
int* getImgInfoVertical(IplImage* img, char c);
int* getImgInfoHorizontal(IplImage* img, char c);
int findBoundaryVertical(IplImage* img, int startY, int dy);
int findBoundaryHorizontal(IplImage* img, int startX, int dx);
void drawVerticalLine(IplImage* img, CvScalar color, int y);
void drawHorizontalLine(IplImage* img, CvScalar color, int x);
int calculateArrDiff(int arr1[], int arr2[], int size);
int* getArr(int **imgArr, int x, int size);

CvScalar BLUE = cvScalar(255, 0, 0);
CvScalar GREEN = cvScalar(0, 255, 0);
CvScalar RED = cvScalar(0, 0, 255);
CvScalar WHITE = cvScalar(255, 255, 255);
CvScalar BLACK = cvScalar(0, 0, 0);

#define HORIZONTALVARIANCELIMIT 250
#define VERTICALVARIANCELIMIT 350
#define VERTICALAVGLIMIT 50
#define SIZE 1000



int main() {
	//이미지 입력
	//printf("Input File Name : ");
	//char str[100];
	//scanf("%s",str);
	//IplImage *src = cvLoadImage(str);
	IplImage* src = cvLoadImage("C:\\tmp\\pg3.jpg");
	CvSize size = cvGetSize(src);

	int topBoundary, bottomBoundary;
	topBoundary = findBoundaryVertical(src, 2, 1);
	bottomBoundary = findBoundaryVertical(src, size.height - 2, -1);

	CvSize newSrcSize=size;

	newSrcSize.height = bottomBoundary;
	newSrcSize.height -= topBoundary;

	CvSize imgSize;
	imgSize.width = size.width;
	imgSize.height = newSrcSize.height / 3;
	
	IplImage* bImg = cvCreateImage(imgSize, 8, 3);
	IplImage* gImg = cvCreateImage(imgSize, 8, 3);
	IplImage* rImg = cvCreateImage(imgSize, 8, 3);
	IplImage* dst = cvCreateImage(imgSize, 8, 3);

	IplImage* newSrc = cvCreateImage(newSrcSize, 8, 3);
	for (int y = topBoundary; y < bottomBoundary; y++) {
		for (int x = 0; x < newSrcSize.width; x++) {
			CvScalar color = cvGet2D(src, y, x);

			cvSet2D(newSrc, y-topBoundary, x, color);
		}
	}

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
	ImgInfo bImgInfo;
	ImgInfo gImgInfo;
	ImgInfo rImgInfo;
	bImgInfo.arr = getImgInfo(bImg);
	gImgInfo.arr = getImgInfo(gImg);
	rImgInfo.arr = getImgInfo(rImg);


	int maxDiffY_BG = 2147483647;
	int maxDiffYPos_BG=0;

	int maxDiffY_BR = 2147483647;
	int maxDiffYPos_BR=0;
	for (int y = 0; y < imgSize.height; y++) {
		int nowdiffBGY = calculateArrDiff(bImgInfo.arr[imgSize.height / 2], gImgInfo.arr[y], imgSize.width);
		if (maxDiffY_BG > nowdiffBGY) {
			maxDiffY_BG = nowdiffBGY;
			maxDiffYPos_BG = y;
			printf("%d번 행 BG 차이:%d\n", y, maxDiffY_BG);
		}

		int nowdiffBRY = calculateArrDiff(bImgInfo.arr[imgSize.height / 2], rImgInfo.arr[y], imgSize.width);
		if (maxDiffY_BR > nowdiffBRY) {
			maxDiffY_BR = nowdiffBRY;
			maxDiffYPos_BR = y;
			printf("%d번 행 BG 차이:%d\n", y, maxDiffY_BG);
		}
	}
	drawVerticalLine(bImg, BLUE, imgSize.height / 2);
	drawVerticalLine(gImg, GREEN, maxDiffYPos_BG);
	drawVerticalLine(rImg, RED, maxDiffYPos_BR);
	printf("BG차이 최대 %d행, BR차이 최대 %d행\n",maxDiffYPos_BG, maxDiffYPos_BR);



	int maxDiffX_BG = INT_MAX;
	int maxDiffXPos_BG = 0;

	int maxDiffX_BR = INT_MAX;
	int maxDiffXPos_BR = 0;
	for (int x = 0; x < imgSize.width; x++) {
		int sumBG=0;
		int sumBR=0;
		for (int y = 0; y < imgSize.height; y++) {
			sumBG += abs(bImgInfo.arr[y][imgSize.width/2]-gImgInfo.arr[y][x]);
			sumBR += abs(bImgInfo.arr[y][imgSize.width / 2]-rImgInfo.arr[y][x]);
		}
		if (maxDiffX_BG > sumBG) {
			maxDiffX_BG = sumBG;
			maxDiffXPos_BG = x;
			printf("%d번 열 BG 차이:%d\n",x,maxDiffX_BG);
		}

		if (maxDiffX_BR > sumBR) {
			maxDiffX_BR = sumBR;
			maxDiffXPos_BR = x;
			printf("%d번 열 BR 차이:%d\n", x, maxDiffX_BR);
		}
	}
	drawHorizontalLine(bImg, BLUE, imgSize.width / 2);
	drawHorizontalLine(gImg, GREEN, maxDiffXPos_BG);
	drawHorizontalLine(rImg, RED, maxDiffXPos_BR);


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

void printImgInfo(ImgInfo info)
{
	int i,j;
	printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡarrㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			printf("%4d",info.arr[i][j]);
		}
		printf("\n");
	}
	printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
}

int** getImgInfo(IplImage* img)
{
	int h=cvGetSize(img).height;
	int w=cvGetSize(img).width;
	int **arr = (int**)malloc(sizeof(int*)*h);
	int i,j;
	for (i = 0; i < h; i++) {
		arr[i] = (int*)malloc(sizeof(int)*w);
	}
	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			arr[i][j] = cvGet2D(img,i,j).val[0];
		}
	}
	return arr;
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
		arr[y]=(int)variance;
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
		arr[x]=(int)variance;
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

int calculateArrDiff(int arr1[], int arr2[], int size)
{
	int diffSum=0;
	for (int i = 0; i < size; i++) {
		diffSum += abs(arr1[i]-arr2[i]);
	}
	return diffSum;
}

int* getArr(int** imgArr, int x, int size)
{
	int *xArr = (int*)malloc(sizeof(int)*size);
	for (int y = 0; y < size; y++) {
		xArr[y] = imgArr[y][x];
	}
	return xArr;
}
