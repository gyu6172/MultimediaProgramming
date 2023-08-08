#include <opencv2/opencv.hpp>

CvScalar adjustBrightAndContrast(CvScalar f, float a, float b) {
	CvScalar out;
	out.val[0] = a*f.val[0] + b;
	out.val[1] = a*f.val[1] + b;
	out.val[2] = a*f.val[2] + b;


	return out;
}

void drawGraph(IplImage* img, float a, float b) {
	cvSet(img, cvScalar(255,255,255));
	int y_prev = 0;

	for (int x = 0; x < 256; x++) {
		int y = a*x + b;
		if(x==0) y_prev = y;

		if(y<0 || y>255)continue;
		if(abs(y-y_prev)<2)
			cvSet2D(img, 255-y, x, cvScalar(0,0,0));
		else {
			int y1 = y_prev;
			int y2 = y;
			if (y1 > y2) {
				std::swap(y1,y2);
			}
			for (int yy = y1; yy < y2; yy++) {
				cvSet2D(img, 255 - yy, x, cvScalar(0, 0, 0));

			}
		}
		y_prev=y;
	}
}

int main() {
	IplImage *src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);

	IplImage *dst = cvCreateImage(size, 8, 3);
	IplImage *graph = cvCreateImage(cvSize(256,256), 8, 3);

	int w = size.width;
	int h = size.height;

	float a = 1.0f;
	float b = 0.0f;


	while (true) {

		//Image Processing :	1. Image Filtering(색 바꾸기) - 1.1 point processing
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				CvScalar f = cvGet2D(src, y, x);	//input color

				CvScalar g = adjustBrightAndContrast(f, a, b);						//output color

				if (x < w / 2) g = f;

				cvSet2D(dst, y, x, g);
			}
		}

		drawGraph(graph, a, b);

		//cvShowImage("src", src);
		cvShowImage("dst", dst);
		cvShowImage("graph", graph);

		int c = cvWaitKey();
		switch (c) {
			case '1': b += 10; break;
			case '2': b -= 10; break;
			case '3': a += 0.1f; break;
			case '4': a -= 0.1f; break;
			case 'q': return 0; break;
			default: break;

		}
		printf("a=%f b=%f\n",a,b);


	}
}

