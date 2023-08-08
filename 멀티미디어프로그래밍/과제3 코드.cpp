#include <opencv2/opencv.hpp>
void myFastMeanFilter(IplImage *src, IplImage* dst, int k);

int main() {
	//��� �Է¹��� ���ڿ�
	char str[1000];
	//�̹����� ����� ����
	IplImage* src;
	while (1) {
		//��� �Է¹���
		printf("Input File Path:");
		scanf("%s", str);
		src = cvLoadImage(str);
		//���� �߸��� ��θ� �Է����� �ʾҴٸ� �ݺ��� Ż��
		if (src != nullptr) {
			break;
		}
		//�ݸ��� Ż������ ���ߴٸ� �����޽��� ����ϰ� �ٽ� �Է¹���
		printf("File Not Found!\n");
	}
	
	//�̹����� ������ ����
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);

	//Ŀ�� ������ �Է¹���
	int k;
	printf("Input Kernel Size:");
	scanf("%d",&k);

	//meanFiltering����
	myFastMeanFilter(src, dst, k);

	//�̹��� ���
	cvShowImage("src", src);
	cvShowImage("dst", dst);
	cvWaitKey();

	return 0;
}

void myFastMeanFilter(IplImage* src, IplImage* dst, int k)
{
	//�̹����� ����, ���� ���� �����س���
	CvSize size = cvGetSize(src);
	int h = size.height;
	int w = size.width;

	//�̹����� CvScalar���� ����� CvScalar�� 2���� �迭
	//�̹����� (x, y)��ġ������ CvScalar���� srcArr[y][x]�� ����Ǿ� ����
	CvScalar** srcArr = (CvScalar**)malloc(sizeof(CvScalar*) * h);
	for (int y = 0; y < h; y++) {
		srcArr[y] = (CvScalar*)malloc(sizeof(CvScalar) * w);
	}
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			srcArr[y][x] = cvGet2D(src, y, x);
		}
	}

	//srcArr�� ���� Summed-Area-Table ����
	CvScalar** srcSAT = (CvScalar**)malloc(sizeof(CvScalar*) * h);
	for (int y = 0; y < h; y++) {
		srcSAT[y] = (CvScalar*)malloc(sizeof(CvScalar) * w);
	}

	//�� ���� x, y�� ���� Arr�� SAT�� �Ʒ� ���� ������. (��, x<w, y<h)
	//i) x=0, y=0 �� ��
	//SAT[0][0] = Arr[0][0]
	srcSAT[0][0] = srcArr[0][0];
	for (int k = 0; k < 3; k++) {
		//ii) x>0, y=0 �� ��
		for (int x = 1; x < w; x++) {
			//SAT[0][x] = SAT[0][x-1] + Arr[0][x]
			srcSAT[0][x].val[k] = srcSAT[0][x - 1].val[k] + srcArr[0][x].val[k];
		}
		//iii) x=0, y>0 �� ��
		for (int y = 1; y < h; y++) {
			//SAT[y][0] = SAT[y-1][0] + Arr[y][0]
			srcSAT[y][0].val[k] = srcSAT[y - 1][0].val[k] + srcArr[y][0].val[k];
		}
		//iV) x>0, y>0 �� ��
		for (int y = 1; y < h; y++) {
			for (int x = 1; x < w; x++) {
				//SAT[y][x] = SAT[y-1][x] + SAT[y][x-1] - SAT[y-1][x-1] + Arr[y][x]
				srcSAT[y][x].val[k] = srcSAT[y - 1][x].val[k] + srcSAT[y][x - 1].val[k] - srcSAT[y - 1][x - 1].val[k] + srcArr[y][x].val[k];
			}
		}
	}

	//mean ������ �� ���� ���� ����
	int ksize = 2 * k + 1;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			//dst�� ������ CvScalar�� ���� g
			CvScalar g=cvScalar(0,0,0);
			for (int d = 0; d < 3; d++) {
				//kw : ������ ���α���(�ʺ�)
				//kh : ������ ���α���(����)
				int kw = ksize, kh = ksize;

				//���ʹ� �߽��� (x, y)�϶� ���� ���� ��ǥ�� (x-k, y-k), ������ �Ʒ��� ��ǥ�� (x+k, y+k)�̴�.
				//���� �� ��ǥ�� �̹����� ���̶�� ����ó���� ���־�� �Ѵ�.

				//���� ������ �����ʰ� �Ʒ����� �̹��� ������ ������ �ʾҴ��� �˻����ش�.
				
				//������ ������ �Ʒ��� x��ǥ�� y��ǥ�� ������ ���� xk�� yk
				//����, xk�� yk���� �̹��� �ȿ� �־�� �Ѵ�.
				int xk = x + k, yk = y + k;

				//���� ������ ������ �κ��� �̹��� ���̶��
				if (x + k >= w) {
					//������ ���� ���̴� �̹��� �ۿ� �������¸�ŭ �پ��� �ȴ�.
					kw -= xk - (w - 1);
					//������ �������� ��ǥ�� �� �̹����� ������ x��ǥ�� �ȴ�.
					xk = w - 1;
				}
				//���� ������ �Ʒ� �κ��� �̹��� ���̶��
				if (y + k >= h) {
					//������ ���α��̴� �̹��� �ۿ� �������¸�ŭ �پ��� �ȴ�.
					kh -= yk - (h - 1);
					//������ �Ʒ����� ��ǥ�� �� �̹����� ������ y��ǥ�� �ȴ�.
					yk = h - 1;
				}
				//������ ������ �Ʒ� ��ǥ������ SAT�� g�� �����ش�.
				g.val[d] += srcSAT[yk][xk].val[d];

				//�̹���, ������ ���ʰ� ������ ������ ������ �ʾҴ��� �˻����ش�.

				//���� ������ ���� �׵θ��� �̹����� ���� �׵θ����� �����ʿ� �ִٸ� 
				if (x - k - 1 >= 0) {
					//Arr[0][0]���� ������ ������ǥ������ ������ ���ʿ��ϰ� �������ٴ� ���̴�.
					//�� ������ g���� ���ش�.
					g.val[d] -= srcSAT[yk][x - k - 1].val[d];
				}
				//�ƴ϶��
				else {
					//���ʿ��� ���� �������� �ʾ����Ƿ� ���� ���� �ʿ䰡 ����.
					//�ٸ� ������ ���α��̰� �پ�� ���̹Ƿ� �� ���̸� �������ش�.
					kw -= k - x;
				}
				//���� ������ ���� �׵θ��� �̹����� ���� �׵θ����� �Ʒ��ʿ� �ִٸ� 
				if (y - k - 1 >= 0) {
					//Arr[0][0]���� ������ ������ǥ������ ������ ���ʿ��ϰ� �������ٴ� ���̴�.
					//�� ������ g���� ���ش�.
					g.val[d] -= srcSAT[y - k - 1][xk].val[d];
				}
				//�ƴ϶��
				else {
					//���ʿ��� ���� �������� �ʾ����Ƿ� ���� ���� �ʿ䰡 ����.
					//�ٸ� ������ ���α��̰� �پ�� ���̹Ƿ� �� ���̸� �������ش�.
					kh -= k - y;
				}
				//���� ���Ͱ� �̹��� ���ʿ� ���������� ��ġ�� �־��ٸ�, ���� ���꿡 ���� �ι� �� ���� �����Ѵ�.
				if (x - k - 1 >= 0 && y - k - 1 >= 0) {
					//�� ���� �ٽ� �����ش�.
					g.val[d] += srcSAT[y - k - 1][x - k - 1].val[d];
				}
				//g�� ���� ������ ũ�⸸ŭ �����ش�.
				g.val[d] /= (kw * kh);
			}
			cvSet2D(dst, y, x, g);
		}
	}

	//�Ҵ� ���� 2���� �迭���� �޸� ���� ���ش�.
	for (int y = 0; y < h; y++) {
		free(srcArr[y]);
		free(srcSAT[y]);
	}
	free(srcArr);
	free(srcSAT);
}
