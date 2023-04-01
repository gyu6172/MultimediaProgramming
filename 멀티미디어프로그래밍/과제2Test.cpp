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
int** getImgInfo(IplImage* img);
int* getImgInfoVertical(IplImage* img, char c);
int* getImgInfoHorizontal(IplImage* img, char c);
int findBoundaryVertical(IplImage* img, int startY, int dy);
int findBoundaryHorizontal(IplImage* img, int startX, int dx);
void drawVerticalLine(IplImage* img, CvScalar color, int y);
void drawHorizontalLine(IplImage* img, CvScalar color, int x);
int calculateArrDiff(int arr1[], int arr2[], int size);
int calculateDiffFilter(int** imgInfo, int filter[][FILTERSIZE], int starty, int startx);
int* getArr(int** imgArr, int x, int size);
float getDistance(CvScalar f, CvScalar g);

int main() {

	//이미지 입력
	//printf("Input File Name : ");
	//char str[100];
	//scanf("%s",str);
	//IplImage *src = cvLoadImage(str);
	IplImage* src = cvLoadImage("C:\\tmp\\pg4.jpg");
	CvSize size = cvGetSize(src);

	CvSize imgSize;
	imgSize.width = size.width;
	imgSize.height = size.height / 3;

	int h = imgSize.height;
	int w = imgSize.width;

	IplImage* bImg = cvCreateImage(imgSize, 8, 3);
	IplImage* gImg = cvCreateImage(imgSize, 8, 3);
	IplImage* rImg = cvCreateImage(imgSize, 8, 3);
	IplImage* dst = cvCreateImage(imgSize, 8, 3);

	for (int y = 0; y < imgSize.height; y++) {
		for (int x = 0; x < imgSize.width; x++) {
			CvScalar bc = cvGet2D(src, y, x);
			CvScalar gc = cvGet2D(src, y + size.height / 3, x);
			CvScalar rc = cvGet2D(src, y + size.height / 3 * 2, x);

			cvSet2D(bImg, y, x, bc);
			cvSet2D(gImg, y, x, gc);
			cvSet2D(rImg, y, x, rc);
		}
	}

	//CvSize templSize;
	//templSize.height = h/3;
	//templSize.width = w/3;
	//IplImage *templ = cvCreateImage(templSize, 8, 3);
	//for (int y = 0; y < templSize.height; y++) {
	//	for (int x = 0; x < templSize.width; x++) {
	//		cvSet2D(templ, y, x, cvGet2D(bImg, y+templSize.height, x+templSize.width));
	//	}
	//}
	//IplImage *resultG = cvCreateImage(cvSize(w-templSize.width+1, h-templSize.height+1), IPL_DEPTH_32F, 1);
	//IplImage *resultR = cvCreateImage(cvSize(w-templSize.width+1, h-templSize.height+1), IPL_DEPTH_32F, 1);
	//cvMatchTemplate(gImg, templ, resultG ,CV_TM_CCOEFF_NORMED);
	//cvMatchTemplate(rImg, templ, resultR ,CV_TM_CCOEFF_NORMED);

	//double ming,maxg, minr, maxr;
	//CvPoint pb, pg, pr;
	//cvMinMaxLoc(resultG, &ming, &maxg, NULL, &pg);
	//cvMinMaxLoc(resultR, &ming, &maxg, NULL, &pr);
	//cvRectangle(gImg, pg, cvPoint(pg.x + templSize.width, pg.y + templSize.height), CV_RGB(0, 255, 0), 2);
	//cvRectangle(rImg, pr, cvPoint(pr.x + templSize.width, pr.y + templSize.height), CV_RGB(255, 0, 0), 2);
	//pb.x = templSize.width;
	//pb.y = templSize.height;

	//printf("B : (%d, %d)\n", pb.x, pb.y);
	//printf("G : (%d, %d)\n", pg.x, pg.y);
	//printf("R : (%d, %d)\n", pr.x, pr.y);

	//cvShowImage("templ",templ);





	//float bsum=0.0, gsum=0.0, rsum=0.0;
	//for (int y = 0; y < h; y++) {
	//	for (int x = 0; x < w; x++) {
	//		bsum += bImgArr[y][x];
	//		gsum += gImgArr[y][x];
	//		rsum += rImgArr[y][x];
	//	}
	//}
	//bsum /= w*h;
	//gsum /= w*h;
	//rsum /= w*h;
	//for (int y = 0; y < h; y++) {
	//	for (int x = 0; x < w; x++) {
	//		if (bImgArr[y][x] < bsum) {
	//			bImgArr[y][x] = 0;
	//		}
	//		if (gImgArr[y][x] < gsum) {
	//			gImgArr[y][x] = 0;
	//		}
	//		if (rImgArr[y][x] < rsum) {
	//			rImgArr[y][x] = 0;
	//		}
	//	}
	//}
	//for (int y = 0; y < h; y++) {
	//	for (int x = 0; x < w; x++) {
	//		int b=bImgArr[y][x];
	//		int g=gImgArr[y][x];
	//		int r=rImgArr[y][x];
	//		cvSet2D(newB, y, x, cvScalar(b,b,b));
	//		cvSet2D(newG, y, x, cvScalar(g,g,g));
	//		cvSet2D(newR, y, x, cvScalar(r,r,r));
	//	}
	//}

	////newB의 표준편차 
	//float bSum = 0.0;
	//for (int x = 0; x < w; x++) {
	//	bSum += bImgArr[h / 2][x];
	//}
	//float bAvg = bSum / w;
	//float bVar = 0.0;
	//for (int x = 0; x < w; x++) {
	//	bVar += pow(bImgArr[h / 2][x] - bAvg, 2);
	//}
	//bVar = sqrt(bVar);

	//float minVarbg = FLT_MAX, minVarbr = FLT_MAX;
	//int minbgy, minbry;
	//for (int y = 0; y < h; y++) {
	//	float gSum = 0.0;
	//	float rSum = 0.0;
	//	for (int x = 0; x < w; x++) {
	//		gSum += gImgArr[y][x];
	//		rSum += rImgArr[y][x];
	//	}
	//	float gAvg = gSum / w;
	//	float rAvg = rSum / w;
	//	float gVar = 0.0;
	//	float rVar = 0.0;
	//	for (int x = 0; x < w; x++) {
	//		gVar += pow(gImgArr[y][x] - gAvg, 2);
	//		rVar += pow(rImgArr[y][x] - rAvg, 2);
	//	}
	//	gVar = sqrt(gVar);
	//	rVar = sqrt(rVar);
	//	if (abs(bVar - gVar) < minVarbg) {
	//		minVarbg = abs(bVar - gVar);
	//		minbgy = y;
	//		printf("%d행 %f차이\n", minbgy, minVarbg);
	//	}
	//	if (abs(bVar - rVar) < minVarbr) {
	//		minVarbr = abs(bVar - rVar);
	//		minbry = y;
	//		printf("%d행 %f차이\n", minbry, minVarbr);
	//	}
	//}


	//2차원배열생성
	float** bImgArr = (float**)malloc(sizeof(float*) * h);
	float** gImgArr = (float**)malloc(sizeof(float*) * h);
	float** rImgArr = (float**)malloc(sizeof(float*) * h);
	for (int i = 0; i < h; i++) {
		bImgArr[i] = (float*)malloc(sizeof(float) * w);
		gImgArr[i] = (float*)malloc(sizeof(float) * w);
		rImgArr[i] = (float*)malloc(sizeof(float) * w);
	}
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			bImgArr[i][j] = (cvGet2D(bImg, i, j).val[0]);
			gImgArr[i][j] = (cvGet2D(gImg, i, j).val[0]);
			rImgArr[i][j] = (cvGet2D(rImg, i, j).val[0]);
		}
	}

	float mdbg=FLT_MAX, mdbr=FLT_MAX;
	int bgy, bry;
	for (int y = 0; y < h; y++) {
		float dbg=0.0, dbr=0.0;
		for (int x = 0; x < w; x++) {
			dbg += abs(bImgArr[h/2][x]-gImgArr[y][x]);
			dbr += abs(bImgArr[h/2][x]-rImgArr[y][x]);
		}
		if (mdbg > dbg) {
			mdbg = dbg;
			bgy = y;
		}
		if (mdbr > dbr) {
			mdbr = dbr;
			bry = y;
		}
	}
	drawVerticalLine(bImg, BLUE, h/2);
	drawVerticalLine(gImg, GREEN, bgy);
	drawVerticalLine(rImg, RED, bry);

	//float *bDiffArr = (float*)malloc(sizeof(float) * w);
	//float *gDiffArr = (float*)malloc(sizeof(float) * w);
	//float *rDiffArr = (float*)malloc(sizeof(float) * w);
	//int C = 400;
	//for (int x = 0; x < w-1; x++) {
	//	bDiffArr[x] = bImgArr[C][x]-bImgArr[C][x+1];
	//	printf("%.2f ",bDiffArr[x]);
	//}
	//printf("\n");
	//float minDiffbg=FLT_MAX;
	//float minDiffbr=FLT_MAX;
	//int bgy=0;
	//int bry=0;

	//for (int y = 0; y < h; y++) {
	//	for (int x = 0; x < w - 1; x++) {
	//		gDiffArr[x] = gImgArr[y][x] - gImgArr[y][x + 1];
	//		rDiffArr[x] = rImgArr[y][x] - rImgArr[y][x + 1];
	//	}
	//	int cnt=0;
	//	float diffBG=0.0;
	//	float diffBR=0.0;
	//	for (int x = 0; x < w - 1; x++) {
	//		diffBG += pow((bDiffArr[x]-gDiffArr[x]),2);
	//		diffBR += pow((bDiffArr[x]-rDiffArr[x]),2);
	//		cnt++;
	//	}
	//	if (diffBG/cnt < minDiffbg) {
	//		minDiffbg = diffBG/cnt;
	//		bgy = y;
	//		printf("BG:%d행 %.2f차이\n" ,bgy ,minDiffbg);
	//	}
	//	if (diffBR/cnt < minDiffbr) {
	//		minDiffbr = diffBR/cnt;
	//		bry = y;
	//		printf("BR:%d행 %.2f차이\n", bry, minDiffbr);
	//	}
	//}
	//drawVerticalLine(bImg, BLUE, C);
	//drawVerticalLine(gImg, GREEN, bgy);
	//drawVerticalLine(rImg, RED, bry);

	//float minDiffBG = FLT_MAX;
	//float minDiffBR = FLT_MAX;
	//int uBG, uBR, vBG, vBR;

	//int dh = imgSize.height / 3;
	//int dw = imgSize.width / 3;

	//for (int v = -10; v <= 10; v++) {
	//	for (int u = -10; u <= 10; u++) {
	//		float sumBG = 0.0;
	//		float sumBR = 0.0;
	//		int cnt = 0;
	//		for (int y = h / 4; y < h / 4 * 3; y++) {
	//			for (int x = w / 4; x < w / 4 * 3; x++) {
	//				if (x + u<0 || x + u>imgSize.width - 1) continue;
	//				if (y + v<0 || y + v>imgSize.height - 1) continue;
	//				sumBG += abs(bImgArr[y][x] - gImgArr[y + v][x + u]);
	//				sumBR += abs(bImgArr[y][x] - rImgArr[y + v][x + u]);
	//				cnt++;
	//			}
	//		}
	//		if (sumBG / cnt < minDiffBG) {
	//			minDiffBG = sumBG / cnt;
	//			uBG = u;
	//			vBG = v;
	//			printf("u=%d, v=%d, avg_error = %f\n", u, v, minDiffBG);
	//		}
	//		if (sumBR / cnt < minDiffBR) {
	//			minDiffBR = sumBR / cnt;
	//			uBR = u;
	//			vBR = v;
	//			printf("u=%d, v=%d, avg_error = %f\n", u, v, minDiffBR);
	//		}
	//	}
	//}

	////dst
	//for (int y = 0; y < imgSize.height; y++) {
	//	for (int x = 0; x < imgSize.width; x++) {
	//		if (x + uBG<0 || x + uBG>imgSize.width - 1) continue;
	//		if (y + vBG<0 || y + vBG>imgSize.height - 1) continue;
	//		if (x + uBR<0 || x + uBR>imgSize.width - 1) continue;
	//		if (y + vBR<0 || y + vBR>imgSize.height - 1) continue;
	//		CvScalar b = cvGet2D(bImg, y, x);
	//		CvScalar g = cvGet2D(gImg, y + vBG, x + uBG);
	//		CvScalar r = cvGet2D(rImg, y + vBR, x + uBR);
	//		CvScalar color;
	//		color.val[0] = b.val[0];
	//		color.val[1] = g.val[0];
	//		color.val[2] = r.val[0];
	//		cvSet2D(dst, y, x, color);
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

