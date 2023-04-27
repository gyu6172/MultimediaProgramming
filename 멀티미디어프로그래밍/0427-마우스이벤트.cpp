#include <opencv2/opencv.hpp>

//콜백함수의 형식(함수명은 마음대로)
void myMouse(int event, int x, int y, int flags, void*) {

	//CV_EVENT_FLAG_LBUTTON : 1(10) : 0000 0001(2)
	//CV_EVENT_FLAG_RBUTTON : 2(10) : 0000 0010(2)
	//CV_EVENT_FLAG_MBUTTON : 4(10) : 0000 0100(2)
	//CV_EVENT_FLAG_CTRLKEY : 8(10) : 0000 1000(2)
	//CV_EVENT_FLAG_SHIFTKEY : 16(10) : 0001 0000(2)
	//CV_EVENT_FLAG_ALTKEY : 32(10) : 0010 0000(2)

	//Ex : 왼클릭과 컨트롤키를 누른 상황
	int myFlag = CV_EVENT_FLAG_LBUTTON | CV_EVENT_FLAG_CTRLKEY;

	//주의 : &연산자는 !=연산자보다 우선순위가 늦다.
	//if ((flags & CV_EVENT_FLAG_SHIFTKEY) != 0) {	//쉬프트가 눌리기만 한다면
	//	printf("Mouse : %d (%d, %d) flag=%d\n", event, x, y, flags);

	//}
	if (flags == myFlag) {
		printf("Mouse : %d (%d, %d) flag=%d\n", event, x, y, flags);
	}
	if (event == CV_EVENT_LBUTTONDOWN) {
		
	}
}

int main() {
	//마우스는 C언어가 만들어질때 없었다.
	//마우스는 운영체제가 관리하기 때문에 Windows API를 다룰줄 알아야 마우스를 쓸 수 있다.
	//콜백 함수 : 특정한 조건(우리가 만들지 않음)을 만족했을 때 호출되는 함수
	//void func(int event, int x, int y, int flags, void *param);	<--이런 형태이다.
	//event : 마우스에 어떤 변화가 있었는지
	//x : 마우스의 x좌표
	//y : 마우스의 y좌표
	//flags : 호출될 당시에 다른 정보(Ex:마우스 왼클릭 했을때 Ctrl키를 누르고 있었어.)

	IplImage* img = cvCreateImage(cvSize(500,500),8,3);
	cvSet(img, cvScalar(255,255,255));

	cvShowImage("image", img);
	cvSetMouseCallback("image", myMouse);

	//cvWaitKey : 키보드 입력을 기다리면서 무한 루프 돎. 마우스에 변화가 생기면 myMouse함수를 호출한다.
	cvWaitKey();

	return 0;


	

	
}