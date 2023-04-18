#include <opencv2/opencv.hpp>
int main() {


	IplImage* src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);
	IplImage* blur = cvCreateImage(size, 8, 3);

	int pat[9] = { 1,8,0,6,2,5,7,3,4 };
	//int pat[4] = {2,1,3,0};
	int numPat = 9;

	cvSet(dst, cvScalar(0, 0, 0));
	cvSmooth(src, blur, CV_BLUR, 5);

	for (int y = 0; y < size.height - 2; y += 3) {
		for (int x = 0; x < size.width - 2; x += 3) {
			CvScalar f = cvGet2D(src, y, x);
			for (int k = 0; k < 3; k++) {

				int bri = f.val[k];

				float div = 255.0f / (numPat+1);

				int lev = bri / div;	//level���� ��� ���� ĥ�ؾ� �Ѵ�.
				//int(0~1) : 0
				//int(1~2) : 1
				//int(2~3) : 2
				//int(3~4) : 3
				//int(4~5) : 4
				//0<=f.val[k]*(numPat+1)/255<=numPat+1
				//lev = (f.val[k]*(numPat+1))/255
				
				//�� numPat+1�� �Ǹ� �����÷ο�. �� ������ �����ϱ� ���� if��
				if (lev > numPat) lev = numPat;

				//lev����ŭ ���� ĥ�ؾ��Ѵ�.
				for (int i = 0; i < lev; i++) {
					int x2 = x + pat[i] % 3;
					int y2 = y + pat[i] / 3;
					CvScalar g = cvGet2D(dst, y2, x2);
					g.val[k] = 255;

					cvSet2D(dst, y2, x2, g);
					cvShowImage("dst", dst);
					cvWaitKey();
				}
			}
		}
	}


	cvShowImage("src", src);

	cvShowImage("blur", blur);
	cvWaitKey();

	return 0;
}