#include <opencv2/opencv.hpp>

int main() {

	//이미지 경로 입력
	printf("Input File Name : ");
	char str[100];
	scanf("%s",str);

	//입력된 경로에 있는 이미지를 불러옴.
	IplImage *src = cvLoadImage(str);

	//src의 크기를 저장하는 변수
	CvSize size = cvGetSize(src);


	//원래 이미지에서 3등분 된 각 이미지의 크기를 저장하는 변수
	CvSize imgSize;
	imgSize.width = size.width;
	imgSize.height = size.height / 3;

	//h에 이미지의 높이저장, w에 이미지의 너비저장
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

	//템플릿의 사이즈는 원래 이미지의 1/3
	CvSize templSize;
	templSize.height = h / 3;
	templSize.width = w / 3;

	//템플릿 이미지 생성
	IplImage* templ = cvCreateImage(templSize, 8, 3);

	//bImg에서 템플릿이미지를 뽑아낸다.
	for (int y = 0; y < templSize.height; y++) {
		for (int x = 0; x < templSize.width; x++) {
			cvSet2D(templ, y, x, cvGet2D(bImg, y + templSize.height, x + templSize.width));
		}
	}

	//연산 결과를 저장하는 이미지 생성
	//이미지의 높이,너비는 (원래이미지의 높이,너비) - (템플릿이미지의 높이,너비) + 1
	IplImage* resultG = cvCreateImage(cvSize(w - templSize.width + 1, h - templSize.height + 1), IPL_DEPTH_32F, 1);
	IplImage* resultR = cvCreateImage(cvSize(w - templSize.width + 1, h - templSize.height + 1), IPL_DEPTH_32F, 1);

	//MatchTemplate 연산 수행.
	//cvMatchTemplate(원본이미지, 템플릿이미지, 결과이미지, 연산 방법);
	cvMatchTemplate(gImg, templ, resultG, CV_TM_CCOEFF_NORMED);
	cvMatchTemplate(rImg, templ, resultR, CV_TM_CCOEFF_NORMED);

	//결과 이미지에서 최고정확도를 계산하여 저장할 변수
	double maxg, maxr;

	//정확도가 최고일때의 사각형 왼쪽 위 좌표를 저장할 변수
	//각각 bImg, gImg, rImg의 좌표를 저장함.
	CvPoint pb, pg, pr;
	
	//bImg가 원본이므로 템플릿이미지가 원본에서 어디있는지 저장함.
	pb.x = templSize.width;
	pb.y = templSize.height;

	//결과 이미지에서 정확도가 가장 높을 때의 사각형 왼쪽 위 좌표를 저장함.
	cvMinMaxLoc(resultG, NULL, &maxg, NULL, &pg);
	cvMinMaxLoc(resultR, NULL, &maxg, NULL, &pr);

	//bImg와 gImg의 좌표의 차이, bImg와 rImg의 좌표의 차이를 저장할 변수
	CvPoint diffBG, diffBR;
	diffBG.x = pg.x - pb.x;
	diffBG.y = pg.y - pb.y;
	diffBR.x = pr.x - pb.x;
	diffBR.y = pr.y - pb.y;
	//bImg의 (x, y)위치에 있는 점은 gImg의 (x+diffBG.x, y+diffBG.y)위치에 있는 점과 대응된다.
	//bImg의 (x, y)위치에 있는 점은 rImg의 (x+diffBR.x, y+diffBR.y)위치에 있는 점과 대응된다.

	//dst에 그리기
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			//만약 좌표가 이미지 밖이라면 연산을 진행하지 않음.
			if(0>x+diffBG.x || x+diffBG.x>w-1) continue;
			if(0>x+diffBR.x || x+diffBR.x>w-1) continue;
			if(0>y+diffBG.y || y+diffBG.y>h-1) continue;
			if(0>y+diffBR.y || y+diffBR.y>h-1) continue;

			//각 점에서의 픽셀값을 뽑아냄.
			CvScalar b = cvGet2D(bImg, y, x);
			CvScalar g = cvGet2D(gImg, y+diffBG.y, x+diffBG.x);
			CvScalar r = cvGet2D(rImg, y+diffBR.y, x+diffBR.x);

			//dst에 BGR값을 대입하여 저장.
			CvScalar color;
			color.val[0] = b.val[0];
			color.val[1] = g.val[0];
			color.val[2] = r.val[0];
			cvSet2D(dst, y, x, color);
		}
	}

	//src와 dst 띄우기
	cvShowImage("src", src);
	cvShowImage("dst", dst);

	cvWaitKey();

	return 0;
}

