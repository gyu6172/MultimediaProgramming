#include <opencv2/opencv.hpp>
//홀수여야함. 짝수면 에러뜰수도
#define FILTERSIZE 10
#define HORIZONTALVARIANCELIMIT 250
#define VERTICALVARIANCELIMIT 350
#define VERTICALAVGLIMIT 50
#define SIZE 1000

CvScalar BLUE = cvScalar(255, 0, 0);
CvScalar GREEN = cvScalar(0, 255, 0);
CvScalar RED = cvScalar(0, 0, 255);
CvScalar WHITE = cvScalar(255, 255, 255);
CvScalar BLACK = cvScalar(0, 0, 0);

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
int calculateDiffFilter(int **imgInfo, int filter[][FILTERSIZE], int starty, int startx);
int* getArr(int **imgArr, int x, int size);
float getDistance(CvScalar f, CvScalar g);

int main() {
	//이미지 입력
	//printf("Input File Name : ");
	//char str[100];
	//scanf("%s",str);
	//IplImage *src = cvLoadImage(str);
	IplImage* src = cvLoadImage("C:\\tmp\\pg4.jpg");
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

	int filter[FILTERSIZE][FILTERSIZE];
	int middlex=imgSize.width/2;
	int middley=imgSize.height/2;

	for (int i = 0; i < FILTERSIZE; i++) {
		for (int j = 0; j < FILTERSIZE; j++) {
			filter[i][j] = bImgInfo.arr[middley+i][middlex+j];
		}
	}

	int d=10;
	float minDiffBG=FLT_MAX;
	float minDiffBR=FLT_MAX;
	int uBG, uBR, vBG, vBR;

	printf("middleX:%d\nmiddleY:%d\n",middlex,middley);
	for (int v = -d; v <= d; v++) {
		for (int u = -d; u <= d; u++) {
			float sumBG=0.0;
			float sumBR=0.0;
			int cnt=0;
			for (int y = imgSize.height/4; y < imgSize.height/4*3; y++) {
				for (int x = imgSize.width/4; x < imgSize.width/4*3; x++) {
					if (x + u<0 || x + u>imgSize.width - 1) continue;
					if (y + v<0 || y + v>imgSize.height - 1) continue;
					sumBG += getDistance(cvGet2D(bImg, y, x), cvGet2D(gImg, y+v, x+u));
					sumBR += getDistance(cvGet2D(bImg, y, x), cvGet2D(rImg, y+v, x+u));
					cnt++;
				}
			}
			float avgBG = sumBG / cnt;
			float avgBR = sumBR / cnt;
			if (avgBG < minDiffBG) {
				minDiffBG = avgBG;
				uBG = u;
				vBG = v;
				printf("u=%d, v=%d, avg_error = %f\n", u, v, minDiffBG);
			}
			if (avgBR < minDiffBR) {
				minDiffBR = avgBR;
				uBR = u;
				vBR = v;
				printf("u=%d, v=%d, avg_error = %f\n", u, v, minDiffBR);
			}
		}
	}

	//int bgfiltering = calculateDiffFilter(gImgInfo.arr, filter, middley + v, middlex + u);
	//if (minDiffBG > bgfiltering) {
	//	minDiffBG = bgfiltering;
	//	uBG = u;
	//	vBG = v;
	//	printf("BG %d차이, u:%d, v:%d\n", minDiffBG, u, v);
	//}
	//int brfiltering = calculateDiffFilter(gImgInfo.arr, filter, middley + v, middlex + u);
	//if (minDiffBR > brfiltering) {
	//	minDiffBR = brfiltering;
	//	uBR = u;
	//	vBR = v;
	//	printf("BR %d차이, u:%d, v:%d\n", minDiffBR, u, v);
	//}

	drawVerticalLine(bImg, BLUE, middley);
	drawVerticalLine(gImg, GREEN, middley+vBG);
	drawVerticalLine(rImg, RED, middley+vBR);

	drawHorizontalLine(bImg, BLUE, middlex);
	drawHorizontalLine(gImg, GREEN, middlex+uBG);
	drawHorizontalLine(rImg, RED, middlex+uBR);

	//dst
	for (int y = 0; y < imgSize.height; y++) {
		for (int x = 0; x < imgSize.width; x++) {
			if (x + uBG<0 || x + uBG>imgSize.width - 1) continue;
			if (y + vBG<0 || y + vBG>imgSize.height - 1) continue;
			if (x + uBR<0 || x + uBR>imgSize.width - 1) continue;
			if (y + vBR<0 || y + vBR>imgSize.height - 1) continue;
			CvScalar b = cvGet2D(bImg, y, x);
			CvScalar g = cvGet2D(gImg, y+vBG, x+uBG);
			CvScalar r = cvGet2D(rImg, y+vBR, x+uBR);
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

int calculateDiffFilter(int** imgInfo, int filter[][FILTERSIZE], int starty, int startx)
{
	int sum=0;
	for (int y = 0; y < FILTERSIZE; y++) {
		for (int x = 0; x < FILTERSIZE; x++) {
			sum += abs(imgInfo[starty+y][startx+x]-filter[y][x]);
		}
	}
	return sum;
}

int* getArr(int** imgArr, int x, int size)
{
	int *xArr = (int*)malloc(sizeof(int)*size);
	for (int y = 0; y < size; y++) {
		xArr[y] = imgArr[y][x];
	}
	return xArr;
}

float getDistance(CvScalar a, CvScalar b) {
	float sum = 0.0;
	for (int k = 0; k < 3; k++) {
		sum += pow(a.val[k] - b.val[k], 2);
	}
	return sum;
}
