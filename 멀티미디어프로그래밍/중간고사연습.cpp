#include <opencv2/opencv.hpp>
int main() {
	IplImage *src = cvCreateImage(cvSize(800,600), 8, 3);
	CvSize size = cvGetSize(src);
	
	CvScalar sc = cvScalar(255,0,0);
	CvScalar ec = cvScalar(0,0,255);
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			CvScalar f = cvScalar(0,0,0);
			
			float nx = (float(x)/(size.width-1))*2-1;
			float ny = (float(y)/(size.height-1))*2-1;

			for (int k = 0; k < 3; k++) {
				f.val[k] = sc.val[k]*(1- sqrt(nx * nx + ny * ny)) + ec.val[k]*(sqrt(nx*nx+ny*ny));
			}

			cvSet2D(src, y, x, f);
		}
	}

	cvShowImage("src",src);
	cvWaitKey();

}