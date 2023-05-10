#include <opencv2/opencv.hpp>
#define R 32
#define L 32
#define T 2700
typedef struct Stroke {
	int radius;
	CvScalar color;
	CvPoint start_point;
	CvPoint *route;
	int points_cnt;
};
typedef struct Grid {
	int colsCnt;	//격자판의 가로 개수
	int rowsCnt;	//격자판의 세로 개수
	int width;		//격자 하나의 가로 길이(너비)
	int height;		//격자 하나의 세로 길이(높이)
};

void drawSplineStroke(IplImage* img, Stroke stroke) {
	CvPoint st = stroke.start_point;
	CvPoint ed;
	for (int i = 0; i < stroke.points_cnt; i++) {
		ed = stroke.route[i];
		cvLine(img, st, ed, stroke.color, stroke.radius);
		printf("(%d, %d)에서 (%d, %d)로 그림\n",st.x,st.y, ed.x,ed.y);
		st = ed;
	}
}
float getDifference(CvScalar f, CvScalar g) {
	float sum = 0.0;
	for (int k = 0; k < 3; k++) {
		sum += (f.val[k] - g.val[k]) * (f.val[k] - g.val[k]);
	}
	return sum;
}
void shuffleArr(Stroke* stroke_arr, int size) {
	for (int i = 0; i < size; i++) {
		int ran = rand() % size;
		Stroke tmp = stroke_arr[i];
		stroke_arr[i] = stroke_arr[ran];
		stroke_arr[ran] = tmp;
	}
}
float mag(CvPoint p) {
	return sqrt(p.x * p.x + p.y * p.y);
}

int main() {
	IplImage* srcImg = cvLoadImage("C:\\tmp\\lena.png");
	CvSize imgSize = cvGetSize(srcImg);
	IplImage* refImg = cvCreateImage(imgSize, 8, 3);
	IplImage* canvas = cvCreateImage(imgSize, 8, 3);

	cvSet(canvas, cvScalar(255, 255, 255));

	int w = imgSize.width;
	int h = imgSize.height;

	//선의 반지름
	int r = R;

	Grid jitteredGrid;
	jitteredGrid.width = L;
	jitteredGrid.height = L;
	jitteredGrid.colsCnt = (imgSize.width) / (jitteredGrid.width) + 1;
	jitteredGrid.rowsCnt = (imgSize.height) / (jitteredGrid.height) + 1;

	//Stroke s;
	//s.color = cvScalar(0,0,0);	
	//s.radius = 2;
	//s.points_cnt = 3;
	//s.start_point = cvPoint(10,10);
	//s.route = (CvPoint*)malloc(sizeof(CvPoint)*s.points_cnt);
	//(s.route)[0] = cvPoint(400,10);
	//(s.route)[1] = cvPoint(40,60);
	//(s.route)[2] = cvPoint(100,200);
	//drawSplineStroke(canvas, s);
	//cvShowImage("c",canvas);
	//cvWaitKey();

	while (r >= 1) {
		Stroke* stroke_arr = (Stroke*)malloc(sizeof(Stroke) * (jitteredGrid.colsCnt * jitteredGrid.rowsCnt));
		int stroke_cnt = 0;

		cvSmooth(srcImg, refImg, CV_GAUSSIAN, r - 1);

		for (int y = 0; y < h; y += jitteredGrid.height) {
			for (int x = 0; x < w; x += jitteredGrid.width) {
				float max_diff = 0;
				float diff_avg = 0.0;
				CvPoint max_diff_pos = { 0,0 };
				CvScalar max_diff_color = cvScalar(255, 255, 255);

				for (int v = y; v < y + jitteredGrid.height; v++) {
					for (int u = x; u < x + jitteredGrid.width; u++) {
						if (u > w - 1 || v > h - 1)	continue;
						if (u < 0 || v < 0) continue;

						CvScalar refColor = cvGet2D(refImg, v, u);

						CvScalar canvasColor = cvGet2D(canvas, v, u);

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

					int max_stroke_length = 10;
					int is_out = 0;

					Stroke s;
					s.route = (CvPoint*)malloc(sizeof(CvPoint)*max_stroke_length);
					s.radius = r;
					s.color = max_diff_color;
					s.start_point = max_diff_pos;
					s.points_cnt=0;

					CvPoint current_point = s.start_point;
					float dx=0, dy=0, last_dx=0, last_dy=0;
					while (true) {
						

						CvScalar top, left, mid, right, bot;
						top = cvGet2D(refImg, current_point.y - 1, current_point.x);
						left = cvGet2D(refImg, current_point.y, current_point.x - 1);
						//mid = cvGet2D(refImg, current_point.y, current_point.x);
						right = cvGet2D(refImg, current_point.y, current_point.x + 1);
						bot = cvGet2D(refImg, current_point.y + 1, current_point.x);

						float gx = 0, gy = 0;
						for (int k = 0; k < 3; k++) {
							gx += right.val[k] - left.val[k];
							gy += bot.val[k] - top.val[k];
						}

						if(gx==0 && gy==0) break;

						dx = gy;
						dy = -gx;

						//d : 단위벡터?
						dx = dx / sqrt(dx*dx + dy*dy);
						dy = dy / sqrt(dx*dx + dy*dy);

						if (/*만약 붓이 확 꺾인다면*/last_dx * dx + last_dy * dy < 0) {
							dx *= -1;
							dy *= -1;
						}

						CvPoint new_point;

						new_point.x = current_point.x + s.radius*dx;
						new_point.y = current_point.y + s.radius*dy;
						if(current_point.x < 0) {
							current_point.x = 0;
							is_out=1;
						}
						if(current_point.y < 0) {
							current_point.y = 0;
							is_out = 1;

						}
						if(current_point.x > w-1) {
							current_point.x = w-1;
							is_out = 1;

						}
						if(current_point.y > h-1) {
							current_point.y = h-1;
							is_out = 1;
						}
						//cvSet2D(canvas, new_point.y, new_point.x, cvScalar(0,0,0));

						s.route[s.points_cnt++] = new_point;

						last_dx = dx;
						last_dy = dy;

						current_point = new_point;

						if (s.points_cnt > max_stroke_length || is_out==1) {
							break;
						}
						
					}

					stroke_arr[stroke_cnt++] = s;
				}
			}
		}

		shuffleArr(stroke_arr, stroke_cnt);
		for (int i = 0; i < stroke_cnt; i++) {
			drawSplineStroke(canvas, stroke_arr[i]);
		}
		for (int i = 0; i < stroke_cnt; i++) {
			free(stroke_arr[i].route);
		}
		free(stroke_arr);

		r /= 2;
		jitteredGrid.width /= 2;
		jitteredGrid.height /= 2;
		jitteredGrid.colsCnt = (imgSize.width) / (jitteredGrid.width) + 1;
		jitteredGrid.rowsCnt = (imgSize.height) / (jitteredGrid.height) + 1;
		cvShowImage("canvas", canvas);
		cvWaitKey();
	}

	cvWaitKey();

}