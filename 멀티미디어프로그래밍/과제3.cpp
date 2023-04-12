#include <opencv2/opencv.hpp>
void myFastMeanFilter(IplImage *src, IplImage* dst, int k);

int main() {
	//경로 입력받을 문자열
	char str[1000];
	//이미지가 저장될 변수
	IplImage* src;
	while (1) {
		//경로 입력받음
		printf("Input File Path:");
		scanf("%s", str);
		src = cvLoadImage(str);
		//만약 잘못된 경로를 입력하지 않았다면 반복문 탈출
		if (src != nullptr) {
			break;
		}
		//반목문을 탈출하지 못했다면 오류메시지 출력하고 다시 입력받음
		printf("File Not Found!\n");
	}
	
	//이미지의 사이즈 저장
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);

	//커널 사이즈 입력받음
	int k;
	printf("Input Kernel Size:");
	scanf("%d",&k);

	//meanFiltering수행
	myFastMeanFilter(src, dst, k);

	//이미지 띄움
	cvShowImage("src", src);
	cvShowImage("dst", dst);
	cvWaitKey();

	return 0;
}

void myFastMeanFilter(IplImage* src, IplImage* dst, int k)
{
	//이미지의 가로, 세로 길이 저장해놓음
	CvSize size = cvGetSize(src);
	int h = size.height;
	int w = size.width;

	//이미지의 CvScalar값이 저장될 CvScalar형 2차원 배열
	//이미지의 (x, y)위치에서의 CvScalar값은 srcArr[y][x]에 저장되어 있음
	CvScalar** srcArr = (CvScalar**)malloc(sizeof(CvScalar*) * h);
	for (int y = 0; y < h; y++) {
		srcArr[y] = (CvScalar*)malloc(sizeof(CvScalar) * w);
	}
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			srcArr[y][x] = cvGet2D(src, y, x);
		}
	}

	//srcArr에 대한 Summed-Area-Table 정의
	CvScalar** srcSAT = (CvScalar**)malloc(sizeof(CvScalar*) * h);
	for (int y = 0; y < h; y++) {
		srcSAT[y] = (CvScalar*)malloc(sizeof(CvScalar) * w);
	}

	//두 변수 x, y에 대해 Arr과 SAT는 아래 식을 만족함. (단, x<w, y<h)
	//i) x=0, y=0 일 때
	//SAT[0][0] = Arr[0][0]
	srcSAT[0][0] = srcArr[0][0];
	for (int k = 0; k < 3; k++) {
		//ii) x>0, y=0 일 때
		for (int x = 1; x < w; x++) {
			//SAT[0][x] = SAT[0][x-1] + Arr[0][x]
			srcSAT[0][x].val[k] = srcSAT[0][x - 1].val[k] + srcArr[0][x].val[k];
		}
		//iii) x=0, y>0 일 때
		for (int y = 1; y < h; y++) {
			//SAT[y][0] = SAT[y-1][0] + Arr[y][0]
			srcSAT[y][0].val[k] = srcSAT[y - 1][0].val[k] + srcArr[y][0].val[k];
		}
		//iV) x>0, y>0 일 때
		for (int y = 1; y < h; y++) {
			for (int x = 1; x < w; x++) {
				//SAT[y][x] = SAT[y-1][x] + SAT[y][x-1] - SAT[y-1][x-1] + Arr[y][x]
				srcSAT[y][x].val[k] = srcSAT[y - 1][x].val[k] + srcSAT[y][x - 1].val[k] - srcSAT[y - 1][x - 1].val[k] + srcArr[y][x].val[k];
			}
		}
	}

	//mean 필터의 한 변의 길이 정의
	int ksize = 2 * k + 1;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			//dst에 저장할 CvScalar형 변수 g
			CvScalar g=cvScalar(0,0,0);
			for (int d = 0; d < 3; d++) {
				//kw : 필터의 가로길이(너비)
				//kh : 필터의 세로길이(높이)
				int kw = ksize, kh = ksize;

				//필터는 중심이 (x, y)일때 왼쪽 위의 좌표가 (x-k, y-k), 오른쪽 아래의 좌표가 (x+k, y+k)이다.
				//만약 이 좌표가 이미지의 밖이라면 예외처리를 해주어야 한다.

				//먼저 필터의 오른쪽과 아래쪽이 이미지 밖으로 나가진 않았는지 검사해준다.
				
				//필터의 오른쪽 아래의 x좌표와 y좌표를 저장할 변수 xk와 yk
				//따라서, xk와 yk역시 이미지 안에 있어야 한다.
				int xk = x + k, yk = y + k;

				//만약 필터의 오른쪽 부분이 이미지 밖이라면
				if (x + k >= w) {
					//필터의 가로 길이는 이미지 밖에 삐져나온만큼 줄어들게 된다.
					kw -= xk - (w - 1);
					//필터의 오른쪽의 좌표는 곧 이미지의 마지막 x좌표가 된다.
					xk = w - 1;
				}
				//만약 필터의 아랫 부분이 이미지 밖이라면
				if (y + k >= h) {
					//필터의 새로길이는 이미지 밖에 삐져나온만큼 줄어들게 된다.
					kh -= yk - (h - 1);
					//필터의 아래쪽의 좌표는 곧 이미지의 마지막 y좌표가 된다.
					yk = h - 1;
				}
				//필터의 오른쪽 아래 좌표에서의 SAT을 g에 더해준다.
				g.val[d] += srcSAT[yk][xk].val[d];

				//이번엔, 필터의 왼쪽과 윗쪽이 밖으로 나가진 않았는지 검사해준다.

				//만약 필터의 왼쪽 테두리가 이미지의 왼쪽 테두리보다 오른쪽에 있다면 
				if (x - k - 1 >= 0) {
					//Arr[0][0]부터 필터의 왼쪽좌표까지의 값들이 불필요하게 더해졌다는 뜻이다.
					//이 값들을 g에서 빼준다.
					g.val[d] -= srcSAT[yk][x - k - 1].val[d];
				}
				//아니라면
				else {
					//불필요한 값이 더해지지 않았으므로 값을 빼줄 필요가 없다.
					//다만 필터의 가로길이가 줄어들 것이므로 이 길이만 조정해준다.
					kw -= k - x;
				}
				//만약 필터의 위쪽 테두리가 이미지의 위쪽 테두리보다 아래쪽에 있다면 
				if (y - k - 1 >= 0) {
					//Arr[0][0]부터 필터의 위쪽좌표까지의 값들이 불필요하게 더해졌다는 뜻이다.
					//이 값들을 g에서 빼준다.
					g.val[d] -= srcSAT[y - k - 1][xk].val[d];
				}
				//아니라면
				else {
					//불필요한 값이 더해지지 않았으므로 값을 빼줄 필요가 없다.
					//다만 필터의 세로길이가 줄어들 것이므로 이 길이만 조정해준다.
					kh -= k - y;
				}
				//만약 필터가 이미지 안쪽에 정상적으로 위치해 있었다면, 위의 연산에 의해 두번 뺀 값이 존재한다.
				if (x - k - 1 >= 0 && y - k - 1 >= 0) {
					//그 값을 다시 더해준다.
					g.val[d] += srcSAT[y - k - 1][x - k - 1].val[d];
				}
				//g의 값을 필터의 크기만큼 나눠준다.
				g.val[d] /= (kw * kh);
			}
			cvSet2D(dst, y, x, g);
		}
	}

	//할당 받은 2차원 배열들을 메모리 해제 해준다.
	for (int y = 0; y < h; y++) {
		free(srcArr[y]);
		free(srcSAT[y]);
	}
	free(srcArr);
	free(srcSAT);
}
