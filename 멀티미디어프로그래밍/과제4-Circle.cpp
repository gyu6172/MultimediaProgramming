#include <opencv2/opencv.hpp>
#define R 32
#define L 32
#define T 2700
typedef struct Circle {
	int radius;
	CvScalar color;
	CvPoint point;
}Circle;
typedef struct Grid{
	int colsCnt;	//�������� ���� ����
	int rowsCnt;	//�������� ���� ����
	int width;		//���� �ϳ��� ���� ����(�ʺ�)
	int height;		//���� �ϳ��� ���� ����(����)
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

	//���� ������
	int r=R;

	Grid jitteredGrid;
	jitteredGrid.width = L;
	jitteredGrid.height = L;
	jitteredGrid.colsCnt = (imgSize.width) / (jitteredGrid.width)+1;
	jitteredGrid.rowsCnt = (imgSize.height) / (jitteredGrid.height)+1;

	//cvShowImage("ref", refImg);

	while (r >= 2) {
		Circle *circleArr = (Circle*)malloc(sizeof(Circle)*(jitteredGrid.colsCnt*jitteredGrid.rowsCnt));
		int circleCnt = 0;

		cvSmooth(srcImg, refImg, CV_GAUSSIAN, r-1);

		for (int y = 0; y < h; y += jitteredGrid.height) {
			for (int x = 0; x < w; x += jitteredGrid.width) {
				float max_diff = 0;
				float diff_avg = 0.0;
				CvPoint max_diff_pos = { 0,0 };
				CvScalar max_diff_color = cvScalar(255, 255, 255);

				for (int v = y; v < y + jitteredGrid.height; v++) {
					for (int u = x; u < x + jitteredGrid.width; u++) {
						if (u > w - 1 || v > h - 1)	continue;
						if(u-1<0||u+1>w-1) continue;
						if(v-1<0||v+1>h-1) continue;

						CvScalar refColor = cvGet2D(refImg, v,u);

						CvScalar canvasColor = cvGet2D(canvas,v,u);

						float diff = getDifference(refColor, canvasColor);
						if (diff > max_diff) {
							max_diff = diff;
							max_diff_pos.x = u;
							max_diff_pos.y = v;
							max_diff_color = refColor;
						}
						diff_avg += diff;
					}
				}

				diff_avg = diff_avg / (jitteredGrid.width * jitteredGrid.height);
				//printf("(%d, %d) err=%.2f\n", x,y,diff_avg);
				if (diff_avg > T) {
					Circle c;
					c.radius = r;
					c.color = max_diff_color;
					c.point = max_diff_pos;
					circleArr[circleCnt++] = c;
				}
			}
		}

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