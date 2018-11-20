#pragma once

#define PI					3.14156

struct FLOWER
{
	int N,			// ���������� ��������� (��� ����������)
		x, y,		// ���������� ������ ������ (��������������)
		r; 			// ������ ������, �.�. ����� �������� (������)
	COLORREF color; // ���� ����� (���� ����������)

	FLOWER()
	{
		N = x = y = r = NULL; color = NULL;
	}
	FLOWER(int _N, int _x, int _y, int _r, COLORREF _color)
	{
		N = _N; x = _x; y = _y;	r = _r;	color = _color;
	}
	// ������ �������� ������
	void Draw(HDC hDC)
	{
		double rx, ry, x1, x2, x3, x4, x5, x6,
			y1, y2, y3, y4, y5, y6, f, df;
		rx = this->r / 2.; 								// r - ����� ��������, rx - �� ��������
		x1 = this->x + rx; y1 = this->y;					// x1, y1 - ����� �������� ��������
		ry = 2.*PI*rx*0.7 / (float)this->N; ry = ry / 2.;	// ry - �������� ������ �������� 
		df = 2.*PI / (float)this->N;						// df - ����, ���������� ���������

															// �������� �������� GDI (������ CreatePen)
															///� �������� ��������� ���� ����� this->color �������� 3 ������
		HPEN pen = CreatePen(PS_SOLID, 3, this->color);
		// ��������� ��������� ��������� ���������� (������ SelectObject)
		///� ��������� ���������� ���� � �������� hDC � ���������� "�������" ����
		HGDIOBJ oldPen = SelectObject(hDC, pen);

		for (int i = 0; i < this->N; i++)
			// ������ ��������� �������� ������ � N ���������� ( 0<=i<N)
		{
			// �������� ��������� ������� ������ � ����� 
			// ������ (������ MoveToEx)
			///� ����������� ������� ������� ���� � ����� � ������������(this->x, this->y)
			MoveToEx(hDC, this->x, this->y, NULL);

			f = (float)i*df; 								// f - ���� �������� �� i-���� ��������

															// ����� ��������, ����������� ��������������
															// �������� �� f ������
			for (x2 = -rx; x2 < rx; x2 += 0.5)
				// ����� ������ �������� ��������
			{
				y2 = sqrt((1. - (x2*x2) / (rx*rx))*(ry*ry));
				x3 = x1 + x2; y3 = y1 - y2;
				x4 = x3 - this->x; y4 = y3 - this->y;
				x5 = x4 * cos(f) - y4 * sin(f);
				y5 = x4 * sin(f) + y4 * cos(f);
				x6 = x5 + this->x; y6 = y5 + this->y;

				// �������� ��������� �������� �������
				// �������� (������ LineTo)
				///� �������� ����� �� ������� ������� ���� �� �����(x6, y6)
				LineTo(hDC, x6, y6);
			}
			for (x2 = rx; x2 > -rx; x2 -= 0.5)
				// ����� ������ �������� ��������
			{
				y2 = sqrt((1. - (x2*x2) / (rx*rx))*(ry*ry));
				x3 = x1 + x2; y3 = y1 + y2;
				x4 = x3 - this->x; y4 = y3 - this->y;
				x5 = x4 * cos(f) - y4 * sin(f);
				y5 = x4 * sin(f) + y4 * cos(f);
				x6 = x5 + this->x; y6 = y5 + this->y;

				// �������� ��������� �������� ������� 
				// �������� (������ LineTo)
				///� �������� ����� �� ������� ������� ���� �� �����(x6, y6)
				LineTo(hDC, x6, y6);
			}
			// �������� ��������� �������� ������� �������� (������ LineTo)
			///�������� ����� �� ������� ������� ���� �� �����(this->x, this->y)
			LineTo(hDC, this->x, this->y);
		}
		// �������������� ��������� ��������� ����������
		///� ��������� "�������" ���� � �������� hDC
		SelectObject(hDC, oldPen);
		// �������� �������� �������� GDI
		///� �������� ���������� ����
		DeleteObject(pen);
	}
	// ����� ����������
	void newPos(RECT FAR rw) {
		N = rand() % 21 + 5;
		r = rand() % 101 + 20;
		x = rand() % (rw.right - r * 2) + r;
		y = rand() % (rw.bottom - r * 2) + r;

		if (y < r)				r = rand() % y;
		if (x < r)				r = rand() % x;
		if (rw.bottom - r < r)	r = rand() % (rw.bottom - r - 40) + 20;
		if (rw.bottom - r < r)	r = rand() % (rw.right - r - 40) + 20;

		color = RGB(rand() % 255, rand() % 255, rand() % 255);
	}
};

