#include <opencv2/opencv.hpp>

//이미지를 입력으로 받아, y좌표가 startY에서 시작해서 dy만큼 증가하면서 이미지의 경계를 찾는 함수
int findBoundaryVertical(IplImage* img, int startY, int dy);

//두 CvScalar값 사이의 차이를 리턴하는 함수
float getDistance(CvScalar f, CvScalar g);

int main() {
	//이미지 경로 입력
	printf("Input File Name : ");
	char str[100];
	scanf("%s", str);

	//입력된 경로에 있는 이미지를 불러옴.
	IplImage* src = cvLoadImage(str);

	//src의 크기를 저장하는 변수
	CvSize size = cvGetSize(src);

	CvSize imgSize;
	imgSize.width = size.width;
	imgSize.height = size.height / 3;

	int h = imgSize.height;
	int w = imgSize.width;

	//B색상의 정보가 포함된 이미지 bImg 선언
	IplImage* bImg = cvCreateImage(imgSize, 8, 3);

	//G색상의 정보가 포함된 이미지 gImg 선언
	IplImage* gImg = cvCreateImage(imgSize, 8, 3);

	//R색상의 정보가 포함된 이미지 rImg 선언
	IplImage* rImg = cvCreateImage(imgSize, 8, 3);

	//최종 출력할 결과 이미지 dst 선언
	IplImage* dst = cvCreateImage(imgSize, 8, 3);

	//src를 3등분하여 bImg, gImg, rImg에 저장함.
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			CvScalar bc = cvGet2D(src, y, x);
			CvScalar gc = cvGet2D(src, y + size.height / 3, x);
			CvScalar rc = cvGet2D(src, y + size.height / 3 * 2, x);

			cvSet2D(bImg, y, x, bc);
			cvSet2D(gImg, y, x, gc);
			cvSet2D(rImg, y, x, rc);
		}
	}

	//이미지의 정보가 담긴 2차원배열을 3개 생성(동적할당)
	float** bImgArr = (float**)malloc(sizeof(float*) * h);
	float** gImgArr = (float**)malloc(sizeof(float*) * h);
	float** rImgArr = (float**)malloc(sizeof(float*) * h);
	for (int i = 0; i < h; i++) {
		bImgArr[i] = (float*)malloc(sizeof(float) * w);
		gImgArr[i] = (float*)malloc(sizeof(float) * w);
		rImgArr[i] = (float*)malloc(sizeof(float) * w);
	}

	//이미지의 정보들을 2차원 배열에 저장.
	//이미지에서 어떤 좌표의 픽셀값은 같은 위치에서의 2차원 배열에 저장된 값과 동일함.
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			bImgArr[i][j] = (cvGet2D(bImg, i, j).val[0]);
			gImgArr[i][j] = (cvGet2D(gImg, i, j).val[0]);
			rImgArr[i][j] = (cvGet2D(rImg, i, j).val[0]);
		}
	}

	//B이미지를 기준으로 G이미지와 R이미지가 각각 얼마나 떨어져 있는지 구할 것이다.
	//B이미지와 G이미지, B이미지와 R이미지의 최소차이가 저장될 변수
	float minDiffBG = FLT_MAX;
	float minDiffBR = FLT_MAX;

	//최소차이일 때, u, v값
	int uBG, uBR, vBG, vBR;

	//이미지를 위치가 맞게 정렬
	//u와 v를 -20~20까지 돌리면서 밝기값의 차이가 가장 적을 때를 찾는다.
	for (int v = -30; v <= 30; v++) {
		for (int u = -30; u <= 30; u++) {
			//밝기 차이의 합
			float sumBG = 0.0;
			float sumBR = 0.0;
			//연산횟수
			int cnt = 0;

			for (int y = h / 4; y < h / 4 * 3; y++) {
				for (int x = w / 4; x < w / 4 * 3; x++) {
					//연산을 진행하는 곳이 이미지 밖이라면 연산을 진행하지 않는다.
					if (x + u<0 || x + u>imgSize.width - 1) continue;
					if (y + v<0 || y + v>imgSize.height - 1) continue;
					//밝기 차이를 더해준다.
					sumBG += abs(bImgArr[y][x] - gImgArr[y + v][x + u]);
					sumBR += abs(bImgArr[y][x] - rImgArr[y + v][x + u]);
					//연산횟수 1증가
					cnt++;
				}
			}
			//밝기 차이의 합을 연산횟수로 나누면 밝기차이의 평균을 구할 수 있다.
			//만약 그 값이 최소차이보다 작다면
			if (sumBG / cnt < minDiffBG) {
				//최소차이에 그 값을 저장.
				minDiffBG = sumBG / cnt;
				//u, v값 저장
				uBG = u;
				vBG = v;
			}
			if (sumBR / cnt < minDiffBR) {
				minDiffBR = sumBR / cnt;
				uBR = u;
				vBR = v;
			}
		}
	}

	//dst에 띄우기
	for (int y = 0; y < imgSize.height; y++) {
		for (int x = 0; x < imgSize.width; x++) {
			//픽셀의 좌표가 이미지 밖이라면 연산을 진행하지 않는다.
			if (x + uBG<0 || x + uBG>imgSize.width - 1) continue;
			if (y + vBG<0 || y + vBG>imgSize.height - 1) continue;
			if (x + uBR<0 || x + uBR>imgSize.width - 1) continue;
			if (y + vBR<0 || y + vBR>imgSize.height - 1) continue;
			CvScalar b = cvGet2D(bImg, y, x);
			CvScalar g = cvGet2D(gImg, y + vBG, x + uBG);
			CvScalar r = cvGet2D(rImg, y + vBR, x + uBR);
			CvScalar color;
			color.val[0] = b.val[0];
			color.val[1] = g.val[0];
			color.val[2] = r.val[0];
			cvSet2D(dst, y, x, color);
		}
	}

	//src와 dst이미지 보여주기
	cvShowImage("src", src);
	cvShowImage("dst", dst);


	cvWaitKey();

	return 0;
}

