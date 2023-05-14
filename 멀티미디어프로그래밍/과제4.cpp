#include <opencv2/opencv.hpp>

//파라미터 정의
#define R 32	//원의 반지름 (또는 Stroke	의 크기)
#define G 4		//가우시안 블러링할 때 반지름에 곱해지는 변수
#define fg 1	//jittered Grid를 생성할 때 반지름에 곱해지는 변수
#define T 20	//threshold값 (그림의 거친 정도를 결정하는 변수)
#define fc 1.0f	//Curved Brush Stroke의 곡률을 결정하는 변수
#define MAX_STROKE_LENGTH 10	//Curved Brush Stroke를 이루는 점들의 최대 개수
#define MIN_STROKE_LENGTH 2		//Curved Brush Stroke를 이루는 점들의 최소 개수

//구조체 정의
typedef struct Circle {	//Circle 구조체
	int radius;			//원의 반지름
	CvScalar color;		//원의 색깔
	CvPoint point;		//원의 중심의 좌표
}Circle;
typedef struct Grid {	//Jittered Grid 구조체
	int colsCnt;		//격자판의 열 개수
	int rowsCnt;		//격자판의 행 개수
	int length;			//격자 하나의 한 변의 길이
}Grid;
typedef struct Stroke {	//Stroke 구조체
	CvPoint start_point;//Stroke의 시작좌표
	CvPoint* route;		//Control Point의 배열(붓이 움직이는 경로)
	int points_cnt;		//Control Point의 개수
	int radius;			//Stroke의 크기
	CvScalar color;		//Stroke의 색깔
}Stroke;

//인자로 이미지와 Circle구조체형 변수를 입력받아 이미지에 원을 그리는 함수
void drawCircle(IplImage* img, Circle circle) {
	cvCircle(img, circle.point, circle.radius, circle.color, -1);
}

//두 개의 CvScalar형 변수를 인자로 넘겨받아 두 변수의 색깔차이를 계산하는 함수
float getDifference(CvScalar f, CvScalar g) {
	float sum = 0.0;
	//각 BGR성분의 차이를 제곱하여 더함.
	for (int k = 0; k < 3; k++) {
		sum += (f.val[k] - g.val[k]) * (f.val[k] - g.val[k]);
	}
	//루트를 씌운 값을 리턴
	return sqrt(sum);
}

//Circle형 변수를 담은 배열의 원소들을 무작위로 섞는 함수
void shuffleCircleArr(Circle* circleArr, int size) {
	//size만큼 반복
	for (int i = 0; i < size; i++) {

		//0이상 size미만의 무작위 정수 생성
		int ran = rand() % size;
		
		//i번째 인덱스와 ran번째 인덱스에 있는 원소를 서로 바꿈
		Circle tmp = circleArr[i];
		circleArr[i] = circleArr[ran];
		circleArr[ran] = tmp;
	}
}

//Curved Brush Stroke를 그리는 함수
void drawSplineStroke(IplImage* img, Stroke stroke) {

	//출발 좌표
	CvPoint st = stroke.start_point;

	//도착 좌표
	CvPoint ed;

	for (int i = 0; i < stroke.points_cnt; i++) {

		//도착좌표 설정
		ed = stroke.route[i];

		//stroke에 저장된 정보를 토대로 출발 좌표에서 도착 좌표까지 선을 그음.
		cvLine(img, st, ed, stroke.color, stroke.radius);

		//도착 좌표는 다음 직선의 출발 좌표가 된다.
		st = ed;
	}
}

//Stroke형 배열을 무작위로 섞는 함수
void shuffleStrokeArr(Stroke* stroke_arr, int size) {
	for (int i = 0; i < size; i++) {
		int ran = rand() % size;
		Stroke tmp = stroke_arr[i];
		stroke_arr[i] = stroke_arr[ran];
		stroke_arr[ran] = tmp;
	}
}

