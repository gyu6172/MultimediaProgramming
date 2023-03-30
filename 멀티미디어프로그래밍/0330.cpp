#include <opencv2/opencv.hpp>
int main() {
	IplImage* src = cvLoadImage("c:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);
	IplImage* dst = cvCreateImage(size, 8, 3);

	int Kn = 1;


	//float kernel[3][3]={
	//	{1, 2, 1},
	//	{2, 4, 2},
	//	{1, 2, 1}
	//};
	//for (int u = 0; u < 3; u++) {
	//	for (int v = 0; v < 3; v++) {
	//		kernel[u][v] /= 16.0f;
	//	}
	//}

	float kernel[3][3] = {
		{1, 0, -1},
		{1, 0, -1},
		{1, 0, -1}
	};
	//for (int u = 0; u < 3; u++) {
	//	for (int v = 0; v < 3; v++) {
	//		kernel[u][v] /= 9.0f;
	//	}
	//}

	cvShowImage("src", src);
	for (int y = Kn; y < size.height - Kn; y++) {
		for (int x = Kn; x < size.width - Kn; x++) {
			CvScalar g = cvScalar(0, 0, 0);

			for (int u = -Kn; u <= Kn; u++) {
				for (int v = -Kn; v <= Kn; v++) {

					CvScalar f = cvGet2D(src, y + v, x + u);
					for (int k = 0; k < 3; k++) {
						g.val[k] += kernel[v + Kn][u + Kn] * f.val[k];
					}
				}
			}
			for (int k = 0; k < 3; k++) {
				g.val[k] = abs(g.val[k]);
			}
			cvSet2D(dst, y, x, g);
		}

		cvShowImage("dst", dst);
		cvWaitKey();
		cvCopy(dst, src);
	}




	return 0;
}