#include <opencv2/opencv.hpp>

int main()
{
	IplImage* fg = cvLoadImage("c:\\tmp\\sprite.jpg");
	IplImage* fga = cvLoadImage("c:\\tmp\\spritea.jpg");
	IplImage* bg = cvLoadImage("c:\\tmp\\background.jpg");
	IplImage* dst = cvCreateImage(cvGetSize(bg), 8, 3);

	int divX = 6;
	int divY = 3;
	int dw = fg->width / divX;
	int dh = fg->height / divY;


	int ind1 = 0;					// 자세 : 0~5
	int ind2 = 0;					// 방향 : 0 직진, 1: 왼쪽, 2: 오른쪽

	int dx = 0;
	int dy = 0;
	int x0 = 0;
	int y0 = 0;

	while (true)
	{

		cvCopy(bg, dst);
		for (int y = 0; y < dh; y++)
			for (int x = 0; x < dw; x++)
			{
				int x1 = x0 + x;
				int y1 = y0 + y;

				int x2 = ind1 * dw + x;
				int y2 = ind2 * dh + y;

				if (x1<0 || x1>dst->width - 1) continue;
				if (y1<0 || y1>dst->height - 1) continue;
				if (y2<0 || y2>fg->height - 1) continue;
				if (x2<0 || x2>fg->width - 1) continue;

				CvScalar f = cvGet2D(fg, y2, x2);
				CvScalar b = cvGet2D(bg, y1, x1);
				CvScalar a = cvGet2D(fga, y2, x2);
				float alpha = a.val[0] / 255;
				CvScalar g;

				for (int k = 0; k < 3; k++)
					g.val[k] = alpha * f.val[k] + (1 - alpha) * b.val[k];
				cvSet2D(dst, y1, x1, g);
			}

		cvShowImage("dst", dst);
		int key = cvWaitKey(100);

		switch (key)
		{
		case 's':	ind2 = 0;	dx = 0;	dy = 30;	break;
		case 'w':	ind2 = 0;	dx = 0;	dy = -30;	break;
		case 'a':	ind2 = 1;	dx = -30;	dy = 0;	break;
		case 'd':	ind2 = 2;	dx = +30;	dy = 0;	break;
		default:									break;
		}

		ind1++;
		ind1 = ind1 % 6;
		x0 += dx;
		y0 += dy;

	}





	return 0;
}














/*
#include <opencv2/opencv.hpp>

int main()
{
	IplImage* bg = cvLoadImage("c:\\temp\\comp1.jpg");
	IplImage* bga = cvLoadImage("c:\\temp\\comp1a.jpg");
	IplImage* fg = cvLoadImage("c:\\temp\\comp2.jpg");
	IplImage* fga = cvLoadImage("c:\\temp\\comp2a.jpg");

	IplImage* dst = cvCreateImage(cvGetSize(bg), 8, 3);

	float time = 0;
	while (true)
	{
		cvCopy(bg, dst);
		for (int y = 0; y < fg->height; y++)
			for (int x = 0; x < fg->width; x++)
			{
				int x1 = x + 100 * sin(time/360*3.141592);
				int y1 = y + 50 * cos(time / 360 * 3.141592);

				if (x1<0 || x1> bg->width - 1) continue;
				if (y1<0 || y1> bg->height - 1) continue;

				CvScalar b = cvGet2D(bg, y1, x1);
				CvScalar f = cvGet2D(fg, y, x);
				CvScalar a = cvGet2D(fga, y, x);

				float alpha = a.val[0] / 255;
				CvScalar g;
				for (int k = 0; k < 3; k++)
					g.val[k] = alpha * f.val[k] + (1 - alpha) * b.val[k];		// Matt Equation

				CvScalar ba = cvGet2D(bga, y1, x1);
				alpha = ba.val[0] / 255;
				for (int k = 0; k < 3; k++)
					g.val[k] = alpha * b.val[k] + (1 - alpha) * g.val[k];		// Matt Equation


				cvSet2D(dst, y1, x1, g);
			}

		cvShowImage("dst", dst);
		cvWaitKey(1);
		time += 1;
	}

	return 0;
}
*/