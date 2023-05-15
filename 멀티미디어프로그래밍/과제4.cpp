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

//src이미지를 토대로 canvas에 Curved Brush Stroke로 그림을 그리는 함수
void drawWithSplineStrokes(IplImage* src, IplImage* canvas) {

	//src와 canvas 띄움
	cvShowImage("src", src);
	cvShowImage("canvas", canvas);
	cvWaitKey(1000);

	//이미지 크기를 저장하는 변수
	CvSize img_size = cvGetSize(src);

	//src와 같은 크기를 가진 레퍼런스 이미지 생성
	IplImage* ref_img = cvCreateImage(img_size, 8, 3);

	//이미지의 너비와 높이 저장할 변수
	int w = img_size.width;
	int h = img_size.height;

	//선의 반지름
	int r = R;

	//Jittered Grind 생성
	Grid jittered_grid;
	jittered_grid.length = fg*R;	//각 격자의 한 변의 길이 정의(반지름에 비례함)
	jittered_grid.colsCnt = (img_size.width) / (jittered_grid.length) + 1;	//격자의 열의 개수
	jittered_grid.rowsCnt = (img_size.height) / (jittered_grid.length) + 1;	//격자의 행의 개수

	int t=0;

	while (t < 5) {

		//Stroke를 저장할 배열
		Stroke* stroke_arr = (Stroke*)malloc(sizeof(Stroke) * (jittered_grid.colsCnt * jittered_grid.rowsCnt));

		//Stroke의 개수
		int stroke_cnt = 0;

		//src이미지에 가우시안 블러링을 적용하여 레퍼런스 이미지를 생성한다.
		cvSmooth(src, ref_img, CV_GAUSSIAN, G * r - 1);

		//(x, y)는 각 격자의 왼쪽 위 꼭짓점의 좌표(x와 y는 격자의 길이만큼 증가)
		for (int y = 0; y < h; y += jittered_grid.length) {
			for (int x = 0; x < w; x += jittered_grid.length) {
				
				//격자 안의 최대 오차를 저장할 변수
				float max_diff = 0;

				//격자의 평균 오차를 저장할 변수
				float diff_avg = 0.0;

				//최대 오차의 좌표와 그 좌표에서의 컬러값을 저장할 변수
				CvPoint max_diff_pos = { 0,0 };
				CvScalar max_diff_color = cvScalar(255, 255, 255);

				//(u, v)는 격자 안에서의 x좌표와 y좌표
				for (int v = y; v < y + jittered_grid.length; v++) {
					for (int u = x; u < x + jittered_grid.length; u++) {

						//만약 u, v가 이미지 밖이라면 연산을 진행하지 않음
						if (u > w - 1 || v > h - 1)	continue;
						if (u < 0 || v < 0) continue;

						//레퍼런스 이미지와 캔버스에서 (u, v)에서의 컬러값이 저장될 변수
						CvScalar refColor = cvGet2D(ref_img, v, u);
						CvScalar canvasColor = cvGet2D(canvas, v, u);

						//두 변수 사이의 오차를 계산
						float diff = getDifference(refColor, canvasColor);

						//만약 그 오차가 최대 오차라면
						if (diff > max_diff) {	
							max_diff = diff;		//최대오차값 업데이트
							max_diff_pos.x = u;		//최대오차가 생기는 곳의 좌표 업데이트
							max_diff_pos.y = v;
							max_diff_color = refColor;	//최대오차에서의 컬러값 업데이트
						}

						//평균오차를 저장하는 변수에 오차를 더해준다.
						diff_avg += diff;
					}
				}

				//모든 오차값이 더해진 변수를 격자의 크기로 나누어 각 격자에서의 평균 오차를 구해준다.
				diff_avg = diff_avg / (jittered_grid.length * jittered_grid.length);

				//만약 평균 오차가 미리 설정된 파라미터 값보다 크다면
				if (diff_avg > T) {

					//Stroke 생성
					Stroke s;
					s.route = (CvPoint*)malloc(sizeof(CvPoint) * MAX_STROKE_LENGTH);	//Control Point의 최대 개수만큼 메모리 동적할당 받음.
					s.radius = r;					//선의 크기 설정
					s.color = max_diff_color;		//선의 색깔 설정
					s.start_point = max_diff_pos;	//선의 시작 위치 설정
					s.points_cnt = 0;				//Control Point의 개수

					CvPoint current_point = s.start_point;	//현재 위치 업데이트
					float last_dx = 0, last_dy = 0;			//마지막으로 Point를 움직인 방향

					while (true) {
						//레퍼런스 이미지에서 현재 위치에서의 컬러값
						CvScalar ref_color = cvGet2D(ref_img, current_point.y, current_point.x);

						//canvas에서 현재 위치에서의 컬러값
						CvScalar canvas_color = cvGet2D(canvas, current_point.y, current_point.x);

						//만약 점의 개수가 최소개수보다 많고 레퍼런스 이미지와 canvas의 오차보다 레퍼런스 이미지와 선의 색깔의 오차가 더 크다면 반복문 탈출
						if (MIN_STROKE_LENGTH < s.points_cnt && getDifference(ref_color, canvas_color) < getDifference(ref_color, s.color)) break;

						//만약 점의 개수가 최대 개수라면 반복문 탈출
						if (s.points_cnt >= MAX_STROKE_LENGTH) break;

						//Image Gradient를 계산할 때 사용되는 좌표변수
						//x방향의 Gradient : L(p2.x, y) - L(p1.x, y)
						//y방향의 Gradient : L(x, p2.y) - L(x, p1.y)
						CvPoint p1 = cvPoint(current_point.x - 1, current_point.y - 1);
						CvPoint p2 = cvPoint(current_point.x + 1, current_point.y + 1);

						//이 부분 주석 필요
						//만약 p1이나 p2가 이미지를 벗어난다면
						//그 좌표를 이미지 안으로 이동시킨다.
						if (p1.x < 0) p1.x = 0;
						if (p1.y < 0) p1.y = 0;
						if (p2.x > w - 1) p2.x = w - 1;
						if (p2.y > h - 1) p2.y = h - 1;

						
						//현재 좌표를 기준으로 각각 왼쪽, 오른쪽, 위, 아래에 있는 픽셀의 컬러값을 저장할 변수
						CvScalar x1_color, x2_color, y1_color, y2_color;
						x1_color = cvGet2D(ref_img, current_point.y, p1.x);
						x2_color = cvGet2D(ref_img, current_point.y, p2.x);
						y1_color = cvGet2D(ref_img, p1.y, current_point.x);
						y2_color = cvGet2D(ref_img, p2.y, current_point.x);

						//Image Gradient를 저장할 변수
						float gx = 0, gy = 0;
						for (int k = 0; k < 3; k++) {
							//x방향으로의 Gradient를 계산함
							gx += x2_color.val[k] - x1_color.val[k];

							//y방향으로의 Gradient를 계산함
							gy += y2_color.val[k] - y1_color.val[k];
						}

						//만약 Gradient가 0이라면 (이미지의 밝기에 변화가 없다면)
						if (gx == 0 && gy == 0) break;

						//새로운 좌표를 구할 때 사용되는 변수(현재좌표를 기준으로 x방향으로 dx만큼, y방향으로 dy만큼 간 좌표가 새로운 좌표가 된다.)
						float dx, dy;

						//Image Gradient의 수직 방향
						dx = -gy;
						dy = gx;

						//마지막 dx와 dy를 기준으로 dx, dy가 급격히 꺾였다면
						if (last_dx * dx + last_dy * dy < 0) {
							//dx와 dy를 반대방향으로 바꿔줌
							dx *= -1;
							dy *= -1;
						}

						//이미지의 곡률을 정하는 변수 fc에 따라 곡률을 정함.
						dx = fc * dx + (1 - fc) * last_dx;
						dy = fc * dy + (1 - fc) * last_dy;

						//dx와 dy에 반지름을 곱해야 하므로 dx와 dy는 단위벡터처럼 생각할 수 있다.
						dx = dx / sqrt(dx * dx + dy * dy);
						dy = dy / sqrt(dx * dx + dy * dy);

						//새로운 좌표를 저장할 변수
						CvPoint new_point;

						//현재 좌표를 기준으로 dx방향으로 r만큼, dy방향으로 r만큼 간 곳에 새로운 좌표가 있다.
						new_point.x = current_point.x + s.radius * dx;
						new_point.y = current_point.y + s.radius * dy;

						//만약 그 좌표가 이미지 밖이라면
						//이미지 안으로 들어올 수 있도록 설정해준다.
						if (new_point.x < 0) new_point.x = 0;
						if (new_point.y < 0) new_point.y = 0;
						if (new_point.x > w - 1) new_point.x = w - 1;
						if (new_point.y > h - 1) new_point.y = h - 1;

						//lastdx와 lastdy를 업데이트
						last_dx = dx;
						last_dy = dy;

						//현재 좌표 업데이트
						current_point = new_point;

						//Stroke의 경로에 새로운 좌표 추가
						s.route[s.points_cnt++] = new_point;
					}

					//반복문이 끝난 뒤에, stroke 배열에 s를 추가
					stroke_arr[stroke_cnt++] = s;
				}
			}
		}

		//Stroke에 있는 요소들을 무작위로 섞음
		shuffleStrokeArr(stroke_arr, stroke_cnt);
		for (int i = 0; i < stroke_cnt; i++) {
			drawSplineStroke(canvas, stroke_arr[i]);
		}
		
		//Stroke배열의 경로를 저장하는 배열 동적할당 해제
		for (int i = 0; i < stroke_cnt; i++) {
			free(stroke_arr[i].route);
		}
		//Stroke 배열 동적할당 해제
		free(stroke_arr);

		//Stroke의 크기를 줄인다.
		r /= 2;
		//격자의 크기를 줄이고 행의 개수와 열의 개수를 업데이트 해준다.
		jittered_grid.length /= 2;
		jittered_grid.colsCnt = (img_size.width) / (jittered_grid.length) + 1;
		jittered_grid.rowsCnt = (img_size.height) / (jittered_grid.length) + 1;

		t++;

		//canvas이미지 띄우기
		cvShowImage("canvas", canvas);
		cvWaitKey(1000);
	}
	cvWaitKey();
}

