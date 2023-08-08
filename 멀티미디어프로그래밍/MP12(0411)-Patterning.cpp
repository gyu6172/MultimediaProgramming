#include <opencv2/opencv.hpp>
int main() {


	IplImage* src = cvLoadImage("C:\\tmp\\세종대학교.jpg");
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);

	int pat[9] = { 1,8,0,6,2,5,7,3,4 };
	//int pat[4] = {2,1,3,0};
	int numPat = 9;

	cvSet(dst, cvScalar(0, 0, 0));

	for (int y = 0; y < size.height - 2; y += 3) {
		for (int x = 0; x < size.width - 2; x += 3) {
			CvScalar f = cvGet2D(src, y, x);
			for (int k = 0; k < 3; k++) {

				//int bri = f.val[k];
				//
				//float div = 255.0f / (numPat+1);
				//
				//int lev = bri / div;	//level개의 흰색 점을 칠해야 한다.

				int lev = (f.val[k]/255.0f)*(numPat+1);

				//int(0~1) : 0
				//int(1~2) : 1
				//int(2~3) : 2
				//int(3~4) : 3
				//int(4~5) : 4
				//0<=f.val[k]*(numPat+1)/255<=numPat+1
				//lev = (f.val[k]*(numPat+1))/255
				
				//딱 numPat+1이 되면 오버플로우. 그 현상을 방지하기 위한 if문
				if (lev == numPat+1) lev = numPat;

				//lev개만큼 점을 칠해야한다.
				for (int i = 0; i < lev; i++) {
					int x2 = x + pat[i] % 3;
					int y2 = y + pat[i] / 3;
					CvScalar g = cvGet2D(dst, y2, x2);
					g.val[k] = 255;

					cvSet2D(dst, y2, x2, g);
					
				}
			}
		}
	}

	cvShowImage("dst", dst);
	cvShowImage("src", src);
	cvWaitKey();

	return 0;
}