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
#define VERTICALVARIANCELIMIT 160

int main() {
	//이미지 입력
	//printf("Input File Name : ");
	//char str[100];
	//scanf("%s",str);
	//IplImage *src = cvLoadImage(str);

	IplImage* src = cvLoadImage("C:\\tmp\\pg2.jpg");
	CvSize size = cvGetSize(src);
	CvSize imgSize = size;

	//for (int y = 0; y < size.height; y++){
	//	double avg=0.0;
	//	for (int x = 0; x < size.width; x++) {
	//		avg += cvGet2D(src,y,x).val[0];
	//	}
	//	avg /= size.width-1;
	//	double variance = 0.0;
	//	for (int x = 0; x < size.width; x++) {
	//		variance += abs(avg-cvGet2D(src,y,x).val[0]);
	//	}
	//	printf("%d번 : %f\n",y,variance);
	//}
	//for (int x = 0; x < size.width; x++) {
	//	cvSet2D(src,29,x,cvScalar(0,0,255));
	//	cvSet2D(src, 50, x, cvScalar(0, 0, 255));
	//	cvSet2D(src, 52, x, cvScalar(0, 0, 255));
	//}


	imgSize.height = size.height / 3;

	IplImage* bImg = cvCreateImage(imgSize, 8, 3);
	IplImage* gImg = cvCreateImage(imgSize, 8, 3);
	IplImage* rImg = cvCreateImage(imgSize, 8, 3);
	IplImage* dst = cvCreateImage(imgSize, 8, 3);

	//int bHeight=findBoundaryVertical(src, cvGetSize(src).height * 1 / 6, 1);
	//int gHeight = findBoundaryVertical(src, cvGetSize(src).height * 3 / 6, 1);
	//int rHeight = findBoundaryVertical(src, cvGetSize(src).height * 5 / 6, 1);

	//imgSize.width = MAX(bHeight, abs(bHeight-gHeight));
	//imgSize.width = MAX(imgSize.width, abs(gHeight-rHeight));

	//원본 이미지 3등분하기
	//Blue
	for (int y = 0; y < imgSize.height; y++) {
		for (int x = 0; x < imgSize.width; x++) {
			CvScalar color = cvGet2D(src, y, x);
			cvSet2D(bImg, y, x, color);
		}
	}
	//Green
	for (int y = 0; y < imgSize.height; y++) {
		for (int x = 0; x < imgSize.width; x++) {
			CvScalar color = cvGet2D(src, y + size.height / 3, x);
			cvSet2D(gImg, y, x, color);
		}
	}
	//Red
	for (int y = 0; y < imgSize.height; y++) {
		for (int x = 0; x < imgSize.width; x++) {
			CvScalar color = cvGet2D(src, y + size.height / 3 * 2, x);
			cvSet2D(rImg, y, x, color);
		}
	}




	int blimit, glimit, rlimit;
	////이미지 조정하기
	////Blue
	//printImgInfoVertical(bImg, 'B');
	////Green
	//printImgInfoVertical(gImg, 'G');
	////Red
	//printImgInfoVertical(rImg, 'R');

	////Blue
	//printImgInfoHorizontal(bImg, 'B');
	////Green
	//printImgInfoHorizontal(gImg, 'G');
	////Red
	//printImgInfoHorizontal(rImg, 'R');

	//blimit=29;
	//glimit=29;
	//rlimit=29;
	////선 긋기
	//for (int x = 0; x < size.width; x++) {
	//	cvSet2D(bImg, blimit, x, cvScalar(255, 0, 0));
	//	cvSet2D(gImg, glimit, x, cvScalar(0, 255, 0));
	//	cvSet2D(rImg, rlimit, x, cvScalar(0, 0, 255));
	//}

	//for (int y = 0; y < imgSize.height; y++) {
	//	cvSet2D(bImg, y, 21, cvScalar(255, 0, 0));
	//	cvSet2D(bImg, y, 44, cvScalar(255, 0, 0));
	//	
	//	cvSet2D(gImg, y, 19, cvScalar(0, 255, 0));
	//	cvSet2D(gImg, y, 47, cvScalar(0, 255, 0));

	//	cvSet2D(rImg, y, 16, cvScalar(0, 0, 255));
	//	cvSet2D(rImg, y, 48, cvScalar(0, 0, 255));
	//}
	printImgInfoVertical(src, 'R');
	printImgInfoHorizontal(src, 'R');

	printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ원본 세로경계ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
	findBoundaryVertical(src, cvGetSize(src).height*1/3-100, 1);
	findBoundaryVertical(src, cvGetSize(src).height*2/3-100, 1);
	findBoundaryVertical(src, cvGetSize(src).height-100, 1);

	//printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ원본 가로경계ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
	//findBoundaryHorizontal(src, cvGetSize(src).width/2, -1);

	printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡB 세로경계ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
	findBoundaryVertical(bImg, cvGetSize(bImg).height/2, 1);
	printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡG 세로경계ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
	findBoundaryVertical(gImg, cvGetSize(gImg).height/2, 1);
	printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡR 세로경계ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
	findBoundaryVertical(rImg, cvGetSize(rImg).height/2, 1);

	printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡB 가로경계ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
	findBoundaryHorizontal(bImg, 100, -1);
	printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡG 가로경계ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
	findBoundaryHorizontal(gImg, 100, -1);
	printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡR 가로경계ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
	findBoundaryHorizontal(rImg, 100, -1);


	//테스트용
	//IplImage *test = cvCreateImage(imgSize, 8, 3);
	//cvAbsDiff(bImg,gImg,test);
	//cvAbsDiff(test,rImg,test);
	//printf("%f",calculateImg(bImg,rImg));

	//cvShowImage("test",test);

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
	//cvShowImage("dst", dst);
	cvShowImage("src", src);


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
	for (int y = startY;y<cvGetSize(img).height-1; y+=dy) {
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
			drawVerticalLine(img, BLUE, maxDiffY);
			return maxDiffY;
		}
	}
	return maxDiffY;
}

int findBoundaryHorizontal(IplImage* img, int startX, int dx)
{
	int maxdiff = 0;
	int maxDiffX = 0;
	for (int x = startX;; x+=dx) {
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
			drawHorizontalLine(img, RED, maxDiffX);
			return maxDiffX;
		}
	}
	return maxDiffX;
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
