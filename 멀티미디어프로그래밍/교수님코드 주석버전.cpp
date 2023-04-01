#include <opencv2/opencv.hpp>

//두 CvScalar값 사이의 차이를 리턴하는 함수
float getDistance(CvScalar f, CvScalar g);

int main() {
	//이미지 경로 입력
	//printf("Input File Name : ");
	//char str[100];
	//scanf("%s",str);

	//입력된 경로에 있는 이미지를 불러옴.
	//IplImage *src = cvLoadImage(str);

	IplImage* src = cvLoadImage("C:\\tmp\\pg1.jpg");

	//src의 크기를 저장하는 변수
	CvSize size = cvGetSize(src);


	//원래 이미지에서 3등분 된 각 이미지의 크기를 저장하는 변수
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

	//newSrc를 3등분하여 bImg, gImg, rImg에 저장함.
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

	//이미지 정렬용 변수
	//B이미지와 G이미지의 최소차이가 저장될 변수
	float minDiffBG = FLT_MAX;

	//B이미지와 R이미지의 최소차이가 저장될 변수
	float minDiffBR = FLT_MAX;

	//B이미지와 G이미지, 또는 B이미지와 R이미지가 최소차이일때의 u값과 v값이 저장될 변수
	int uBG, uBR, vBG, vBR;

	//u와 v를 -10부터 10까지 반복을 돌림.
	for (int v = -20; v <= 20; v++) {
		for (int u = -20; u <= 20; u++) {
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


//두 CvScalar변수의 값차이를 리턴하는 함수
//입력으로 들어오는 CvScalar 변수의 RGB값은 모두 같다고 가정한다.
float getDistance(CvScalar a, CvScalar b) {
	return abs(a.val[0] - b.val[0]);
}