//img에서의 경계를 찾는 함수.
//y좌표가 startY일때 그 행의 밝기값을 모두 더하고, y를 dy만큼 더해 같은 연산을 진행함.
//그 차이가 특정 상수값 이상이면 이미지에서 경계선이 될 것이다.
//dy가 양수일 경우 위에서 아래로 탐색, 음수일 경우 아래에서 위로 탐색을 진행
int findBoundaryVertical(IplImage* img, int startY, int dy)
{
	//y=startY로 초기화, y좌표가 이미지 안에 있을 때, y에 dy만큼 더함.
	for (int y = startY; 0 < y && y < cvGetSize(img).height - 1; y += dy) {
		//y좌표가 y인 행의 밝기값의 평균
		double bright = 0.0;
		//y좌표가 y+dy인 행의 밝기값의 평균
		double nextBright = 0.0;

		//이미지의 왼쪽에서 오른쪽까지 밝기값을 모두 더하여 저장
		for (int x = 0; x < cvGetSize(img).width; x++) {
			bright += cvGet2D(img, y, x).val[0];
			nextBright += cvGet2D(img, y + dy, x).val[0];
		}

		//평균을 내기 위해 각 변수의 값을 열의 개수로 나눠줌.
		bright /= cvGetSize(img).width - 1;
		nextBright /= cvGetSize(img).width - 1;

		//만약 차이가 50보다 크다면
		if (abs(bright - nextBright) > 50) {
			//현재 행의 y좌표를 리턴.
			return y;
		}
	}
}

//두 CvScalar변수의 값차이를 리턴하는 함수
//입력으로 들어오는 CvScalar 변수의 RGB값은 모두 같다고 가정한다.
float getDistance(CvScalar a, CvScalar b) {
	return abs(a.val[0] - b.val[0]);
}
