#include <opencv2/opencv.hpp>

//이미지 피라미드?

struct align {
	int dx;
	int dy;
	float error;
};

float dist(CvScalar f, CvScalar g) {
	float sum=0.0;
	for (int k = 0; k < 3; k++) {
		sum += (f.val[k] - g.val[k])*(f.val[k] - g.val[k]);
	}
	return sum;
}

//pa를 기준으로 -du~du까지, -dv~dv까지 step만큼 움직이며 검사
void myAlign(IplImage* src, IplImage* dst, align* pa, int du, int dv, int step) {
	if(step < 1)return;
	if(du < 1)return;
	if(dv < 1)return;

	CvSize size = cvGetSize(src);
	int w = size.width;
	int h = size.height;

	IplImage *s_blur = cvCreateImage(size, 8, 3);
	IplImage *d_blur = cvCreateImage(size, 8, 3);
	cvSmooth(src, s_blur, CV_BLUR, 2*step+1);
	cvSmooth(dst, d_blur, CV_BLUR, 2*step+1);

	float minErr = FLT_MAX;
	int min_u = 0;
	int min_v = 0;

	int stu = pa->dx - du;
	int edu = pa->dx + du;
	int stv = pa->dy - dv;
	int edv = pa->dy + dv;

	int ystep = h/100;
	int xstep = w/100;

	int ustep = step;
	int vstep = step;

	//범위를 결정해주자.
	for (int v = stv; v < edv; v=+vstep) {
		for (int u = stu; u < edu; u+=ustep) {
			float err = 0.0f;
			int count =0;
			for (int y = h/5; y < h/5*4; y+=ystep) {
				for (int x = w/5; x < w/5*4; x+=xstep) {
					int nx = x+u;
					int ny = y+v;

					if(nx<0 || nx>w-1)continue;
					if(ny<0 || ny>h-1)continue;

					CvScalar f = cvGet2D(s_blur, y, x);
					CvScalar g = cvGet2D(d_blur, ny, nx);

					err += dist(f, g);
					count++;
				}
			}
			err /= count;
			if (err < minErr) {
				minErr = err;
				min_u = u;
				min_v = v;
				printf("u=%d v=%d err=%f\n",u,v,err);
			}
		}
	}
	pa->dx = min_u;
	pa->dy = min_v;
	pa->error = minErr;

	myAlign(src, dst, pa, step, step, step/4);

	cvReleaseImage(&s_blur);
	cvReleaseImage(&d_blur);
}

int main() {
	IplImage *src = cvLoadImage("C:\\tmp\\pg1.jpg");
	
	int w = src->width;
	int h = src->height/3;
	CvSize size = cvSize(w,h);
	IplImage *dst = cvCreateImage(size, 8, 3);

	IplImage *s[3];
	for (int i = 0; i < 3; i++) {
		s[i] = cvCreateImage(size, 8, 3);
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int y2 = y+h*i;
				CvScalar f = cvGet2D(src, y2, x);
				cvSet2D(s[i], y, x, f);
			}
		}
	}

	align al[3];
	for (int i = 0; i < 3; i++) {
		al[i].dx = 0;
		al[i].dy = 0;
		al[i].error = FLT_MAX;
	}

	myAlign(s[0], s[1], &al[1], w/4, h/4, 16);
	myAlign(s[0], s[2], &al[2], w/4, h/4, 16);

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			CvScalar g = cvScalar(0,0,0);

			for (int i = 0; i < 3; i++) {
				int nx = x+al[i].dx;
				int ny = y+al[i].dy;

				if(nx<0 || nx>w-1)continue;
				if(ny<0 || ny>h-1)continue;
				
				CvScalar f = cvGet2D(s[i], ny, nx);
				g.val[i] = f.val[i];
			}

			cvSet2D(dst, y, x, g);
		}
	}


	cvShowImage("src", src);
	cvShowImage("dst", dst);
	cvWaitKey();
	return 0;
}