struct FERN
{
	int x, y,		// ���������� ���. ����. ���� (��������������) 
		h;			// ������ ����� (������)
	COLORREF color; // ���� ����� (���� �����������)
	FERN()
	{
		x = y = h = NULL; color = NULL;
	}
	// ������� ��������� ����� �����������
	void Draw(HDC hDC)
	{
		long MaxIteration = this->h * 100l; // ����� ��������
		static double x0 = 0, y0 = 0, xx, yy, x, y;

		// ������� ������������� ��� ������� ������������ �����������
		double a[4] = { 0.00, 0.85, 0.20,-0.15 };
		double b[4] = { 0.00, 0.04,-0.26, 0.28 };
		double c[4] = { 0.00,-0.04, 0.23, 0.26 };
		double d[4] = { 0.16, 0.85, 0.22, 0.24 };
		double e[4] = { 0.00, 0.00, 0.00, 0.00 };
		double f[4] = { 0.00, 1.60, 1.60, 0.44 };

		for (int i = 0; i<MaxIteration; i++)
			// ������ ��������� ������� (0<=i<MaxIteration)
		{
			// ����������� ���������
			int k = rand() % 100 + 1;
			if (k <= 1) k = 0;
			else	if (k>1 && k <= 86) k = 1;
			else	if (k>86 && k <= 93) k = 2;
			else if (k>93 && k <= 100)
				k = 3;
			xx = a[k] * x0 + b[k] * y0 + e[k];
			yy = c[k] * x0 + d[k] * y0 + f[k];
			x0 = xx; y0 = yy;
			x = this->x + (int)(x0*this->h / 11 + this->h / 4);
			y = this->y + (int)(y0*(-this->h) / 11 + this->h);
			// ����� ���������� ������� (������� SetPixel)
			//� ���������� ������ this->color ������� � ������������(x, y)
			SetPixel(hDC, x, y, this->color);
		}
	}
	// ����� ����������
	void newPos(RECT FAR rw)
	{
		h = rand() % (rw.bottom - 40) + 20;
		x = rand() % (rw.right - h / 2);
		y = rand() % (rw.bottom - h);
		color = RGB(rand() % 255, rand() % 255, rand() % 255);
	}
};

struct SNOWFLAKE
{
	int x, y;			// ���������� ������ (��������������)
	unsigned N,			// ���- �� ������� (�������) (��� ����������)
		P,			// ���-�� ������ � ������ ����� (��� ����������)
		r; 		// ������ �������� (������)
	COLORREF color; 	// ���� ����� (���� ����������)
	SNOWFLAKE()
	{
		x = y = NULL; N = P = r = NULL; color = NULL;
	}
	SNOWFLAKE(int _x, int _y, unsigned _N, unsigned _P, unsigned _r, COLORREF _color)
	{
		x = _x; y = _y; N = _N; P = _P; r = _r; color = _color;
	}
	// ������ �������� ��������
	void Draw(HDC hDC)
	{
		if (this->N>0) // ������ ��������� �������� ( 1<=k<=N)
		{
			// ����� ����� (����)
			int L = (int)(this->r * 3 * exp((this->N - 1)*log(4.)) / (exp(this->N*log(4.)) - 1));
			for (int i = 0; i<this->P; i++)
				// ����� ���� � �� ��� ����� - ����� �������� ����� 
			{
				// �������� �������� GDI (������ CreatePen)
				//� �������� ��������� ���� ����� this->color �������� 1 �������
				HPEN pen = CreatePen(PS_SOLID, 1, this->color);

				// ��������� ��������� ��������� ���������� (������ SelectObject)
				//� ��������� ���������� ���� � �������� hDC � ���������� "�������" ����
				HGDIOBJ oldPen = SelectObject(hDC, pen);

				// ����������� ��������� ��������� ��������
				int x1 = (int)(this->x + L * cos(2 * PI*i / this->P));
				int y1 = (int)(this->y + L * sin(2 * PI*i / this->P));

				// ����� ����� �� (this->x,this->y) �� (x1,y1)
				// (������� MoveToEx � LineTo)
				//� ����������� ������� ������� ���� � ����� � ������������(this->x, this->y)
				MoveToEx(hDC, this->x, this->y, NULL);
				//� �������� ����� �� ������� ������� ���� �� �����(x1, y1)
				LineTo(hDC, x1, y1);
				// �������������� ��������� ��������� ����������
				//� ��������� "�������" ���� � �������� hDC
				SelectObject(hDC, oldPen);
				// �������� �������� �������� GDI
				//� �������� ���������� ����
				DeleteObject(pen);
				// ����� P ������ ����� � ������� N-1 �� i-��� 
				// ����� ����� � ������� N
				struct SNOWFLAKE snowTmp(x1, y1, this->N - 1, this->P, this->r / 4, this->color);
				snowTmp.Draw(hDC);	// ����� ��� ������ ��������
			}
		}
	}
	// ����� ����������
	void newPos(RECT FAR rw)
	{
		N = 4;
		P = 8;
		r = rand() % 201 + 20;
		x = rand() % (rw.right - r * 2) + r;
		y = rand() % (rw.bottom - r * 2) + r;
		color = RGB(rand() % 255, rand() % 255, rand() % 255);
	}
};