//원본이미지 src를 canvas에 원으로 그림을 그리는 함수
void drawWithCircle(IplImage* src, IplImage* canvas) {
	
	//src와 canvas 띄움.
	cvShowImage("src", src);
	cvShowImage("canvas", canvas);
	cvWaitKey(1000);

	//이미지의 크기 가져옴.
	CvSize img_size = cvGetSize(src);

	//레퍼런스 이미지 생성(src와 같은 크기로)
	IplImage* ref_img = cvCreateImage(img_size, 8, 3);

	//이미지의 높이와 너비 저장.
	int w = img_size.width;
	int h = img_size.height;

	//원의 반지름 저장.
	int r = R;

	//Jittered Grid생성
	Grid jittered_grid;
	jittered_grid.length = fg*R;	//한 변의 길이 설정
	jittered_grid.colsCnt = (img_size.width) / (jittered_grid.length) + 1;	//열의 개수 설정
	jittered_grid.rowsCnt = (img_size.height) / (jittered_grid.length) + 1;	//행의 개수 설정

	//각 층을 그릴 때마다 증가할 변수
	int t = 0;

	//canvas는 5개의 층으로 구성될 것이다.
	while (t < 5) {

		//각 층마다 그릴 원들을 담을 배열(grid의 총 개수만큼 동적할당 받음)
		Circle* circleArr = (Circle*)malloc(sizeof(Circle) * (jittered_grid.colsCnt * jittered_grid.rowsCnt));

		//원의 개수
		int circleCnt = 0;

		//src에 가우시안 블러를 적용하여 ref_img 생성 (그림을 뭉개는 정도는 원의 반지름에 비례)
		cvSmooth(src, ref_img, CV_GAUSSIAN, G * r - 1);

		//(x, y)는 각 격자의 왼쪽 위 꼭짓점의 좌표
		for (int y = 0; y < h; y += jittered_grid.length) {
			for (int x = 0; x < w; x += jittered_grid.length) {

				//격자 안에서의 최대오차와 평균 오차가 저장될 변수
				float max_diff = 0;
				float diff_avg = 0.0;

				//최대오차의 좌표와 그 좌표에서의 컬러값이 저장될 변수
				CvPoint max_diff_pos = { 0,0 };
				CvScalar max_diff_color = cvScalar(255, 255, 255);

				//(u, v)는 각 격자 안에서의 x좌표와 y좌표
				for (int v = y; v < y + jittered_grid.length; v++) {
					for (int u = x; u < x + jittered_grid.length; u++) {

						//u와 v가 이미지 밖이라면 연산을 진행하지 않음
						if (u > w - 1 || v > h - 1)	continue;
						if (u < 0 || v < 0) continue;

						//레퍼런스 이미지의 컬러값
						CvScalar ref_color = cvGet2D(ref_img, v, u);

						//canvas의 컬러값
						CvScalar canvas_color = cvGet2D(canvas, v, u);

						//ref_color와 canvas_color의 오차를 계산
						float diff = getDifference(ref_color, canvas_color);

						//만약 계산한 오차가 최대 오차라면
						if (diff > max_diff) {
							max_diff = diff;		//최대오차 초기화
							max_diff_pos.x = u;		//최대오차의 좌표 초기화
							max_diff_pos.y = v;
							max_diff_color = ref_color;	//최대오차의 컬러값 초기화
						}
						//평균오차를 저장하는 변수에 오차값을 더해줌.(마지막에 격자의 크기로 나눠줄 것이다.)
						diff_avg += diff;
					}
				}

				//평균 오차 계산
				diff_avg /= (jittered_grid.length * jittered_grid.length);

				//그 격자의 평균 오차가 T보다 작다면 canvas와 ref_img는 이미 비슷하므로 원을 그릴 필요가 없다.
				if (diff_avg > T) {
					//평균오차가 T보다 클 때만 circleArr에 원을 추가
					Circle c;
					c.radius = r;
					c.color = max_diff_color;
					c.point = max_diff_pos;
					circleArr[circleCnt++] = c;
				}
			}
		}

		//연산을 이미지의 왼쪽 위부터 진행하기 때문에 항상 왼쪽 위에 그려질 원이 먼저 그려지게 된다.
		//이것을 막기 위해 circleArr을 무작위로 섞는다.
		shuffleCircleArr(circleArr, circleCnt);

		for (int i = 0; i < circleCnt; i++) {
			//circleArr에 저장된 원을 canvas에 그림
			drawCircle(canvas, circleArr[i]);
		}
		//circleArr 동적할당 해제
		free(circleArr);

		//반지름을 줄임
		r /= 2;
		//격자의 크기도 줄임
		jittered_grid.length /= 2;
		jittered_grid.colsCnt = (img_size.width) / (jittered_grid.length) + 1;
		jittered_grid.rowsCnt = (img_size.height) / (jittered_grid.length) + 1;

		//canvas이미지 띄움
		cvShowImage("canvas", canvas);
		cvWaitKey(1000);

		t++;
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
	jittered_grid.length = fg*R;
	jittered_grid.colsCnt = (img_size.width) / (jittered_grid.length) + 1;
	jittered_grid.rowsCnt = (img_size.height) / (jittered_grid.length) + 1;

	while (r >= 2) {
		Stroke* stroke_arr = (Stroke*)malloc(sizeof(Stroke) * (jittered_grid.colsCnt * jittered_grid.rowsCnt));
		int stroke_cnt = 0;

		cvSmooth(src, ref_img, CV_GAUSSIAN, G * r - 1);

		for (int y = 0; y < h; y += jittered_grid.length) {
			for (int x = 0; x < w; x += jittered_grid.length) {
				float max_diff = 0;
				float diff_avg = 0.0;
				CvPoint max_diff_pos = { 0,0 };
				CvScalar max_diff_color = cvScalar(255, 255, 255);

				for (int v = y; v < y + jittered_grid.length; v++) {
					for (int u = x; u < x + jittered_grid.length; u++) {
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

				diff_avg = diff_avg / (jittered_grid.length * jittered_grid.length);

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

						dx = fc * dx + (1 - fc) * last_dx;
						dy = fc * dy + (1 - fc) * last_dy;

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
		jittered_grid.length /= 2;
		jittered_grid.colsCnt = (img_size.width) / (jittered_grid.length) + 1;
		jittered_grid.rowsCnt = (img_size.height) / (jittered_grid.length) + 1;
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