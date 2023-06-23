#include <opencv2/opencv.hpp>
#include "MatrixInverse.h"
#include "vec.h"
#include "mat.h"

IplImage* src = nullptr;
IplImage* dst = nullptr;
int W = 500;										// 결과 이미지 가로 크기
int H = 500;										// 결과 이미지 세로 크기

vec3 pos[8] = {										// 육면체를 구성하는 8개의 꼭지점 3차원 좌표
		vec3(-0.5, -0.5,  0.5),
		vec3(-0.5,  0.5,  0.5),
		vec3(0.5,  0.5,  0.5),
		vec3(0.5, -0.5,  0.5),
		vec3(-0.5, -0.5, -0.5),
		vec3(-0.5,  0.5, -0.5),
		vec3(0.5,  0.5, -0.5),
		vec3(0.5, -0.5, -0.5) };



struct rect											// 사각형 한 면
{
	int ind[4];										// 꼭지점의 인덱스
	vec3 pos[4];									// 꼭지점의 화면 방향으로의 3차원 위치
	vec3 nor;										// 법선(normal) 벡터 방향 (= 면이 향하는 방향)
};

rect setRect(int a, int b, int c, int d)			// 사각형 정보를 채워주는 함수(바로 아래 cube정의에 사용)
{
	rect r;
	r.ind[0] = a;
	r.ind[1] = b;
	r.ind[2] = c;
	r.ind[3] = d;
	return r;
}

rect cube[6] = { setRect(1, 0, 3, 2),				// 사각형 6개를 정의해 육면체를 구성
				 setRect(2, 3, 7, 6),
				 setRect(3, 0, 4, 7),
				 setRect(6, 5, 1, 2),
				 setRect(6, 7, 4, 5),
				 setRect(5, 4, 0, 1) };

vec3 epos = vec3(1.5, 1.5, 1.5);					// 카메라(시점의 3차원) 위치
mat4 ModelMat;										// 모델에 변형을 주는 변형 행렬
mat4 ViewMat;										// 카메라 시점을 맞춰주는 변형 행렬
mat4 ProjMat;										// 화면상 위치로 투영해주는 변형 행렬

void init()											// 초기화
{
	ModelMat = mat4(1.0f);
	ViewMat = LookAt(epos, vec3(0, 0, 0), vec3(0, 1, 0));
	// 카메라 위치(epos)에서 (0,0,0)을 바라보는 카메라 설정			
	ProjMat = Perspective(45, W / (float)H, 0.1, 100);
	// 45도의 시야각을 가진 투영 변환 (가시거리 0.1~100)
}

void rotateModel(float rx, float ry, float rz)		// 육면체 모델에 회전을 적용하는 함수
{
	ModelMat = RotateX(rx) * RotateY(ry) * RotateZ(rz) * ModelMat;
}

vec3 convert3Dto2D(vec3 in)							// 3차원 좌표를 화면에 투영된 2차원+깊이값(z) 좌표로 변환
{
	vec4 p = ProjMat * ViewMat * ModelMat * vec4(in);
	p.x /= p.w;
	p.y /= p.w;
	p.z /= p.w;
	p.x = (p.x + 1) / 2.0f * W;
	p.y = (-p.y + 1) / 2.0f * H;
	return vec3(p.x, p.y, p.z);
}

void updatePosAndNormal(rect* r, vec3 p[])			// 육면체의 회전에 따른 각 면의 3차원 좌표 및 법선 벡터 방향 업데이트
{
	for (int i = 0; i < 4; i++)
		r->pos[i] = convert3Dto2D(p[r->ind[i]]);
	vec3 a = normalize(r->pos[0] - r->pos[1]);
	vec3 b = normalize(r->pos[2] - r->pos[1]);
	r->nor = cross(a, b);
}

void setMultiply(float M[][8], float A[][9], float B[][8]) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			M[i][j] = 0.0f;
			for (int k = 0; k < 9; k++) {
				M[i][j] += A[i][k] * B[k][j];

			}
		}
	}
}