void drawImgGraphVertical(int arr[], CvSize size, const char* str)
{
	IplImage* graph = cvCreateImage(size, 8, 3);
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
		for (int y = 0; y < arr[x]; y++) {
			cvSet2D(graph, y, x, BLACK);
		}
	}
	cvShowImage(str, graph);

}

void printImgInfo(ImgInfo info)
{
	int i, j;
	printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡarrㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			printf("%4d", info.arr[i][j]);
		}
		printf("\n");
	}
	printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
}

int** getImgInfo(IplImage* img)
{
	int h = cvGetSize(img).height;
	int w = cvGetSize(img).width;
	int** arr = (int**)malloc(sizeof(int*) * h);
	int i, j;
	for (i = 0; i < h; i++) {
		arr[i] = (int*)malloc(sizeof(int) * w);
	}
	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			arr[i][j] = cvGet2D(img, i, j).val[0];
		}
	}
	return arr;
}

int* getImgInfoVertical(IplImage* img, char c)
{
	int* arr = (int*)malloc(sizeof(int) * SIZE);
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
		arr[y] = (int)variance;
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
		arr[x] = (int)variance;
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
	int diffSum = 0;
	for (int i = 0; i < size; i++) {
		diffSum += abs(arr1[i] - arr2[i]);
	}
	return diffSum;
}

int calculateDiffFilter(int** imgInfo, int filter[][FILTERSIZE], int starty, int startx)
{
	int sum = 0;
	for (int y = 0; y < FILTERSIZE; y++) {
		for (int x = 0; x < FILTERSIZE; x++) {
			sum += abs(imgInfo[starty + y][startx + x] - filter[y][x]);
		}
	}
	return sum;
}

int* getArr(int** imgArr, int x, int size)
{
	int* xArr = (int*)malloc(sizeof(int) * size);
	for (int y = 0; y < size; y++) {
		xArr[y] = imgArr[y][x];
	}
	return xArr;
}

float getDistance(CvScalar a, CvScalar b) {
	float sum = 0.0;
	sum += abs(a.val[0] - b.val[0]);
	return sum;
}
