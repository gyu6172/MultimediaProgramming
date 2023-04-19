#include <opencv2/opencv.hpp>
void myMeanFiltering(IplImage* src, IplImage* dst, int k);
void myGaussianFiltering(IplImage* src, IplImage* dst);
void myMedianFiltering(IplImage* src, IplImage* dst, int kn);
int compare(CvScalar a, CvScalar b);

int main() {
	IplImage *src = cvLoadImage("C:\\tmp\\lena.png");
	CvSize size = cvGetSize(src);
	IplImage* meanDst = cvCreateImage(size, 8, 3);
	IplImage* gaussianDst = cvCreateImage(size, 8, 3);
	IplImage* medianDst = cvCreateImage(size, 8, 3);

	myMeanFiltering(src, meanDst, 3);
	myGaussianFiltering(src, gaussianDst);
	myMedianFiltering(src, medianDst, 1);
	cvShowImage("src",src);
	cvWaitKey();
}

void myMeanFiltering(IplImage* src, IplImage* dst, int k)
{
	CvSize size = cvGetSize(src);
	int h = size.height;
	int w = size.width;

	for (int y = k; y < h-k; y++) {
		for (int x = k; x < w-k; x++) {
			//오리지널 버전
			//CvScalar f = cvScalar(0,0,0);
			//for (int i = -k; i <= k; i++) {
			//	for (int j = -k; j <= k; j++) {

			//		CvScalar g = cvGet2D(src, y+i, x+j);

			//		for (int d = 0; d < 3; d++) {
			//			f.val[d] += g.val[d];
			//		}
			//	}
			//}
			//for (int d = 0; d < 3; d++) {
			//	f.val[d] /= (2*k+1)*(2*k+1);
			//}
			//cvSet2D(dst, y, x, f);

			CvScalar f = cvScalar(0, 0, 0);
			for (int i = -k; i <= k; i++) {
				for (int j = -k; j <= k; j++) {

					CvScalar g = cvGet2D(src, y + i, x + j);

					for (int d = 0; d < 3; d++) {
						f.val[d] += g.val[d]/((2*k+1)*(2*k+1));
					}

				}
			}
			cvSet2D(dst, y, x, f);

		}
	}

	cvShowImage("mean",dst);
}

void myGaussianFiltering(IplImage* src, IplImage* dst)
{
	int gauss[3][3]={{1,2,1},
					{2,4,2},
					{1,2,1}
	};
	int gaussSize=1;
	CvSize size = cvGetSize(src);
	int h = size.height;
	int w = size.width;

	for (int y = 1; y < h-1; y++) {
		for (int x = 1; x < w-1; x++) {
			
			CvScalar f = cvScalar(0,0,0);
			
			for (int i = -gaussSize; i <= gaussSize; i++) {
				for (int j = -gaussSize; j <= gaussSize; j++) {

					CvScalar g = cvGet2D(src, y+i, x-j);

					for (int k = 0; k < 3; k++) {
						f.val[k] += g.val[k]/16*gauss[i+gaussSize][j + gaussSize];
					}

				}
			}

			cvSet2D(dst, y, x, f);
		}
	}

	cvShowImage("gaussian", dst);
}

void myMedianFiltering(IplImage* src, IplImage* dst, int kn)
{
	CvSize size = cvGetSize(src);
	int h = size.height;
	int w = size.width;

	int total = (2*kn+1)*(2*kn+1);

	//CvScalar *arr = (CvScalar*)malloc(sizeof(CvScalar)*(2*kn+1));
	CvScalar *arr = new CvScalar[total];

	for (int y = kn; y < h-kn; y++) {
		for (int x = kn; x < w-kn; x++) {

			int iter=0;

			for (int i = -kn; i <= kn; i++) {
				for (int j = -kn; j <= kn; j++) {
					CvScalar g = cvGet2D(src, y+i, x+j);
					arr[iter] = g;
					iter++;
				}
			}
			for (int i = 0; i < total - 1; i++) {
				for (int j = i + 1; j < total; j++) {
					if (compare(arr[i], arr[j]) < 0) {
						//swap f[0] and f[i]
						CvScalar tmp = arr[i];
						arr[i] = arr[j];
						arr[j] = tmp;
					}
				}
			}
			CvScalar f = arr[total/2];
			cvSet2D(dst, y, x, f);
		}
	}
	delete[] arr;
	cvShowImage("median", dst);

}

int compare(CvScalar a, CvScalar b)
//0 : 같다
//양수 : a>b
//음수 : a<b
{
	int a_bri = a.val[0] + a.val[1] + a.val[2];
	int b_bri = b.val[0] + b.val[1] + b.val[2];
	return a_bri - b_bri;
}
