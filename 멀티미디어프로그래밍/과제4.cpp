#include <opencv2/opencv.hpp>
#define R 32
#define L 32
#define T 1200
#define Fc 1.0f
#define MAX_STROKE_LENGTH 10
#define MIN_STROKE_LENGTH 2
typedef struct Circle {
	int radius;
	CvScalar color;
	CvPoint point;
}Circle;
typedef struct Grid {
	int colsCnt;	//격자판의 가로 개수
	int rowsCnt;	//격자판의 세로 개수
	int width;		//격자 하나의 가로 길이(너비)
	int height;		//격자 하나의 세로 길이(높이)
}Grid;
typedef struct Stroke {
	CvPoint start_point;
	CvPoint* route;
	int points_cnt;
	int radius;
	CvScalar color;
}Stroke;

void drawCircle(IplImage* img, Circle circle) {
	cvCircle(img, circle.point, circle.radius, circle.color, -1);
}

float getDifference(CvScalar f, CvScalar g) {
	float sum = 0.0;
	for (int k = 0; k < 3; k++) {
		sum += (f.val[k] - g.val[k]) * (f.val[k] - g.val[k]);
	}
	return sum;
}

void shuffleCircleArr(Circle* circleArr, int size) {
	for (int i = 0; i < size; i++) {
		int ran = rand() % size;
		Circle tmp = circleArr[i];
		circleArr[i] = circleArr[ran];
		circleArr[ran] = tmp;
	}
}

void drawSplineStroke(IplImage* img, Stroke stroke) {
	CvPoint st = stroke.start_point;
	CvPoint ed;
	for (int i = 0; i < stroke.points_cnt; i++) {
		ed = stroke.route[i];
		cvLine(img, st, ed, stroke.color, stroke.radius);
		st = ed;
	}
}

void shuffleStrokeArr(Stroke* stroke_arr, int size) {
	for (int i = 0; i < size; i++) {
		int ran = rand() % size;
		Stroke tmp = stroke_arr[i];
		stroke_arr[i] = stroke_arr[ran];
		stroke_arr[ran] = tmp;
	}
}

void drawWithCircle(IplImage* src, IplImage* canvas) {

	cvShowImage("src", src);
	cvShowImage("canvas", canvas);
	cvWaitKey(1000);

	CvSize img_size = cvGetSize(src);

	IplImage* ref_img = cvCreateImage(img_size, 8, 3);

	int w = img_size.width;
	int h = img_size.height;

	int r = R;

	Grid jittered_grid;
	jittered_grid.width = L;
	jittered_grid.height = L;
	jittered_grid.colsCnt = (img_size.width) / (jittered_grid.width) + 1;
	jittered_grid.rowsCnt = (img_size.height) / (jittered_grid.height) + 1;

	while (r >= 2) {
		Circle* circleArr = (Circle*)malloc(sizeof(Circle) * (jittered_grid.colsCnt * jittered_grid.rowsCnt));

		int circleCnt = 0;

		cvSmooth(src, ref_img, CV_GAUSSIAN, 4 * r - 1);

		for (int y = 0; y < h; y += jittered_grid.height) {
			for (int x = 0; x < w; x += jittered_grid.width) {
				float max_diff = 0;
				float diff_avg = 0.0;
				CvPoint max_diff_pos = { 0,0 };
				CvScalar max_diff_color = cvScalar(255, 255, 255);

				for (int v = y; v < y + jittered_grid.height; v++) {
					for (int u = x; u < x + jittered_grid.width; u++) {
						if (u > w - 1 || v > h - 1)	continue;
						if (u < 0 || v < 0) continue;

						CvScalar ref_color = cvGet2D(ref_img, v, u);

						CvScalar canvas_color = cvGet2D(canvas, v, u);

						float diff = getDifference(ref_color, canvas_color);
						if (diff > max_diff) {
							max_diff = diff;
							max_diff_pos.x = u;
							max_diff_pos.y = v;
							max_diff_color = ref_color;
						}
						diff_avg += diff;
					}
				}

				diff_avg /= (jittered_grid.width * jittered_grid.height);

				if (diff_avg > T) {
					Circle c;
					c.radius = r;
					c.color = max_diff_color;
					c.point = max_diff_pos;
					circleArr[circleCnt++] = c;
				}
			}
		}

		shuffleCircleArr(circleArr, circleCnt);
		for (int i = 0; i < circleCnt; i++) {
			drawCircle(canvas, circleArr[i]);
		}
		free(circleArr);

		r /= 2;
		jittered_grid.width /= 2;
		jittered_grid.height /= 2;
		jittered_grid.colsCnt = (img_size.width) / (jittered_grid.width) + 1;
		jittered_grid.rowsCnt = (img_size.height) / (jittered_grid.height) + 1;
		cvShowImage("canvas", canvas);
		cvWaitKey(1000);
	}
	cvWaitKey();
}

