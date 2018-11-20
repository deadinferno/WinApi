#pragma once

#define PI					3.14156

struct FLOWER
{
	int N,			// количество лепестков (вид изображени¤)
		x, y,		// координаты центра цветка (местоположение)
		r; 			// радиус цветка, т.е. длина лепестка (размер)
	COLORREF color; // цвет линии (цвет изображени¤)

	FLOWER()
	{
		N = x = y = r = NULL; color = NULL;
	}
	FLOWER(int _N, int _x, int _y, int _r, COLORREF _color)
	{
		N = _N; x = _x; y = _y;	r = _r;	color = _color;
	}
	// ‘ункци¤ рисовани¤ цветка
	void Draw(HDC hDC)
	{
		double rx, ry, x1, x2, x3, x4, x5, x6,
			y1, y2, y3, y4, y5, y6, f, df;
		rx = this->r / 2.; 								// r - длина лепестка, rx - ее половина
		x1 = this->x + rx; y1 = this->y;					// x1, y1 - центр нулевого лепестка
		ry = 2.*PI*rx*0.7 / (float)this->N; ry = ry / 2.;	// ry - половина ширины лепестка 
		df = 2.*PI / (float)this->N;						// df - угол, занимаемый лепестком

															// создание объектов GDI (функци¤ CreatePen)
															///Е создание сплошного пера цвета this->color толщиной 3 пиксел¤
		HPEN pen = CreatePen(PS_SOLID, 3, this->color);
		// установка атрибутов контекста отображени¤ (функци¤ SelectObject)
		///Е установка созданного пера в контекст hDC и сохранение "старого" пера
		HGDIOBJ oldPen = SelectObject(hDC, pen);

		for (int i = 0; i < this->N; i++)
			// рисуем очередной лепесток цветка с N лепестками ( 0<=i<N)
		{
			// сдвинуть начальную позицию вывода в центр 
			// цветка (функци¤ MoveToEx)
			///Е передвинуть текущую позицию пера в точку с координатами(this->x, this->y)
			MoveToEx(hDC, this->x, this->y, NULL);

			f = (float)i*df; 								// f - угол поворота дл¤ i-того лепестка

															// вывод лепестка, повернутого относительного
															// нулевого на f радиан
			for (x2 = -rx; x2 < rx; x2 += 0.5)
				// вывод первой половины лепестка
			{
				y2 = sqrt((1. - (x2*x2) / (rx*rx))*(ry*ry));
				x3 = x1 + x2; y3 = y1 - y2;
				x4 = x3 - this->x; y4 = y3 - this->y;
				x5 = x4 * cos(f) - y4 * sin(f);
				y5 = x4 * sin(f) + y4 * cos(f);
				x6 = x5 + this->x; y6 = y5 + this->y;

				// нарисуем очередной фрагмент контура
				// лепестка (функци¤ LineTo)
				///Е провести линию от текущей позиции пера до точки(x6, y6)
				LineTo(hDC, x6, y6);
			}
			for (x2 = rx; x2 > -rx; x2 -= 0.5)
				// вывод второй половины лепестка
			{
				y2 = sqrt((1. - (x2*x2) / (rx*rx))*(ry*ry));
				x3 = x1 + x2; y3 = y1 + y2;
				x4 = x3 - this->x; y4 = y3 - this->y;
				x5 = x4 * cos(f) - y4 * sin(f);
				y5 = x4 * sin(f) + y4 * cos(f);
				x6 = x5 + this->x; y6 = y5 + this->y;

				// нарисуем очередной фрагмент контура 
				// лепестка (функци¤ LineTo)
				///Е провести линию от текущей позиции пера до точки(x6, y6)
				LineTo(hDC, x6, y6);
			}
			// нарисуем очередной фрагмент контура лепестка (функци¤ LineTo)
			///провести линию от текущей позиции пера до точки(this->x, this->y)
			LineTo(hDC, this->x, this->y);
		}
		// восстановление атрибутов контекста отображени¤
		///Е установка "старого" пера в контекст hDC
		SelectObject(hDC, oldPen);
		// удаление созданых объектов GDI
		///Е удаление созданного пера
		DeleteObject(pen);
	}
	// Ќовые координаты
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
	int x, y,		// координаты лев. верх. угла (местоположение) 
		h;			// высота листа (размер)
	COLORREF color; // цвет линии (цвет изображения)
	FERN()
	{
		x = y = h = NULL; color = NULL;
	}
	// Функция рисования листа папоротника
	void Draw(HDC hDC)
	{
		long MaxIteration = this->h * 100l; // число итераций
		static double x0 = 0, y0 = 0, xx, yy, x, y;

		// таблица коэффициентов для данного фрактального изображения
		double a[4] = { 0.00, 0.85, 0.20,-0.15 };
		double b[4] = { 0.00, 0.04,-0.26, 0.28 };
		double c[4] = { 0.00,-0.04, 0.23, 0.26 };
		double d[4] = { 0.16, 0.85, 0.22, 0.24 };
		double e[4] = { 0.00, 0.00, 0.00, 0.00 };
		double f[4] = { 0.00, 1.60, 1.60, 0.44 };

		for (int i = 0; i<MaxIteration; i++)
			// рисуем очередной пиксель (0<=i<MaxIteration)
		{
			// определение координат
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
			// вывод очередного пикселя (функция SetPixel)
			//… нарисовать цветом this->color пиксель с координатами(x, y)
			SetPixel(hDC, x, y, this->color);
		}
	}
	// Новые координаты
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
	int x, y;			// координаты центра (местоположение)
	unsigned N,			// кол- во звеньев (уровней) (вид изображени¤)
		P,			// кол-во ветвей в каждом звене (вид изображени¤)
		r; 		// радиус снежинки (размер)
	COLORREF color; 	// цвет линии (цвет изображени¤)
	SNOWFLAKE()
	{
		x = y = NULL; N = P = r = NULL; color = NULL;
	}
	SNOWFLAKE(int _x, int _y, unsigned _N, unsigned _P, unsigned _r, COLORREF _color)
	{
		x = _x; y = _y; N = _N; P = _P; r = _r; color = _color;
	}
	// ‘ункци¤ рисовани¤ снежинки
	void Draw(HDC hDC)
	{
		if (this->N>0) // рисуем очередной фрагмент ( 1<=k<=N)
		{
			// длина ветви (луча)
			int L = (int)(this->r * 3 * exp((this->N - 1)*log(4.)) / (exp(this->N*log(4.)) - 1));
			for (int i = 0; i<this->P; i++)
				// вывод луча и на его конце - лучей меньшего уровн¤ 
			{
				// создание объектов GDI (функци¤ CreatePen)
				//Е создание сплошного пера цвета this->color толщиной 1 пиксель
				HPEN pen = CreatePen(PS_SOLID, 1, this->color);

				// установка атрибутов контекста отображени¤ (функци¤ SelectObject)
				//Е установка созданного пера в контекст hDC и сохранение "старого" пера
				HGDIOBJ oldPen = SelectObject(hDC, pen);

				// определение координат очередной развилки
				int x1 = (int)(this->x + L * cos(2 * PI*i / this->P));
				int y1 = (int)(this->y + L * sin(2 * PI*i / this->P));

				// вывод линии от (this->x,this->y) до (x1,y1)
				// (функции MoveToEx и LineTo)
				//Е передвинуть текущую позицию пера в точку с координатами(this->x, this->y)
				MoveToEx(hDC, this->x, this->y, NULL);
				//Е провести линию от текущей позиции пера до точки(x1, y1)
				LineTo(hDC, x1, y1);
				// восстановление атрибутов контекста отображени¤
				//Е установка "старого" пера в контекст hDC
				SelectObject(hDC, oldPen);
				// удаление созданых объектов GDI
				//Е удаление созданного пера
				DeleteObject(pen);
				// вывод P ветвей звена с номером N-1 на i-той 
				// ветви звена с номером N
				struct SNOWFLAKE snowTmp(x1, y1, this->N - 1, this->P, this->r / 4, this->color);
				snowTmp.Draw(hDC);	// вывод при помощи рекурсии
			}
		}
	}
	// Ќовые координаты
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
	int x, y;			// координаты центра звезды (местоположение)
	unsigned N,			// количество фрагментов (вид изображени¤)
		r; 		// радиус звезды (размер)
	COLORREF color; 	// цвет линии (цвет изображени¤)
	STAR()
	{
		x = y = NULL; N = r = NULL; color = NULL;
	}
	// ¬ывод угла, заштрихованного отрезками
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

