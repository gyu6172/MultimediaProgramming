#include <opencv2/opencv.hpp>

//�ݹ��Լ��� ����(�Լ����� �������)
void myMouse(int event, int x, int y, int flags, void*) {

	//CV_EVENT_FLAG_LBUTTON : 1(10) : 0000 0001(2)
	//CV_EVENT_FLAG_RBUTTON : 2(10) : 0000 0010(2)
	//CV_EVENT_FLAG_MBUTTON : 4(10) : 0000 0100(2)
	//CV_EVENT_FLAG_CTRLKEY : 8(10) : 0000 1000(2)
	//CV_EVENT_FLAG_SHIFTKEY : 16(10) : 0001 0000(2)
	//CV_EVENT_FLAG_ALTKEY : 32(10) : 0010 0000(2)

	//Ex : ��Ŭ���� ��Ʈ��Ű�� ���� ��Ȳ
	int myFlag = CV_EVENT_FLAG_LBUTTON | CV_EVENT_FLAG_CTRLKEY;

	//���� : &�����ڴ� !=�����ں��� �켱������ �ʴ�.
	//if ((flags & CV_EVENT_FLAG_SHIFTKEY) != 0) {	//����Ʈ�� �����⸸ �Ѵٸ�
	//	printf("Mouse : %d (%d, %d) flag=%d\n", event, x, y, flags);

	//}
	if (flags == myFlag) {
		printf("Mouse : %d (%d, %d) flag=%d\n", event, x, y, flags);
	}
	if (event == CV_EVENT_LBUTTONDOWN) {
		
	}
}

int main() {
	//���콺�� C�� ��������� ������.
	//���콺�� �ü���� �����ϱ� ������ Windows API�� �ٷ��� �˾ƾ� ���콺�� �� �� �ִ�.
	//�ݹ� �Լ� : Ư���� ����(�츮�� ������ ����)�� �������� �� ȣ��Ǵ� �Լ�
	//void func(int event, int x, int y, int flags, void *param);	<--�̷� �����̴�.
	//event : ���콺�� � ��ȭ�� �־�����
	//x : ���콺�� x��ǥ
	//y : ���콺�� y��ǥ
	//flags : ȣ��� ��ÿ� �ٸ� ����(Ex:���콺 ��Ŭ�� ������ CtrlŰ�� ������ �־���.)

	IplImage* img = cvCreateImage(cvSize(500,500),8,3);
	cvSet(img, cvScalar(255,255,255));

	cvShowImage("image", img);
	cvSetMouseCallback("image", myMouse);

	//cvWaitKey : Ű���� �Է��� ��ٸ��鼭 ���� ���� ��. ���콺�� ��ȭ�� ����� myMouse�Լ��� ȣ���Ѵ�.
	cvWaitKey();

	return 0;


	

	
}