int main() {
	//이미지의 경로가 저장될 변수
	char src[1000];
	IplImage *src_img;
	while (true) {
		//경로 입력받음
		printf("Input File Path: ");
		scanf("%s",src);
		src_img = cvLoadImage(src);

		//만약 경로가 잘못되었다면 반복문을 탈출하지 못함
		if (src_img != nullptr) {
			break;
		}
		printf("File not Found!\n");
	}

	//그리기 모드 입력받는 변수
	int draw_mode = -1;
	while (true) {
		printf("Select Drawing Mode (0=circle, 1=stroke):");
		scanf("%d",&draw_mode);

		//입력한 값이 0이나 1이 아니라면 반복문을 탈출하지 못함
		if (draw_mode == 0 || draw_mode == 1) {
			break;
		}
		printf("Wrong Drawing Mode!\n");
	}

	//원본 이미지의 크기를 저장하여 같은 크기의 canvas를 생성함
	CvSize imgSize = cvGetSize(src_img);
	IplImage* canvas = cvCreateImage(imgSize, 8, 3);

	//canvas를 하얀색으로 초기화
	cvSet(canvas, cvScalar(255, 255, 255));

	if (draw_mode == 0) {		//원으로 그리기 모드
		drawWithCircle(src_img, canvas);
	}
	else {						//Stroke로 그리기 모드
		drawWithSplineStrokes(src_img, canvas);
		
	}

	return 0;
}