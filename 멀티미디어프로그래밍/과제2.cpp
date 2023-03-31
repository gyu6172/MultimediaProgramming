#include <opencv2/opencv.hpp>

//�̹����� �Է����� �޾�, y��ǥ�� startY���� �����ؼ� dy��ŭ �����ϸ鼭 �̹����� ��踦 ã�� �Լ�
int findBoundaryVertical(IplImage* img, int startY, int dy);

//�� CvScalar�� ������ ���̸� �����ϴ� �Լ�
float getDistance(CvScalar f, CvScalar g);

int main() {
	//�̹��� ��� �Է�
	printf("Input File Name : ");
	char str[100];
	scanf("%s",str);

	//�Էµ� ��ο� �ִ� �̹����� �ҷ���.
	IplImage *src = cvLoadImage(str);

	//src�� ũ�⸦ �����ϴ� ����
	CvSize size = cvGetSize(src);

	//src �̹����� ����ó�� �� Ʋ �ȿ� �׷��� �ִµ�, �� Ʋ�� ���� ���� �Ʒ��� ����� y��ǥ�� ������ ����
	int topBoundary, bottomBoundary;
	//�̹����� ���ʺ��� �Ʒ������� Ž���ϸ鼭 ó�� ������ ��輱�� y��ǥ�� ������.
	topBoundary = findBoundaryVertical(src, 2, 1);
	//�̹����� �Ʒ��ʺ��� �������� Ž���ϸ鼭 ó�� ������ ��輱�� y��ǥ�� ������.
	bottomBoundary = findBoundaryVertical(src, size.height - 2, -1);

	//���� ��輱���� ���� �ִ� �κа�, �Ʒ��� ��輱���� �Ʒ��� �ִ� �κ��� �ʿ���� �����̹Ƿ� �������� ���̴�.
	
	//���� ��, ���ο� �̹����� ũ�⸦ ������ ����
	CvSize newSrcSize = size;

	//���� �̹����� ���̿� �Ʒ��� ��輱�� y��ǥ�� ������ ��, ���� ��輱�� y��ǥ�� ���� ���ο� �̹����� ���̰� ����.
	newSrcSize.height = bottomBoundary;
	newSrcSize.height -= topBoundary;

	//���� �̹������� 3��� �� �� �̹����� ũ�⸦ �����ϴ� ����
	CvSize imgSize;
	imgSize.width = size.width;
	imgSize.height = newSrcSize.height / 3;

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

	//�ʿ���� ������ ������ �̹����� ����� newSrc
	IplImage* newSrc = cvCreateImage(newSrcSize, 8, 3);
	//newSrc�� ���� ��輱���� �Ʒ��� ��輱������ �̹����� �������ش�.
	for (int y = topBoundary; y < bottomBoundary; y++) {
		for (int x = 0; x < newSrcSize.width; x++) {
			CvScalar color = cvGet2D(src, y, x);

			cvSet2D(newSrc, y - topBoundary, x, color);
		}
	}

	//newSrc�� 3����Ͽ� bImg, gImg, rImg�� ������.
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			CvScalar bc = cvGet2D(newSrc, y, x);
			CvScalar gc = cvGet2D(newSrc, y + newSrcSize.height / 3, x);
			CvScalar rc = cvGet2D(newSrc, y + newSrcSize.height / 3 * 2, x);

			cvSet2D(bImg, y, x, bc);
			cvSet2D(gImg, y, x, gc);
			cvSet2D(rImg, y, x, rc);
		}
	}

	//�̹��� ���Ŀ� ����
	//B�̹����� G�̹����� �ּ����̰� ����� ����
	float minDiffBG = FLT_MAX;

	//B�̹����� R�̹����� �ּ����̰� ����� ����
	float minDiffBR = FLT_MAX;

	//B�̹����� G�̹���, �Ǵ� B�̹����� R�̹����� �ּ������϶��� u���� v���� ����� ����
	int uBG, uBR, vBG, vBR;

	//u�� v�� -10���� 10���� �ݺ��� ����.
	for (int v = -10; v <= 10; v++) {
		for (int u = -10; u <= 10; u++) {
			//�̹����� �� �ȼ������� ���̸� ��� ���Ͽ� ������ ����
			float sumBG = 0.0;
			float sumBR = 0.0;
			//���� Ƚ���� ����� ����
			int cnt = 0;
			//y��ǥ�� h/4���� h/4*3����, x��ǥ�� w/4���� w/4*3���� �ݺ�
			for (int y = h / 4; y < h / 4 * 3; y++) {
				for (int x = w / 4; x < w / 4 * 3; x++) {
					//���� ������ �����ϴ� ���� �̹��� ���̶�� ������ ���� ����
					if (x + u<0 || x + u>w - 1) continue;
					if (y + v<0 || y + v>h - 1) continue;

					//getDistance�Լ��� �̿��Ͽ� bImg������ (x, y)������ �ȼ����� gImg������ (x+u, y+v)������ �ȼ����� ���̸� ����.
					sumBG += getDistance(cvGet2D(bImg, y, x), cvGet2D(gImg, y + v, x + u));
					sumBR += getDistance(cvGet2D(bImg, y, x), cvGet2D(rImg, y + v, x + u));
					//����Ƚ�� 1����
					cnt++;
				}
			}
			//���� ������ ���� �ּ� ���̺��� �۴ٸ�
			if (sumBG / cnt < minDiffBG) {
				//�� ���� �ּ����� ������ ������.
				minDiffBG = sumBG / cnt;
				//�ּ������϶��� u,v�� ����.
				uBG = u;
				vBG = v;
			}
			if (sumBR / cnt < minDiffBR) {
				minDiffBR = sumBR / cnt;
				uBR = u;
				vBR = v;
			}
		}
	}

	//dst�� �� �����ϱ�
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			//���� ���� �ȼ��� ��ǥ�� �̹��� ���̶�� ���� �������� ����.
			if (x + uBG<0 || x + uBG>w - 1) continue;
			if (y + vBG<0 || y + vBG>h - 1) continue;
			if (x + uBR<0 || x + uBR>w - 1) continue;
			if (y + vBR<0 || y + vBR>h - 1) continue;

			//bImg������ (x,y)��ġ�� �ȼ����� ����
			CvScalar b = cvGet2D(bImg, y, x);
			
			//gImg������ (x+uBG, y+vBG)��ġ�� �ȼ����� ����
			CvScalar g = cvGet2D(gImg, y + vBG, x + uBG);

			//rImg������ (x+uBR, y+vBR)��ġ�� �ȼ����� ����
			CvScalar r = cvGet2D(rImg, y + vBR, x + uBR);

			//dst�� ����� CvScalar��
			CvScalar color;

			//CvScalar���� BGR������ �ʱ�ȭ��.
			color.val[0] = b.val[0];
			color.val[1] = g.val[0];
			color.val[2] = r.val[0];

			//�� ���� dst�� ����.
			cvSet2D(dst, y, x, color);
		}
	}

	//src�� dst�̹��� �����ֱ�
	cvShowImage("src", src);
	cvShowImage("dst", dst);


	cvWaitKey();

	return 0;
}

