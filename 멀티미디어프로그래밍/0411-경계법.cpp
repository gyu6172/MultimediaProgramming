#include <opencv2/opencv.hpp>

float getDiff(CvScalar a, CvScalar b) {
	float sum=0;
	for (int k = 0; k < 3; k++) {
		sum += (a.val[k] - b.val[k])* (a.val[k] - b.val[k]);
	}
	return sum;
}

int main() {
	//Thresholding(경계법) : 특정 밝기 이상이면 흰색, 아니면 검은색
	//Patterning(패턴법)
	//Dithering(디더링법)

	IplImage *src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);
	IplImage *dst = cvCreateImage(size, 8, 3);

	CvScalar pal[] = {	cvScalar(0,0,0), 
						cvScalar(255,255,255), 
						//cvScalar(128,128,128),
						//cvScalar(50,50,50),
						//cvScalar(100,100,100),
						//cvScalar(200,200,200),
						cvScalar(0,0,255),
						cvScalar(0,255,0),
						cvScalar(255,0,0),
						cvScalar(255,255,0),
						cvScalar(0,255,255),
						cvScalar(255,0,255),
					};
	int numPal = 9;
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvGet2D(src,y,x);

			int min_ind = 0;
			float min_diff = FLT_MAX;
			for (int i = 0; i < numPal; i++) {
				float diff = getDiff(pal[i], f);
				if (diff < min_diff) {
					min_diff = diff;
					min_ind = i;
				}
			}
			

			CvScalar g = pal[min_ind];
			cvSet2D(dst, y, x, g);
		}
	}


	cvShowImage("src",src);
	cvShowImage("dst",dst);
	cvWaitKey();

	return 0;
}