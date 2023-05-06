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
void shuffleArr(Circle *circleArr, int size) {
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

	int w = imgSize.width;
	int h = imgSize.height;

	//원의 반지름
	int r=32;

	Grid jitteredGrid;
	jitteredGrid.width = r;
	jitteredGrid.height = r;
	jitteredGrid.colsCnt = (imgSize.width) / (jitteredGrid.width)+1;
	jitteredGrid.rowsCnt = (imgSize.height) / (jitteredGrid.height)+1;

	//cvShowImage("ref", refImg);

	while (r > 1) {
		Circle *circleArr = (Circle*)malloc(sizeof(Circle)*(jitteredGrid.colsCnt*jitteredGrid.rowsCnt));
		int circleCnt = 0;

		cvSmooth(srcImg, refImg, CV_GAUSSIAN, r-1);

		for (int y = 0; y < h; y += jitteredGrid.height) {
			for (int x = 0; x < w; x += jitteredGrid.width) {
				float max_diff = 0;
				CvPoint max_diff_pos = { 0,0 };
				CvScalar max_diff_color = cvScalar(255, 255, 255);

				for (int v = y; v < y + jitteredGrid.height; v++) {
					for (int u = x; u < x + jitteredGrid.width; u++) {
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

				Circle c;
				c.radius = r;
				c.color = max_diff_color;
				c.point = max_diff_pos;
				circleArr[circleCnt++] = c;
			}
		}


		printf("circleCnt : %d\n", circleCnt);
		shuffleArr(circleArr, circleCnt);
		for (int i = 0; i < circleCnt; i++) {
			drawCircle(canvas, circleArr[i]);
		}
		free(circleArr);


		r/=2;
		jitteredGrid.width /= 2;
		jitteredGrid.height /= 2;
		jitteredGrid.colsCnt = (imgSize.width) / (jitteredGrid.width) + 1;
		jitteredGrid.rowsCnt = (imgSize.height) / (jitteredGrid.height) + 1;
		cvShowImage("canvas", canvas);
		cvWaitKey();
	}

	cvWaitKey();
	
}