struct STAR
{
	int x, y;			// ���������� ������ ������ (��������������)
	unsigned N,			// ���������� ���������� (��� ����������)
		r; 		// ������ ������ (������)
	COLORREF color; 	// ���� ����� (���� ����������)
	STAR()
	{
		x = y = NULL; N = r = NULL; color = NULL;
	}
	// ����� ����, ��������������� ���������
	void ugol(HDC hDC, int x1, int y1, int x2, int y2, int x3, int y3, int n)
	{
		int n1 = n + 1; double a; int xx1, yy1, xx2, yy2;
		for (int i = 1; i <= n1; i++)
		{
			a = (i - 1.) / n;
			xx1 = x1 + (x2 - x1)*a;
			yy1 = y1 + (y2 - y1)*a;
			xx2 = x2 + (x3 - x2)*a;
			yy2 = y2 + (y3 - y2)*a;

			// ����� ��������� ����� �� (xx1,yy1) �� (xx2,yy2)
			// (������� MoveToEx � LineTo)
			//� ����������� ������� ������� ���� � ����� � ������������(xx1, yy1)
			MoveToEx(hDC, xx1, yy1, NULL);
			//� �������� ����� �� ������� ������� ���� �� �����(xx2, yy2)
			LineTo(hDC, xx2, yy2);
		}
	}
	// ������ �������� ������
	void Draw(HDC hDC)
	{
		int rc = this->r / 3; // ������ ������ ������
		double a1, a2, a3, d = 2.*PI / (this->N * 2);

		// �������� �������� GDI (������ CreatePen)
		//� �������� ��������� ���� ����� this->color �������� 3 ������
		HPEN pen = CreatePen(PS_SOLID, 3, this->color);
		// ��������� ��������� ��������� ���������� (������ SelectObject)
		//� ��������� ���������� ���� � �������� hDC � ���������� "�������" ����
		HGDIOBJ oldPen = SelectObject(hDC, pen);

		for (int i = 1; i <= this->N; i++)
			// ������ ��������� �������� ������ ( 1<=i<=N)
		{
			// ����������� �������� �����
			a1 = 2 * PI*(i - 1) / this->N;
			a2 = a1 + d; a3 = a1 - d;

			// ����� ����, ��������������� ���������
			ugol(hDC, this->x, this->y, this->x + this->r*sin(a1),
				this->y + this->r*cos(a1), this->x + rc * sin(a2), this->y + rc * cos(a2), 25);
			ugol(hDC, this->x, this->y, this->x + this->r*sin(a1), this->y + this->r*cos(a1),
				this->x + rc * sin(a3), this->y + rc * cos(a3), 25);
		}
		// �������������� ��������� ��������� ����������
		///� ��������� "�������" ���� � �������� hDC
		SelectObject(hDC, oldPen);
		// �������� �������� �������� GDI
		///� �������� ���������� ����
		DeleteObject(pen);
	}
	// ����� ����������
	void newPos(RECT FAR rw)
	{
		N = rand() % 16 + 5;
		r = rand() % 201 + 20;
		x = rand() % (rw.right - r);
		y = rand() % (rw.bottom - r);

		if (y < r)				r = rand() % y;
		if (x < r)				r = rand() % x;
		if (rw.bottom - r < r)	r = rand() % (rw.bottom - r - 40) + 20;
		if (rw.bottom - r < r)	r = rand() % (rw.right - r - 40) + 20;

		color = RGB(rand() % 255, rand() % 255, rand() % 255);
	}
};