void doHomography(IplImage* src, IplImage* dst, rect pts) {
	//src의 네 꼭짓점을 pts.pos[0~4]로 보내서 dst에 그리기

	//we are trying to find M, and A*M = 0
	float A[8][9], transA[9][8];
	float AAt[8][8], inverseAAt[8][8];
	float pseudoInverseA[9][8];
	float M[3][3];

	CvPoint p1 = cvPoint(0, 0);		//go to pts.pos[0]
	CvPoint p2 = cvPoint(0, H - 1);	//go to pts.pos[1]
	CvPoint p3 = cvPoint(W - 1, H - 1);	//go to pts.pos[2]
	CvPoint p4 = cvPoint(W - 1, 0);	//go to pts.pos[3]

	for (int i = 0; i < 4; i++) {
		CvPoint p;
		if (i == 0) p = p1;
		if (i == 1) p = p2;
		if (i == 2) p = p3;
		if (i == 3) p = p4;
		A[2 * i][0] = float(-p.x)/(W-1);
		A[2 * i][1] = float(-p.y)/(H-1);
		A[2 * i][2] = -1;
		A[2 * i][3] = 0;
		A[2 * i][4] = 0;
		A[2 * i][5] = 0;
		A[2 * i][6] = float(pts.pos[i].x)/(W-1) * float(p.x)/(W-1);
		A[2 * i][7] = float(pts.pos[i].x)/(W-1) * float(p.y)/(H-1);
		A[2*i][8] = float(pts.pos[i].x)/(W-1);

		A[2 * i + 1][0] = 0;
		A[2 * i + 1][1] = 0;
		A[2 * i + 1][2] = 0;
		A[2 * i + 1][3] = float(-p.x)/(W-1);
		A[2 * i + 1][4] = float(-p.y)/(H-1);
		A[2 * i + 1][5] = -1;
		A[2 * i + 1][6] = float(pts.pos[i].y)/(H-1) * float(p.x)/(W-1);
		A[2 * i + 1][7] = float(pts.pos[i].y)/(H-1) * float(p.y)/(H-1);
		A[2*i+1][8] = float(pts.pos[i].y)/(H-1);
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 9; j++) {
			transA[j][i] = A[i][j];
		}
	}

	setMultiply(AAt, A, transA);

	InverseMatrixGJ8(AAt, inverseAAt);

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 8; j++) {
			pseudoInverseA[i][j] = 0.0f;
			for (int k = 0; k < 8; k++) {
				pseudoInverseA[i][j] += transA[i][k] * inverseAAt[k][j];
			}
		}
	}

	for (int i = 0; i < 9; i++) {
		M[i/3][i%3] = 0.0f;
		for (int j = 0; j < 8; j++) {
			M[i/3][i%3] += pseudoInverseA[i][j]*0.0001f;
		}
	}

	printf("\nㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");

	for (int i = 0; i < 9; i++) {
		printf("%.2f ",M[i/3][i%3]);
	}
	printf("\nㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");

	float IM[3][3];
	InverseMatrixGJ3(M, IM);

	for (int y2 = 0; y2 < H; y2++) {
		for (int x2 = 0; x2 < W; x2++) {

			float x1 = IM[0][0] * float(x2)/(W-1) + IM[0][1] * float(y2)/(H-1) + IM[0][2];
			float y1 = IM[1][0] * float(x2)/(W-1) + IM[1][1] * float(y2)/(H-1) + IM[1][2];
			float w1 = IM[2][0] * float(x2)/(W-1) + IM[2][1] * float(y2)/(H-1) + IM[2][2];

			x1 /= w1;
			y1 /= w1;

			x1 *= W-1;
			x2 *= H-1;

			if (x1<0 || x1>W - 1) continue;
			if (y1<0 || y1>H - 1) continue;

			CvScalar f = cvGet2D(src, y1, x1);
			cvSet2D(dst, y2, x2, f);
		}
	}

}

void drawImage()									// 그림을 그린다 (각 면의 테두리를 직선으로 그림)
{
	cvSet(dst, cvScalar(0, 0, 0));
	for (int i = 0; i < 6; i++)
	{
		updatePosAndNormal(&cube[i], pos);
		if (cube[i].nor.z < 0) continue;			// 보이지 않는 사각형을 제외, 보이는 사각형만 그린다	
		for (int j = 0; j < 4; j++)
		{
			vec3 p1 = cube[i].pos[j];
			vec3 p2 = cube[i].pos[(j + 1) % 4];
			cvLine(dst, cvPoint(p1.x, p1.y), cvPoint(p2.x, p2.y), cvScalar(255, 255, 255), 3);

		}
		//Lena 이미지를 cube[i]의 네 꼭짓점(cube[i].pos[j])로 보내야한다.
		doHomography(src, dst, cube[i]);

	}
	//cvCircle(dst, cvPoint(cube[0].pos[0].x, cube[0].pos[0].y), 5, cvScalar(255,0,0), -1);
	//cvCircle(dst, cvPoint(cube[0].pos[1].x, cube[0].pos[1].y), 5, cvScalar(0,255,0), -1);
	//cvCircle(dst, cvPoint(cube[0].pos[2].x, cube[0].pos[2].y), 5, cvScalar(0,0,255), -1);
	//cvCircle(dst, cvPoint(cube[0].pos[3].x, cube[0].pos[3].y), 5, cvScalar(0,255,255), -1);
	cvShowImage("3D view", dst);
}

void myMouse(int event, int x, int y, int flags, void*)
{
	static CvPoint prev = cvPoint(0, 0);
	if (event == CV_EVENT_LBUTTONDOWN)
		prev = cvPoint(x, y);
	if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON) == CV_EVENT_FLAG_LBUTTON)
	{
		int dx = x - prev.x;
		int dy = y - prev.y;
		rotateModel(dy, dx, -dy);					// 마우스 조작에 따라 모델을 회전함
		drawImage();
		prev = cvPoint(x, y);
	}
}

int main()
{

	////이미지의 경로가 저장될 변수
	//char str[1000];
	//while (true) {
	//	//경로 입력받음
	//	printf("Input File Path: ");
	//	scanf("%s", str);
	//	src = cvLoadImage(str);

	//	//만약 경로가 잘못되었다면 반복문을 탈출하지 못함
	//	if (src != nullptr) {
	//		break;
	//	}
	//	printf("File not Found!\n");
	//}

	src = cvLoadImage("C:\\tmp\\lena.png");

	W = src->width;
	H = src->height;

	dst = cvCreateImage(cvSize(W, H), 8, 3);
	init();

	while (true)
	{
		rotateModel(0, 1, 0);
		drawImage();
		cvSetMouseCallback("3D view", myMouse);
		int key = cvWaitKey(1);
		if (key == ' ') key = cvWaitKey();
	}

	return 0;
}