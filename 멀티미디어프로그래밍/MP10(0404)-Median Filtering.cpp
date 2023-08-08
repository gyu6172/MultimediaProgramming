#include <opencv2/opencv.hpp>
int compare(CvScalar a, CvScalar b) 
//0 : 같다
//양수 : a>b
//음수 : a<b
{
	int a_bri = a.val[0]+ a.val[1] + a.val[2];
	int b_bri = b.val[0]+ b.val[1] + b.val[2];
	return a_bri-b_bri;
}
int main() {
	IplImage *src = cvLoadImage("C:\\tmp\\saltpepper.jpg");
	CvSize size = cvGetSize(src);
	IplImage *dst = cvCreateImage(size, 8, 3);

	int nK = 1;
	int total = (2 * nK + 1) * (2 * nK + 1);

	//CV_BLUR		: mean filter
	//CV_GAUSSIAN	: gaussian filter
	//CV_MEDIAN		: median filter
	//cvSmooth(src, dst, CV_MEDIAN, 2*nK+1);

	//CvScalar *f = (CvScalar*)malloc(sizeof(CvScalar)*total);	//<-C언어 문법(비추)
	CvScalar * f = new CvScalar[total];

	for (int y = nK; y < size.height-nK; y++) {
		for (int x = nK; x < size.width-nK; x++) {
			int count=0;
			for (int v = -nK; v <= nK; v++) {
				for (int u = -nK; u <= nK; u++) {
					f[count] = cvGet2D(src, y+v, x+u);
					count++;
				}
			}
			//sorting(정렬)
			//f[0]이 가장 밝다고 가정
			for (int i = 0; i < total-1; i++) {
				for (int j = i + 1; j < total; j++) {
					if (compare(f[i], f[j]) < 0) {
						//swap f[0] and f[i]
						CvScalar tmp = f[i];
						f[i] = f[j];
						f[j] = tmp;
					}
				}
			}
			
			CvScalar g=f[total/2];
			cvSet2D(dst, y, x, g);
		}
	}
	//free(f);
	delete[] f;


	cvShowImage("src",src);
	cvShowImage("dst",dst);
	cvWaitKey();

	return 0;
}