struct TREE
{
	int f,			// ��������� ���� ������� (��� ����������)
		x, y,		// ���������� ���� ������ (��������������)
		h;			// ������ ��������� ������ ������ (������)
	COLORREF color; // ���� ����������
	TREE()
	{
		f = x = y = h = NULL; color = NULL;
	}

	// ����� �����, �������� �� �������
	void ugolok(HDC hDC, int *X[], int *Y[], int *UG[], int *N, int dl, int ug0, int ug, int *X0, int *Y0, int *i)
	{
		int x0 = *X0, y0 = *Y0, x1, y1, w; double grad = PI / 180.;
		if (*i == 0) w = 1; else w = 2;
		for (int a = 1; a <= w; a++)
		{
			(*i)++;
			if ((*i) + 1>*N)
				// ���������� �������� �������������� �����
			{
				*N = (*i) + 1;
				*X = (int*)realloc(*X, (*N) * sizeof(int));
				*Y = (int*)realloc(*Y, (*N) * sizeof(int));
				*UG = (int*)realloc(*UG, (*N) * sizeof(int));
			}
			(*UG)[*i] = ug + ug0;
			(*X)[*i] = x1 = x0 + cos(ug*grad)*dl;
			(*Y)[*i] = y1 = y0 - sin(ug*grad)*dl;

			// ����� ��������� ����� �� (x0,y0) �� (x1,y1)
			// (������� MoveToEx � LineTo)
			//� ����������� ������� ������� ���� � ����� � ������������(x0, y0)
			MoveToEx(hDC, x0, y0, NULL);
			//� �������� ����� �� ������� ������� ���� �� �����(x1, y1)
			LineTo(hDC, x1, y1);
			ug -= ug0 * 2 + (rand() % 26 - 15);
		}
	}
	// ������ �������� ������
	void Draw(HDC hDC) // ������ ������ ������
	{
		int oldh = h;
		int x0, y0, *X, *Y, ug, i, k, *UG, ug0, m, n, kvet, hvet, N;
		i = 0; k = 0; m = 1; ug0 = 28 + this->f; // �������������
		N = (k + 1);
		// ������� ����� ��������� �������� - �������� ��������� � �����
		X = (int*)malloc(N * sizeof(int));
		Y = (int*)malloc(N * sizeof(int));
		UG = (int*)malloc(N * sizeof(int));

		// �������� �������� GDI (������ CreatePen)
		//� �������� ��������� ���� ����� this->color �������� 1 ������
		HPEN pen = CreatePen(PS_SOLID, 3, this->color);

		// ��������� ��������� ��������� ���������� (������ SelectObject)
		//� ��������� ���������� ���� � �������� hDC � ���������� "�������" ����
		HGDIOBJ oldPen = SelectObject(hDC, pen);

		// ����� ������
		ug = UG[k] = ug0 * 3.2;
		x0 = X[k] = this->x; 	y0 = Y[k] = this->y;
		ugolok(hDC, &X, &Y, &UG, &N, this->h, ug0, ug, &x0, &y0, &i);

		do // ������ ���� ������
		{
			hvet = rand() % 2 + 2; //�������� ��������� �����
			if (hvet == 2) kvet = 128 * 2; else kvet = 64 * 2;
			if (this->h>hvet) this->h -= hvet; else break;
			for (n = 1; n <= m; n++)
			{
				k++;
				ug = UG[k] + this->f;
				x0 = X[k]; y0 = Y[k];
				ugolok(hDC, &X, &Y, &UG, &N, this->h, ug0, ug, &x0, &y0, &i);
			}
			m = m * 2;
		} while (m <= kvet);

		// �������������� ��������� ��������� ����������
		//� ��������� "�������" ���� � �������� hDC
		SelectObject(hDC, oldPen);

		// �������� �������� �������� GDI
		//� �������� ���������� ����
		DeleteObject(pen);

		free(X); free(Y); free(UG);
		h = oldh;
	}
	// ����� ����������
	void newPos(RECT FAR rw)
	{
		h = rand() % 21 + 20;
		x = rand() % (rw.right - 6 * h) + 6 * h;
		y = rand() % (rw.bottom - 6 * h) + 6 * h;
		color = RGB(rand() % 255, rand() % 255, rand() % 255);
	}
};

