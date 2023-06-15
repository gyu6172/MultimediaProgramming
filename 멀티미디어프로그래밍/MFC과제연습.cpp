#include <opencv2/opencv.hpp>
#define PI 3.141592653589
IplImage* img;
IplImage* cpy;
CvPoint P, Q;

CvPoint outpenta[5];
CvPoint inpenta[5];

void drawPentagon(IplImage* img, CvPoint st, CvPoint ed) {
	int r = sqrt(pow(st.x - ed.x, 2) + pow(st.y - ed.y, 2));

	float theta = -2.0f * PI / 5;

	outpenta[0] = cvPoint(st.x, st.y - r);
	inpenta[0] = cvPoint(st.x, st.y + r / 2.8f);
	for (int i = 1; i < 5; i++) {
		CvPoint prevP1 = outpenta[i - 1];
		int x1 = cos(theta) * (prevP1.x - st.x) - sin(theta) * (prevP1.y - st.y) + st.x;
		int y1 = sin(theta) * (prevP1.x - st.x) + cos(theta) * (prevP1.y - st.y) + st.y;

		CvPoint prevP2 = inpenta[i - 1];
		int x2 = cos(theta) * (prevP2.x - st.x) - sin(theta) * (prevP2.y - st.y) + st.x;
		int y2 = sin(theta) * (prevP2.x - st.x) + cos(theta) * (prevP2.y - st.y) + st.y;
		outpenta[i] = cvPoint(x1, y1);
		inpenta[i] = cvPoint(x2, y2);
	}


	CvPoint tmp1 = inpenta[3];
	CvPoint tmp2 = inpenta[4];
	for (int i = 2; i >= 0; i--) {
		inpenta[i + 2] = inpenta[i];
	}
	inpenta[0] = tmp1;
	inpenta[1] = tmp2;

	for (int i = 0; i < 4; i++) {
		cvLine(img, outpenta[i], outpenta[i + 1], cvScalar(50 * i, 50 * i, 50 * i), 3);
		cvLine(img, inpenta[i], inpenta[i + 1], cvScalar(50 * i, 50 * i, 50 * i), 3);
	}

	/*cvLine(img, outpenta[4], outpenta[0], cvScalar(0, 0, 0), 3);
	cvLine(img, inpenta[4], inpenta[0], cvScalar(0, 0, 0), 3);*/
}

void drawStar(IplImage* img, CvPoint st, CvPoint ed) {
	int r = sqrt(pow(st.x-ed.x, 2) + pow(st.y-ed.y, 2));

	float theta = -2.0f*PI/5;

	outpenta[0] = cvPoint(st.x, st.y-r);
	inpenta[0] = cvPoint(st.x, st.y+r/2.618f);
	for (int i = 1; i < 5; i++) {
		CvPoint prevP1 = outpenta[i-1];
		int x1 = cos(theta)*(prevP1.x - st.x) - sin(theta)*(prevP1.y-st.y) + st.x;
		int y1 = sin(theta)*(prevP1.x - st.x) + cos(theta)*(prevP1.y-st.y) + st.y;

		CvPoint prevP2 = inpenta[i - 1];
		int x2 = cos(theta) * (prevP2.x - st.x) - sin(theta) * (prevP2.y - st.y) + st.x;
		int y2 = sin(theta) * (prevP2.x - st.x) + cos(theta) * (prevP2.y - st.y) + st.y;
		outpenta[i] = cvPoint(x1,y1);
		inpenta[i] = cvPoint(x2,y2);
	}

	
	CvPoint tmp1 = inpenta[3];
	CvPoint tmp2 = inpenta[4];
	for (int i = 2; i >= 0; i--) {
		inpenta[i+2] = inpenta[i];
	}
	inpenta[0] = tmp1;
	inpenta[1] = tmp2;

	for (int i = 0; i < 10; i++) {
		if (i % 2 == 0) {
			cvLine(img, outpenta[i/2], inpenta[i/2], cvScalar(0,0,0), 3);
		}
		else if (i % 2 == 1) {
			cvLine(img, inpenta[i/2], outpenta[(i/2+1)%5], cvScalar(0, 0, 0), 3);
		}
	}

}



void myMouse(int event, int x, int y, int flag, void*) {
	if (event == CV_EVENT_LBUTTONDOWN) {
		P = cvPoint(x, y);
		cvCopy(img ,cpy);
	}
	if (event == CV_EVENT_MOUSEMOVE) {
		if (flag == CV_EVENT_FLAG_LBUTTON) {
			Q = cvPoint(x, y);
			cvCopy(cpy, img);

			drawStar(img, P, Q);

			cvShowImage("img", img);

		}

	}
}

int main() {
	img = cvCreateImage(cvSize(800, 600), 8, 3);
	cvSet(img, cvScalar(255,255,255));
	cpy = cvCloneImage(img);

	cvShowImage("img",img);
	cvSetMouseCallback("img", myMouse);
	cvWaitKey();
}