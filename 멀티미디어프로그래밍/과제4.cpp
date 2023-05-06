#include <opencv2/opencv.hpp>
typedef struct Stroke {
	int radius;
	CvScalar color;
	CvPoint startPoint;
	CvPoint route[];
}Stroke;
typedef struct Circle {
	int radius;
	CvScalar color;
	CvPoint point;
}Circle;
typedef struct Grid{
	int colsCnt;	//격자판의 가로 개수
	int rowsCnt;	//격자판의 세로 개수
	int width;		//격자 하나의 가로 길이(너비)
	int height;		//격자 하나의 세로 길이(높이)
};

void drawCircle(IplImage* img, Circle circle) {
	cvCircle(img, circle.point, circle.radius, circle.color, -1);
}
float getDifference(CvScalar f, CvScalar g) {
	float sum = 0.0;
	for (int k = 0; k < 3; k++) {
		sum += (f.val[k]-g.val[k])*(f.val[k]-g.val[k]);
	}
	return sum;
}
void shuffleArr(Circle circleArr[], int size) {
	for (int i = 0; i < size; i++) {
		int ran = rand()%size;
		Circle tmp = circleArr[i];
		circleArr[i] = circleArr[ran];
		circleArr[ran] = tmp;
	}
}

int main() {
	IplImage* srcImg = cvLoadImage("C:\\tmp\\lena.png");
	CvSize imgSize = cvGetSize(srcImg);
	IplImage* refImg = cvCreateImage(imgSize, 8, 3);
	IplImage* canvas = cvCreateImage(imgSize, 8, 3);

	cvSet(canvas, cvScalar(255,255,255));
	cvSmooth(srcImg, refImg, CV_GAUSSIAN, 11);

	int w = imgSize.width;
	int h = imgSize.height;

	Grid jitterdGrid;
	jitterdGrid.width = 40;
	jitterdGrid.height = 40;
	jitterdGrid.colsCnt = (imgSize.width) / (jitterdGrid.width);
	jitterdGrid.rowsCnt = (imgSize.height) / (jitterdGrid.height);

	cvShowImage("ref", refImg);

	int r=32;
	while (r > 2) {
		//Circle *circleArr = (Circle*)malloc(sizeof(Circle)*(jitterdGrid.colsCnt*jitterdGrid.rowsCnt));
		Circle circleArr[20000];
		int circleCnt = 0;

		for (int y = 0; y < h; y += jitterdGrid.height) {
			for (int x = 0; x < w; x += jitterdGrid.width) {
				float max_diff = 0;
				CvPoint max_diff_pos = { 0,0 };
				CvScalar max_diff_color = cvScalar(255, 255, 255);

				for (int v = y; v < y + jitterdGrid.height; v++) {
					for (int u = x; u < x + jitterdGrid.width; u++) {
						if (u > w - 1 || v > h - 1)	continue;

						CvScalar refColor = cvGet2D(refImg, v, u);
						CvScalar canvasColor = cvGet2D(canvas, v, u);

						float diff = getDifference(refColor, canvasColor);
						if (diff > max_diff) {
							max_diff = diff;
							max_diff_pos.x = u;
							max_diff_pos.y = v;
							max_diff_color = refColor;
						}

					}
				}

				Circle c = { r, max_diff_color, max_diff_pos };
				circleArr[circleCnt++] = c;
			}
		}

		printf("circleCnt : %d\n", circleCnt);
		shuffleArr(circleArr, circleCnt);
		for (int i = 0; i < circleCnt; i++) {
			drawCircle(canvas, circleArr[i]);
		}

		r/=2;
		jitterdGrid.width /= 2;
		jitterdGrid.height /= 2;
		jitterdGrid.colsCnt = (imgSize.width) / (jitterdGrid.width);
		jitterdGrid.rowsCnt = (imgSize.height) / (jitterdGrid.height);
		//free(circleArr);
		cvShowImage("canvas", canvas);
		cvWaitKey(1000);
	}

	cvWaitKey();
	
}