struct UZOR
{
	int x, y;						// ���������� ���. ����. ���� ���������� 
	unsigned C1, A3, A4, B3, B4,	// ������������ 
		w, h;					// ������ � ������ ������� (������)
	COLORREF color;					// ���� ����� (���� ����������)
	UZOR()
	{
		x = y = C1 = A3 = A4 = B3 = B4 = w = h = NULL; color = NULL;
	}
	// ������ �������� �����
	void Draw(HDC hDC) // ������ ������ �����
	{
		int dx = this->w / 2, dy = this->h / 2, ddx = dx / 4, ddy = dy / 4, x1, x2, y1, y2;
		double D, T = 5;
		COLORREF grey = RGB(127, 127, 127);
		// ����� ���� ��������� � ������ ����������.
		int step = 1;
		// �������� �������� GDI (������ CreatePen)
		//� �������� ��������� ���� ����� this->color �������� 3 ������
		HPEN pen = CreatePen(PS_SOLID, 3, this->color);
		//� �������� ��������� ���� ����� grey �������� 3 ������
		HPEN pen_gray = CreatePen(PS_SOLID, 3, COLOR_GRAYTEXT);

		for (double t = 0; t<T; t += 0.01) // ������ ��������� �������� ����� t ( 0<=T<5)
		{
			HGDIOBJ oldPen;
			// ��������� ��������� ��������� ���������� (������ SelectObject)
			if (step)
			{
				oldPen = SelectObject(hDC, pen);
				//� ��������� ���� ����� ���������� � �������� hDC
				//� ���������� "�������" ����
			}
			else
			{
				//� ��������� ���� ������ ����� � �������� hDC
				//� ���������� "�������" ����
				oldPen = SelectObject(hDC, pen_gray);
			}
			step = !step;

			// ����������� ��������� ������ � ����� ��������� ����� �����
			D = t + this->C1;
			x1 = (int)((2 * cos(this->A4*t) + 2 * cos(this->A3 * 2 * t))*ddx);
			y1 = (int)((2 * cos(this->B4*t) + 2 * cos(this->B3 * 2 * t))*ddy);
			x2 = (int)((2 * cos(this->A4*D) + 2 * cos(this->A3 * 2 * D))*ddx);
			y2 = (int)((2 * cos(this->B4*D) + 2 * cos(this->B3 * 2 * D))*ddy);
			x1 = this->x + (x1 + dx); y1 = this->y + (y1 + dy);
			x2 = this->x + (x2 + dx); y2 = this->y + (y2 + dy);
			// ����� ��������� ����� �� (x1,y1) �� (x2,y2)
			// (������� MoveToEx � LineTo)
			//� ����������� ������� ������� ���� � ����� � ������������(x1, y1)
			MoveToEx(hDC, x1, y1, NULL);
			//� �������� ����� �� ������� ������� ���� �� �����(x2, y2)
			LineTo(hDC, x2, y2);
			// �������������� ��������� ��������� ����������
			//� ��������� "�������" ���� � �������� hDC
			SelectObject(hDC, oldPen);
		}

		// �������� ��������� �������� GDI
		//� �������� ���� ��������� ������
		DeleteObject(pen);
		DeleteObject(pen_gray);
	}
	// ����� ����������
	void newPos(RECT FAR rw)
	{
		A4 = rand() % 6;
		A3 = rand() % 6;
		B4 = rand() % 6;
		B3 = rand() % 6;
		C1 = rand() % 6;
		w = rand() % (rw.right - 40) + 20;
		h = rand() % (rw.bottom - 40) + 20;
		x = rand() % (rw.right - w);
		y = rand() % (rw.bottom - h);
		color = RGB(rand() % 255, rand() % 255, rand() % 255);
	}
};