void drawWithSplineStrokes(IplImage* src, IplImage* canvas) {

	cvShowImage("src", src);
	cvShowImage("canvas", canvas);
	cvWaitKey(1000);

	CvSize img_size = cvGetSize(src);
	IplImage* ref_img = cvCreateImage(img_size, 8, 3);

	int w = img_size.width;
	int h = img_size.height;

	//선의 반지름
	int r = R;

	Grid jittered_grid;
	jittered_grid.width = L;
	jittered_grid.height = L;
	jittered_grid.colsCnt = (img_size.width) / (jittered_grid.width) + 1;
	jittered_grid.rowsCnt = (img_size.height) / (jittered_grid.height) + 1;

	while (r >= 2) {
		Stroke* stroke_arr = (Stroke*)malloc(sizeof(Stroke) * (jittered_grid.colsCnt * jittered_grid.rowsCnt));
		int stroke_cnt = 0;

		cvSmooth(src, ref_img, CV_GAUSSIAN, 4 * r - 1);

		for (int y = 0; y < h; y += jittered_grid.height) {
			for (int x = 0; x < w; x += jittered_grid.width) {
				float max_diff = 0;
				float diff_avg = 0.0;
				CvPoint max_diff_pos = { 0,0 };
				CvScalar max_diff_color = cvScalar(255, 255, 255);

				for (int v = y; v < y + jittered_grid.height; v++) {
					for (int u = x; u < x + jittered_grid.width; u++) {
						if (u > w - 1 || v > h - 1)	continue;
						if (u < 0 || v < 0) continue;

						CvScalar refColor = cvGet2D(ref_img, v, u);
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

				diff_avg = diff_avg / (jittered_grid.width * jittered_grid.height);

				if (diff_avg > T) {

					Stroke s;
					s.route = (CvPoint*)malloc(sizeof(CvPoint) * MAX_STROKE_LENGTH);
					s.radius = r;
					s.color = max_diff_color;
					s.start_point = max_diff_pos;
					s.points_cnt = 0;

					CvPoint current_point = s.start_point;
					float last_dx = 0, last_dy = 0;

					while (true) {

						CvScalar ref_color = cvGet2D(ref_img, current_point.y, current_point.x);
						CvScalar canvas_color = cvGet2D(ref_img, current_point.y, current_point.x);
						if (MIN_STROKE_LENGTH < s.points_cnt && getDifference(ref_color, canvas_color) < getDifference(ref_color, s.color)) break;
						if (s.points_cnt >= MAX_STROKE_LENGTH) break;

						CvPoint p1 = cvPoint(current_point.x - 1, current_point.y - 1);
						CvPoint p2 = cvPoint(current_point.x + 1, current_point.y + 1);

						if (p1.x < 0) p1.x = 0;
						if (p1.y < 0) p1.y = 0;
						if (p2.x > w - 1) p2.x = w - 1;
						if (p2.y > h - 1) p2.y = h - 1;

						CvScalar x1_color, x2_color, y1_color, y2_color;
						x1_color = cvGet2D(ref_img, current_point.y, p1.x);
						x2_color = cvGet2D(ref_img, current_point.y, p2.x);
						y1_color = cvGet2D(ref_img, p1.y, current_point.x);
						y2_color = cvGet2D(ref_img, p2.y, current_point.x);

						float gx = 0, gy = 0;
						for (int k = 0; k < 3; k++) {
							gx += x2_color.val[k] - x1_color.val[k];
							gy += y2_color.val[k] - y1_color.val[k];
						}

						if (gx == 0 && gy == 0) break;

						float dx, dy;
						dx = -gy;
						dy = gx;

						//만약 방향이 확 꺾였다면?
						if (last_dx * dx + last_dy * dy < 0) {
							dx *= -1;
							dy *= -1;
						}

						dx = Fc * dx + (1 - Fc) * last_dx;
						dy = Fc * dy + (1 - Fc) * last_dy;

						//d : 단위벡터?
						dx = dx / sqrt(dx * dx + dy * dy);
						dy = dy / sqrt(dx * dx + dy * dy);

						CvPoint new_point;
						new_point.x = current_point.x + s.radius * dx;
						new_point.y = current_point.y + s.radius * dy;
						if (new_point.x < 0) new_point.x = 0;
						if (new_point.y < 0) new_point.y = 0;
						if (new_point.x > w - 1) new_point.x = w - 1;
						if (new_point.y > h - 1) new_point.y = h - 1;

						last_dx = dx;
						last_dy = dy;

						current_point = new_point;

						s.route[s.points_cnt++] = new_point;
					}
					stroke_arr[stroke_cnt++] = s;
				}
			}
		}

		shuffleStrokeArr(stroke_arr, stroke_cnt);
		for (int i = 0; i < stroke_cnt; i++) {
			drawSplineStroke(canvas, stroke_arr[i]);
		}
		for (int i = 0; i < stroke_cnt; i++) {
			free(stroke_arr[i].route);
		}
		free(stroke_arr);

		r /= 2;
		jittered_grid.width /= 2;
		jittered_grid.height /= 2;
		jittered_grid.colsCnt = (img_size.width) / (jittered_grid.width) + 1;
		jittered_grid.rowsCnt = (img_size.height) / (jittered_grid.height) + 1;
		cvShowImage("canvas", canvas);
		cvWaitKey(1000);
	}
	cvWaitKey();
}

int main() {
	char src[1000];
	IplImage *src_img;
	while (true) {
		printf("Input File Path: ");
		scanf("%s",src);
		src_img = cvLoadImage(src);
		if (src_img != nullptr) {
			break;
		}
		printf("File not Found!\n");
	}

	int draw_mode = -1;
	while (true) {
		printf("Select Drawing Mode (0=circle, 1=stroke):");
		scanf("%d",&draw_mode);
		if (draw_mode == 0 || draw_mode == 1) {
			break;
		}
		printf("Wrong Drawing Mode!\n");
	}

	CvSize imgSize = cvGetSize(src_img);
	IplImage* canvas = cvCreateImage(imgSize, 8, 3);

	cvSet(canvas, cvScalar(255, 255, 255));

	if (draw_mode == 0) {
		drawWithCircle(src_img, canvas);
	}
	else {
		drawWithSplineStrokes(src_img, canvas);
	}

}