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
	jitterdGrid.width = 64;
	jitterdGrid.height = 64;
	jitterdGrid.colsCnt = (imgSize.width) / (jitterdGrid.width);
	jitterdGrid.rowsCnt = (imgSize.height) / (jitterdGrid.height);

	cvShowImage("ref",refImg);
	cvShowImage("canvas",canvas);

	for (int y = 0; y < h; y+=jitterdGrid.height) {
		for (int x = 0; x < w; x+=jitterdGrid.width) {
			
			float max_diff = 0;
			CvPoint max_diff_pos = {0,0};
			CvScalar max_diff_color = cvScalar(255,255,255);

			for (int v = y; v < y + jitterdGrid.height; v++) {
				for (int u = x; u < x + jitterdGrid.width; x++) {
					if(u>w-1 || v>h-1)	continue;

					CvScalar refColor = cvGet2D(refImg, v, u);
					CvScalar canvasColor = cvGet2D(canvas, v, u);

					float diff = getDifference(refColor, canvasColor);
					if (diff > max_diff) {
						max_diff = diff;
						max_diff_pos.x = u;
						max_diff_pos.y = v;
						max_diff_color = refColor;
						printf("diff:%.2f, x:%d, y:%d\n",max_diff, u, v);
					}
				}
			}

			Circle c = {64, max_diff_color, max_diff_pos};
			drawCircle(canvas, c);
		}
	}

	cvShowImage("ref", refImg);
	cvShowImage("canvas", canvas);
	cvWaitKey();
	
}