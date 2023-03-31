#include <opencv2/opencv.hpp>

//이미지를 입력으로 받아, y좌표가 startY에서 시작해서 dy만큼 증가하면서 이미지의 경계를 찾는 함수
int findBoundaryVertical(IplImage* img, int startY, int dy);

//두 CvScalar값 사이의 차이를 리턴하는 함수
float getDistance(CvScalar f, CvScalar g);

int main() {
	//이미지 경로 입력
	printf("Input File Name : ");
	char str[100];
	scanf("%s",str);

	//입력된 경로에 있는 이미지를 불러옴.
	IplImage *src = cvLoadImage(str);

	//src의 크기를 저장하는 변수
	CvSize size = cvGetSize(src);

	//src 이미지는 액자처럼 된 틀 안에 그려져 있는데, 그 틀의 위쪽 경계와 아래쪽 경계의 y좌표를 저장할 변수
	int topBoundary, bottomBoundary;
	//이미지의 위쪽부터 아래쪽으로 탐색하면서 처음 만나는 경계선의 y좌표를 리턴함.
	topBoundary = findBoundaryVertical(src, 2, 1);
	//이미지의 아래쪽부터 위쪽으로 탐색하면서 처음 만나는 경계선의 y좌표를 리턴함.
	bottomBoundary = findBoundaryVertical(src, size.height - 2, -1);

	//위쪽 경계선보다 위에 있는 부분과, 아래쪽 경계선보다 아래에 있는 부분은 필요없는 정보이므로 삭제해줄 것이다.
	
	//삭제 후, 새로운 이미지의 크기를 저장할 변수
	CvSize newSrcSize = size;

	//원래 이미지의 높이에 아래쪽 경계선의 y좌표를 저장한 뒤, 위쪽 경계선의 y좌표를 빼면 새로운 이미지의 높이가 나옴.
	newSrcSize.height = bottomBoundary;
	newSrcSize.height -= topBoundary;

	//원래 이미지에서 3등분 된 각 이미지의 크기를 저장하는 변수
	CvSize imgSize;
	imgSize.width = size.width;
	imgSize.height = newSrcSize.height / 3;

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

	//필요없는 정보가 삭제된 이미지가 저장될 newSrc
	IplImage* newSrc = cvCreateImage(newSrcSize, 8, 3);
	//newSrc에 위쪽 경계선부터 아래쪽 경계선까지의 이미지만 저장해준다.
	for (int y = topBoundary; y < bottomBoundary; y++) {
		for (int x = 0; x < newSrcSize.width; x++) {
			CvScalar color = cvGet2D(src, y, x);

			cvSet2D(newSrc, y - topBoundary, x, color);
		}
	}

	//newSrc를 3등분하여 bImg, gImg, rImg에 저장함.
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			CvScalar bc = cvGet2D(newSrc, y, x);
			CvScalar gc = cvGet2D(newSrc, y + newSrcSize.height / 3, x);
			CvScalar rc = cvGet2D(newSrc, y + newSrcSize.height / 3 * 2, x);

			cvSet2D(bImg, y, x, bc);
			cvSet2D(gImg, y, x, gc);
			cvSet2D(rImg, y, x, rc);
		}
	}

	//이미지 정렬용 변수
	//B이미지와 G이미지의 최소차이가 저장될 변수
	float minDiffBG = FLT_MAX;

	//B이미지와 R이미지의 최소차이가 저장될 변수
	float minDiffBR = FLT_MAX;

	//B이미지와 G이미지, 또는 B이미지와 R이미지가 최소차이일때의 u값과 v값이 저장될 변수
	int uBG, uBR, vBG, vBR;

	//u와 v를 -10부터 10까지 반복을 돌림.
	for (int v = -10; v <= 10; v++) {
		for (int u = -10; u <= 10; u++) {
			//이미지의 각 픽셀에서의 차이를 모두 더하여 저장할 변수
			float sumBG = 0.0;
			float sumBR = 0.0;
			//연산 횟수가 저장될 변수
			int cnt = 0;
			//y좌표는 h/4부터 h/4*3까지, x좌표는 w/4부터 w/4*3까지 반복
			for (int y = h / 4; y < h / 4 * 3; y++) {
				for (int x = w / 4; x < w / 4 * 3; x++) {
					//만약 연산을 진행하는 곳이 이미지 밖이라면 연산을 하지 않음
					if (x + u<0 || x + u>w - 1) continue;
					if (y + v<0 || y + v>h - 1) continue;

					//getDistance함수를 이용하여 bImg에서의 (x, y)에서의 픽셀값과 gImg에서의 (x+u, y+v)에서의 픽셀값의 차이를 구함.
					sumBG += getDistance(cvGet2D(bImg, y, x), cvGet2D(gImg, y + v, x + u));
					sumBR += getDistance(cvGet2D(bImg, y, x), cvGet2D(rImg, y + v, x + u));
					//연산횟수 1증가
					cnt++;
				}
			}
			//만약 차이의 합이 최소 차이보다 작다면
			if (sumBG / cnt < minDiffBG) {
				//그 값을 최소차이 변수에 저장함.
				minDiffBG = sumBG / cnt;
				//최소차이일때의 u,v값 저장.
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

	//dst에 값 저장하기
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			//만약 현재 픽셀의 좌표가 이미지 밖이라면 값을 저장하지 않음.
			if (x + uBG<0 || x + uBG>w - 1) continue;
			if (y + vBG<0 || y + vBG>h - 1) continue;
			if (x + uBR<0 || x + uBR>w - 1) continue;
			if (y + vBR<0 || y + vBR>h - 1) continue;

			//bImg에서는 (x,y)위치의 픽셀값을 얻어옴
			CvScalar b = cvGet2D(bImg, y, x);
			
			//gImg에서는 (x+uBG, y+vBG)위치의 픽셀값을 얻어옴
			CvScalar g = cvGet2D(gImg, y + vBG, x + uBG);

			//rImg에서는 (x+uBR, y+vBR)위치의 픽셀값을 얻어옴
			CvScalar r = cvGet2D(rImg, y + vBR, x + uBR);

			//dst에 저장될 CvScalar값
			CvScalar color;

			//CvScalar값의 BGR값들을 초기화함.
			color.val[0] = b.val[0];
			color.val[1] = g.val[0];
			color.val[2] = r.val[0];

			//그 값을 dst에 저장.
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
	for (int y = startY;0 < y && y < cvGetSize(img).height - 1 ; y += dy) {
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
