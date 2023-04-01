#include <opencv2/opencv.hpp>

int main() {

	//�̹��� ��� �Է�
	printf("Input File Name : ");
	char str[100];
	scanf("%s",str);

	//�Էµ� ��ο� �ִ� �̹����� �ҷ���.
	IplImage *src = cvLoadImage(str);

	//src�� ũ�⸦ �����ϴ� ����
	CvSize size = cvGetSize(src);


	//���� �̹������� 3��� �� �� �̹����� ũ�⸦ �����ϴ� ����
	CvSize imgSize;
	imgSize.width = size.width;
	imgSize.height = size.height / 3;

	//h�� �̹����� ��������, w�� �̹����� �ʺ�����
	int h = imgSize.height;
	int w = imgSize.width;

	//B������ ������ ���Ե� �̹��� bImg ����
	IplImage* bImg = cvCreateImage(imgSize, 8, 3);

	//G������ ������ ���Ե� �̹��� gImg ����
	IplImage* gImg = cvCreateImage(imgSize, 8, 3);

	//R������ ������ ���Ե� �̹��� rImg ����
	IplImage* rImg = cvCreateImage(imgSize, 8, 3);

	//���� ����� ��� �̹��� dst ����
	IplImage* dst = cvCreateImage(imgSize, 8, 3);

	//newSrc�� 3����Ͽ� bImg, gImg, rImg�� ������.
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			CvScalar bc = cvGet2D(src, y, x);
			CvScalar gc = cvGet2D(src, y + size.height / 3, x);
			CvScalar rc = cvGet2D(src, y + size.height / 3 * 2, x);

			cvSet2D(bImg, y, x, bc);
			cvSet2D(gImg, y, x, gc);
			cvSet2D(rImg, y, x, rc);
		}
	}

	//���ø��� ������� ���� �̹����� 1/3
	CvSize templSize;
	templSize.height = h / 3;
	templSize.width = w / 3;

	//���ø� �̹��� ����
	IplImage* templ = cvCreateImage(templSize, 8, 3);

	//bImg���� ���ø��̹����� �̾Ƴ���.
	for (int y = 0; y < templSize.height; y++) {
		for (int x = 0; x < templSize.width; x++) {
			cvSet2D(templ, y, x, cvGet2D(bImg, y + templSize.height, x + templSize.width));
		}
	}

	//���� ����� �����ϴ� �̹��� ����
	//�̹����� ����,�ʺ�� (�����̹����� ����,�ʺ�) - (���ø��̹����� ����,�ʺ�) + 1
	IplImage* resultG = cvCreateImage(cvSize(w - templSize.width + 1, h - templSize.height + 1), IPL_DEPTH_32F, 1);
	IplImage* resultR = cvCreateImage(cvSize(w - templSize.width + 1, h - templSize.height + 1), IPL_DEPTH_32F, 1);

	//MatchTemplate ���� ����.
	//cvMatchTemplate(�����̹���, ���ø��̹���, ����̹���, ���� ���);
	cvMatchTemplate(gImg, templ, resultG, CV_TM_CCOEFF_NORMED);
	cvMatchTemplate(rImg, templ, resultR, CV_TM_CCOEFF_NORMED);

	//��� �̹������� �ְ���Ȯ���� ����Ͽ� ������ ����
	double maxg, maxr;

	//��Ȯ���� �ְ��϶��� �簢�� ���� �� ��ǥ�� ������ ����
	//���� bImg, gImg, rImg�� ��ǥ�� ������.
	CvPoint pb, pg, pr;
	
	//bImg�� �����̹Ƿ� ���ø��̹����� �������� ����ִ��� ������.
	pb.x = templSize.width;
	pb.y = templSize.height;

	//��� �̹������� ��Ȯ���� ���� ���� ���� �簢�� ���� �� ��ǥ�� ������.
	cvMinMaxLoc(resultG, NULL, &maxg, NULL, &pg);
	cvMinMaxLoc(resultR, NULL, &maxg, NULL, &pr);

	//bImg�� gImg�� ��ǥ�� ����, bImg�� rImg�� ��ǥ�� ���̸� ������ ����
	CvPoint diffBG, diffBR;
	diffBG.x = pg.x - pb.x;
	diffBG.y = pg.y - pb.y;
	diffBR.x = pr.x - pb.x;
	diffBR.y = pr.y - pb.y;
	//bImg�� (x, y)��ġ�� �ִ� ���� gImg�� (x+diffBG.x, y+diffBG.y)��ġ�� �ִ� ���� �����ȴ�.
	//bImg�� (x, y)��ġ�� �ִ� ���� rImg�� (x+diffBR.x, y+diffBR.y)��ġ�� �ִ� ���� �����ȴ�.

	//dst�� �׸���
	for (int y = 0; y < imgSize.height; y++) {
		for (int x = 0; x < imgSize.width; x++) {
			//���� ��ǥ�� �̹��� ���̶�� ������ �������� ����.
			if(0>x+diffBG.x || x+diffBG.x>w-1) continue;
			if(0>x+diffBR.x || x+diffBR.x>w-1) continue;
			if(0>y+diffBG.y || y+diffBG.y>h-1) continue;
			if(0>y+diffBR.y || y+diffBR.y>h-1) continue;

			//�� �������� �ȼ����� �̾Ƴ�.
			CvScalar b = cvGet2D(bImg, y, x);
			CvScalar g = cvGet2D(gImg, y+diffBG.y, x+diffBG.x);
			CvScalar r = cvGet2D(rImg, y+diffBR.y, x+diffBR.x);

			//dst�� BGR���� �����Ͽ� ����.
			CvScalar color;
			color.val[0] = b.val[0];
			color.val[1] = g.val[0];
			color.val[2] = r.val[0];
			cvSet2D(dst, y, x, color);
		}
	}

	//src�� dst ����
	cvShowImage("src", src);
	cvShowImage("dst", dst);

	cvWaitKey();

	return 0;
}

