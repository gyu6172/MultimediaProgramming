#include <opencv2/opencv.hpp>
void img1(IplImage *src);
void img2(IplImage* src);
void img3(IplImage* src);
void img4(IplImage* src);
void img5(IplImage* src);
void img6(IplImage* src);

CvScalar getGray(IplImage *src, int y, int x);
int main() {
	IplImage *src = cvLoadImage("C:\\tmp\\sejong_small.jpg");
	if (src == nullptr) {
		printf("Image Load Error!");
		return -1;
	}

	img1(src);
	img2(src);
	img3(src);
	img4(src);
	img5(src);
	img6(src);

	cvWaitKey();
}

void img1(IplImage* src)
{
	CvSize size = cvGetSize(src);
	int h = size.height;
	int w = size.width;

	IplImage *dst = cvCreateImage(size, 8, 3);
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			CvScalar color = cvGet2D(src, y,x);
			cvSet2D(dst, h-y-1, x, color);
		}
	}

	cvShowImage("img1",dst);
}

void img2(IplImage* src)
{
	CvSize size = cvGetSize(src);
	int h = size.height;
	int w = size.width;

	IplImage* dst = cvCreateImage(size, 8, 3);

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			CvScalar color = cvGet2D(src, y, x);
			if (x < w / 2) {
				cvSet2D(dst, y,x+w/2,color);
			}
			else if (x >= w / 2) {
				cvSet2D(dst, y, x - w / 2, color);
			}
		}
	}

	cvShowImage("img2", dst);
}

void img3(IplImage* src)
{
	CvSize size = cvGetSize(src);
	int h = size.height;
	int w = size.width;

	IplImage* dst = cvCreateImage(size, 8, 3);

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			double nx = double(x)/(w-1)*2-1;
			double ny = double(y)/(h-1)*2-1;
			if(-1<nx+ny && nx+ny<1 && -1<ny-nx && ny-nx<1){
				cvSet2D(dst, y, x, cvGet2D(src,y,x));
			}
			else{
				cvSet2D(dst, y, x, getGray(src, y, x));
			}
		}
	}

	cvShowImage("img3", dst);
}

void img4(IplImage* src)
{
	CvSize size = cvGetSize(src);
	int h = size.height;
	int w = size.width;
	IplImage* dst = cvCreateImage(size, 8, 3);

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			double nx = double(x) / (w - 1) * 2 - 1;
			double ny = double(y) / (h - 1) * 2 - 1;
			if (nx * nx + ny * ny > 1) {
				cvSet2D(dst, y, x, getGray(src, y, x));
			}
			else {
				cvSet2D(dst, y, x, cvGet2D(src, y, x));
			}
		}
	}

	cvShowImage("img4", dst);
}

void img5(IplImage* src)
{
	CvSize size = cvGetSize(src);
	int h = size.height;
	int w = size.width;
	IplImage* dst = cvCreateImage(size, 8, 3);

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			double nx = double(x) / (w - 1) * 20 - 10;
			double ny = double(y) / (h - 1) * 20 - 10;
			if (int(sqrt(nx * nx + ny * ny))%2==1) {
				cvSet2D(dst, y, x, cvScalar(0,0,0));
			}
			else{
				cvSet2D(dst, y, x, cvGet2D(src, y, x));
			}
		}
	}

	cvShowImage("img5", dst);
}

void img6(IplImage* src)
{
	CvSize size = cvGetSize(src);
	int h = size.height;
	int w = size.width;
	IplImage* dst = cvCreateImage(size, 8, 3);

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			double nx = double(x) / (w - 1)*10;
			double ny = double(y) / (h - 1)*10;
			if (int(ny)%2==0) {
				if (int(nx) % 2 == 0) {
					cvSet2D(dst, y, x, cvScalar(0, 0, 0));
				}
				else if (int(nx) % 2 == 1) {
					cvSet2D(dst, y, x, cvGet2D(src, y, x));
				}
			}
			else if(int(ny)%2==1){
				if (int(nx) % 2 == 0) {
					cvSet2D(dst, y, x, cvGet2D(src, y, x));
				}
				else if (int(nx) % 2 == 1) {
					cvSet2D(dst, y, x, cvScalar(0,0,0));
				}
			}
		}
	}
	cvShowImage("img6", dst);
}

CvScalar getGray(IplImage* src, int y, int x)
{
	CvScalar color = cvGet2D(src, y, x);
	
	int bright = (color.val[0] + color.val[1] + color.val[2])/3;
	CvScalar grayColor;
	for (int k = 0; k < 3; k++) {
		grayColor.val[k] = bright;
	}
	
	return grayColor;
}