			// вывод очередной линии от (xx1,yy1) до (xx2,yy2)
			// (функции MoveToEx и LineTo)
			//Е передвинуть текущую позицию пера в точку с координатами(xx1, yy1)
			MoveToEx(hDC, xx1, yy1, NULL);
			//Е провести линию от текущей позиции пера до точки(xx2, yy2)
			LineTo(hDC, xx2, yy2);
		}
	}
	// ‘ункци¤ рисовани¤ звезды
	void Draw(HDC hDC)
	{
		int rc = this->r / 3; // радиус центра звезды
		double a1, a2, a3, d = 2.*PI / (this->N * 2);

		// создание объектов GDI (функци¤ CreatePen)
		//Е создание сплошного пера цвета this->color толщиной 3 пиксел¤
		HPEN pen = CreatePen(PS_SOLID, 3, this->color);
		// установка атрибутов контекста отображени¤ (функци¤ SelectObject)
		//Е установка созданного пера в контекст hDC и сохранение "старого" пера
		HGDIOBJ oldPen = SelectObject(hDC, pen);

		for (int i = 1; i <= this->N; i++)
			// рисуем очередной фрагмент звезды ( 1<=i<=N)
		{
			// определение значений углов
			a1 = 2 * PI*(i - 1) / this->N;
			a2 = a1 + d; a3 = a1 - d;

			// вывод угла, заштрихованного отрезками
			ugol(hDC, this->x, this->y, this->x + this->r*sin(a1),
				this->y + this->r*cos(a1), this->x + rc * sin(a2), this->y + rc * cos(a2), 25);
			ugol(hDC, this->x, this->y, this->x + this->r*sin(a1), this->y + this->r*cos(a1),
				this->x + rc * sin(a3), this->y + rc * cos(a3), 25);
		}
		// восстановление атрибутов контекста отображени¤
		///Е установка "старого" пера в контекст hDC
		SelectObject(hDC, oldPen);
		// удаление созданых объектов GDI
		///Е удаление созданного пера
		DeleteObject(pen);
	}
	// Ќовые координаты
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
	int f,			// определ¤ет угол наклона (вид изображени¤)
		x, y,		// координаты корн¤ дерева (местоположение)
		h;			// высота основного ствола дерева (размер)
	COLORREF color; // цвет изображени¤
	TREE()
	{
		f = x = y = h = NULL; color = NULL;
	}

	// ¬ывод веток, выход¤щих из вершины
	void ugolok(HDC hDC, int *X[], int *Y[], int *UG[], int *N, int dl, int ug0, int ug, int *X0, int *Y0, int *i)
	{
		int x0 = *X0, y0 = *Y0, x1, y1, w; double grad = PI / 180.;
		if (*i == 0) w = 1; else w = 2;
		for (int a = 1; a <= w; a++)
		{
			(*i)++;
			if ((*i) + 1>*N)
				// необходимо выделить дополнительную пам¤ть
			{
				*N = (*i) + 1;
				*X = (int*)realloc(*X, (*N) * sizeof(int));
				*Y = (int*)realloc(*Y, (*N) * sizeof(int));
				*UG = (int*)realloc(*UG, (*N) * sizeof(int));
			}
			(*UG)[*i] = ug + ug0;
			(*X)[*i] = x1 = x0 + cos(ug*grad)*dl;
			(*Y)[*i] = y1 = y0 - sin(ug*grad)*dl;

			// вывод очередной линии от (x0,y0) до (x1,y1)
			// (функции MoveToEx и LineTo)
			//Е передвинуть текущую позицию пера в точку с координатами(x0, y0)
			MoveToEx(hDC, x0, y0, NULL);
			//Е провести линию от текущей позиции пера до точки(x1, y1)
			LineTo(hDC, x1, y1);
			ug -= ug0 * 2 + (rand() % 26 - 15);
		}
	}
	// ‘ункци¤ рисовани¤ дерева
	void Draw(HDC hDC) // функци¤ вывода дерева
	{
		int oldh = h;
		int x0, y0, *X, *Y, ug, i, k, *UG, ug0, m, n, kvet, hvet, N;
		i = 0; k = 0; m = 1; ug0 = 28 + this->f; // инициализации
		N = (k + 1);
		// текущее число элементов массивов - хранилищ координат и углов
		X = (int*)malloc(N * sizeof(int));
		Y = (int*)malloc(N * sizeof(int));
		UG = (int*)malloc(N * sizeof(int));

		// создание объектов GDI (функци¤ CreatePen)
		//Е создание сплошного пера цвета this->color толщиной 1 пиксел
		HPEN pen = CreatePen(PS_SOLID, 3, this->color);

		// установка атрибутов контекста отображени¤ (функци¤ SelectObject)
		//Е установка созданного пера в контекст hDC и сохранение "старого" пера
		HGDIOBJ oldPen = SelectObject(hDC, pen);

		// ствол дерева
		ug = UG[k] = ug0 * 3.2;
		x0 = X[k] = this->x; 	y0 = Y[k] = this->y;
		ugolok(hDC, &X, &Y, &UG, &N, this->h, ug0, ug, &x0, &y0, &i);

		do // рисуем само дерево
		{
			hvet = rand() % 2 + 2; //параметр уменьшени¤ ветки
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

		// восстановление атрибутов контекста отображени¤
		//Е установка "старого" пера в контекст hDC
		SelectObject(hDC, oldPen);

		// удаление созданых объектов GDI
		//Е удаление созданного пера
		DeleteObject(pen);

		free(X); free(Y); free(UG);
		h = oldh;
	}
	// Ќовые координаты
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
	int x, y;						// координаты лев. верх. угла изображени¤ 
	unsigned C1, A3, A4, B3, B4,	// коэффициенты 
		w, h;					// ширина и высота рисунка (размер)
	COLORREF color;					// цвет линии (цвет изображени¤)
	UZOR()
	{
		x = y = C1 = A3 = A4 = B3 = B4 = w = h = NULL; color = NULL;
	}
	// ‘ункци¤ рисовани¤ узора
	void Draw(HDC hDC) // функци¤ вывода узора
	{
		int dx = this->w / 2, dy = this->h / 2, ddx = dx / 4, ddy = dy / 4, x1, x2, y1, y2;
		double D, T = 5;
		COLORREF grey = RGB(127, 127, 127);
		// серый цвет чередуетс¤ с цветом изображени¤.
		int step = 1;
		// создание объектов GDI (функци¤ CreatePen)
		//Е создание сплошного пера цвета this->color толщиной 3 пиксел¤
		HPEN pen = CreatePen(PS_SOLID, 3, this->color);
		//Е создание сплошного пера цвета grey толщиной 3 пиксел¤
		HPEN pen_gray = CreatePen(PS_SOLID, 3, COLOR_GRAYTEXT);

		for (double t = 0; t<T; t += 0.01) // рисуем очередной фрагмент узора t ( 0<=T<5)
		{
			HGDIOBJ oldPen;
			// установка атрибутов контекста отображени¤ (функци¤ SelectObject)
			if (step)
			{
				oldPen = SelectObject(hDC, pen);
				//Е установка пера цвета изображени¤ в контекст hDC
				//и сохранение "старого" пера
			}
			else
			{
				//Е установка пера серого цвета в контекст hDC
				//и сохранение "старого" пера
				oldPen = SelectObject(hDC, pen_gray);
			}
			step = !step;

			// определение координат начала и конца очередной линии узора
			D = t + this->C1;
			x1 = (int)((2 * cos(this->A4*t) + 2 * cos(this->A3 * 2 * t))*ddx);
			y1 = (int)((2 * cos(this->B4*t) + 2 * cos(this->B3 * 2 * t))*ddy);
			x2 = (int)((2 * cos(this->A4*D) + 2 * cos(this->A3 * 2 * D))*ddx);
			y2 = (int)((2 * cos(this->B4*D) + 2 * cos(this->B3 * 2 * D))*ddy);
			x1 = this->x + (x1 + dx); y1 = this->y + (y1 + dy);
			x2 = this->x + (x2 + dx); y2 = this->y + (y2 + dy);
			// вывод очередной линии от (x1,y1) до (x2,y2)
			// (функции MoveToEx и LineTo)
			//Е передвинуть текущую позицию пера в точку с координатами(x1, y1)
			MoveToEx(hDC, x1, y1, NULL);
			//Е провести линию от текущей позиции пера до точки(x2, y2)
			LineTo(hDC, x2, y2);
			// восстановление атрибутов контекста отображени¤
			//Е установка "старого" пера в контекст hDC
			SelectObject(hDC, oldPen);
		}

		// удаление созданных объектов GDI
		//Е удаление двух созданных перьев
		DeleteObject(pen);
		DeleteObject(pen_gray);
	}
	// Ќовые координаты
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