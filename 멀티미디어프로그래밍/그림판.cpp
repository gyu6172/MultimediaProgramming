#include <opencv2/opencv.hpp>
CvScalar BLACK = cvScalar(0,0,0);
IplImage* paint = cvCreateImage(cvSize(1000, 700), 8, 3);

typedef struct Button {
	int x,y;
	int width,height;
	CvScalar color;
}Btn;
Button* buttons;
int buttonCnt;
void setButton(IplImage* img, Button* btn) {
	CvPoint p1, p2;
	p1.x = btn->x;
	p1.y = btn->y;
	p2.x = p1.x + btn->width;
	p2.y = p1.y + btn->height;
	cvRectangle(img, p1, p2, btn->color, -1);
}
void myMouse(int event, int x, int y, int flags, void*) {
	if (event == CV_EVENT_MOUSEMOVE) {
		for (int i = 0; i < buttonCnt; i++) {
			if (buttons[i].x <= x && x <= buttons[i].x+buttons[i].width) {
				if (buttons[i].y <= y && y <= buttons[i].y + buttons[i].height) {
					buttons[i].color = cvScalar(100,100,100);
					setButton(paint, &buttons[i]);
					printf("btn%d, x:%d, y:%d\n",i,x,y);
					cvShowImage("그림판", paint);
				}
				else {
					buttons[i].color = cvScalar(200,200,200);
					setButton(paint, &buttons[i]);
					cvShowImage("그림판", paint);

				}
			}
		}
	}
}

int main() {
	cvSet(paint, cvScalar(255,255,255));

	buttons = (Button*)malloc(sizeof(Button)*100);
	for (int y = 10; y <= 50; y += 40) {
		for (int x = 10; x <= 90; x += 40) {
			Button btn = {x,y,30,30,cvScalar(200,200,200)};
			buttons[buttonCnt++] = btn;
			setButton(paint, &btn);
		}
	}

	cvShowImage("그림판",paint);
	cvSetMouseCallback("그림판", myMouse);
	cvWaitKey();
}