//img������ ��踦 ã�� �Լ�.
//y��ǥ�� startY�϶� �� ���� ��Ⱚ�� ��� ���ϰ�, y�� dy��ŭ ���� ���� ������ ������.
//�� ���̰� Ư�� ����� �̻��̸� �̹������� ��輱�� �� ���̴�.
//dy�� ����� ��� ������ �Ʒ��� Ž��, ������ ��� �Ʒ����� ���� Ž���� ����
int findBoundaryVertical(IplImage* img, int startY, int dy)
{
	//y=startY�� �ʱ�ȭ, y��ǥ�� �̹��� �ȿ� ���� ��, y�� dy��ŭ ����.
	for (int y = startY;0 < y && y < cvGetSize(img).height - 1 ; y += dy) {
		//y��ǥ�� y�� ���� ��Ⱚ�� ���
		double bright = 0.0;
		//y��ǥ�� y+dy�� ���� ��Ⱚ�� ���
		double nextBright = 0.0;

		//�̹����� ���ʿ��� �����ʱ��� ��Ⱚ�� ��� ���Ͽ� ����
		for (int x = 0; x < cvGetSize(img).width; x++) {
			bright += cvGet2D(img, y, x).val[0];
			nextBright += cvGet2D(img, y + dy, x).val[0];
		}

		//����� ���� ���� �� ������ ���� ���� ������ ������.
		bright /= cvGetSize(img).width - 1;
		nextBright /= cvGetSize(img).width - 1;

		//���� ���̰� 50���� ũ�ٸ�
		if (abs(bright - nextBright) > 50) {
			//���� ���� y��ǥ�� ����.
			return y;
		}
	}
}

//�� CvScalar������ �����̸� �����ϴ� �Լ�
//�Է����� ������ CvScalar ������ RGB���� ��� ���ٰ� �����Ѵ�.
float getDistance(CvScalar a, CvScalar b) {
	return abs(a.val[0] - b.val[0]);
}
