#include <opencv2/opencv.hpp>

struct Stroke {
	CvPoint pt;
	CvScalar color;
	int R;
};

float diff2(CvScalar f, CvScalar g) {
	float sum = 0.0f;

	for (int k = 0; k < 3; k++) {
		sum += (f.val[k]-g.val[k]) * (f.val[k] - g.val[k]);
	}

	return sum;
}

void paintLayer(IplImage* dst, IplImage* ref, int R) {
	int grid = R * 1.0f;
	int xdiv = dst->width / grid/* + 1*/;	//+1은 자투리를 버린다는 뜻이다.
	int ydiv = dst->height / grid/* + 1*/;

	int totalStroke = xdiv * ydiv;
	Stroke* s = new Stroke[totalStroke];
	int curNum = 0;

	float T = 400.0f;

	for (int i = 0; i < ydiv; i++) {
		for (int j = 0; j < xdiv; j++) {

			float err = 0.0f;
			int ct = 0;
			float max_err = 0.0f;
			CvScalar max_color;
			CvPoint max_pos;

			for (int v = 0; v < grid; v++) {
				for (int u = 0; u < grid; u++) {
					int x = grid * j + u;
					int y = grid * i + v;

					if (x<0 || x>dst->width - 1) continue;
					if (y<0 || y>dst->height - 1) continue;

					CvScalar f = cvGet2D(ref, y, x);
					CvScalar d = cvGet2D(dst, y, x);
					
					float cur_err = diff2(f, d);
					err += cur_err;
					ct++;

					if (cur_err > max_err) {
						max_err = cur_err;
						max_color = f;
						max_pos = cvPoint(x, y);
					}
				}
			}

			err /= ct;
			if (err > T) {
				s[curNum].color = max_color;
				s[curNum].pt = max_pos;
				s[curNum].R = R;
				curNum++;
			}
		}
	}

	for (int i = 0; i < curNum; i++) {
		int idx = rand() % curNum;
		Stroke tmp = s[i];
		s[i] = s[idx];
		s[idx] = tmp;
	}

	for (int i = 0; i < curNum; i++) {
		
		cvCircle(dst, s[i].pt, s[i].R, s[i].color, -1);
	}

	delete[] s;
}

int main() {

	IplImage* src = cvLoadImage("C:\\tmp\\lena.png");
	IplImage* dst = cvCreateImage(cvGetSize(src), 8, 3);

	cvSet(dst, cvScalar(255, 255, 255));

	int R[5] = { 32,16,8,4,2 };
	int num = 5;

	IplImage* ref = cvCreateImage(cvGetSize(src), 8, 3);
	for (int i = 0; i < num; i++) {
		cvSmooth(src, ref, CV_GAUSSIAN, R[i] * 4 + 1);
		paintLayer(dst, ref, R[i]);
		cvShowImage("dst", dst);
		cvWaitKey();
	}
}