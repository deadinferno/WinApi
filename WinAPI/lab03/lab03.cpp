#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <math.h>

struct FERN
{
	int x, y; // координаты лев. верх. угла (местоположение) 
	int h;		// высота листа (размер)
	COLORREF color; // цвет линии (цвет изображения)
};

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

const wchar_t MainWindowClassName[] = L"MainWindow";

// --- Функция WinMain
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
#pragma region Регистрация_класса

	WNDCLASS MainWindowClass; 	// Структура для информации о классе окна
	memset(&MainWindowClass, 0, sizeof(MainWindowClass));
	MainWindowClass.lpszClassName = MainWindowClassName;		// Имя класса окон
	MainWindowClass.lpfnWndProc = (WNDPROC)MainWindowProc;
	// Адрес оконной функции
	MainWindowClass.style = CS_HREDRAW | CS_VREDRAW;	// Стиль класса 
	MainWindowClass.hInstance = hInstance;		// Экземпляр приложения
	MainWindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	// Пиктограмма для окон

	MainWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	// Курсор мыши для окон

	MainWindowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	// Кисть для окон

	MainWindowClass.lpszMenuName = NULL;	// Ресурс меню окон
	MainWindowClass.cbClsExtra = 0;			// Дополнительная память
	MainWindowClass.cbWndExtra = 0;			// Дополнительная память

	// Pегистрация класса окна.
	RegisterClass(&MainWindowClass);

#pragma endregion
#pragma region Создание_окна

	// Создаем главное окно приложения.
	HWND MainWindow; 	// Дескриптор главного окна приложения
	MainWindow = CreateWindow(
		MainWindowClassName, 			// Имя класса окон
		MainWindowClassName,			// Заголовок окна 
		WS_OVERLAPPEDWINDOW, 		// Стиль окна
		256,			// X-координаты 
		256,			// Y-координаты 
		800,			// Ширина окна
		600,			// Высота окна
		NULL,			// Дескриптор окна-родителя
		NULL,			// Дескриптор меню окна
		hInstance,		// Дескриптор экземпляра приложения
		NULL);		// Дополнительная информация
	if (!MainWindow)
	{
		// Окно не создано, выдаем предупреждение.
		MessageBox(NULL,
			L"Create: error", L"AppTittle", MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	// Отображаем окно.
	ShowWindow(MainWindow, nCmdShow);

	// Обновляем содержимое клиентской области окна.
	UpdateWindow(MainWindow);

#pragma endregion

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		// Преобразование некоторых сообщений, 
		// полученных с помощью клавиатуры
		TranslateMessage(&msg);
		// Отправляем сообщение оконной процедуре
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

void draw_fern(HDC hDC, struct FERN frn)
// функция вывода листа папоротника
{
	long MaxIteration = frn.h * 25l; // число итераций
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
		x = frn.x + (int)(x0*frn.h / 11 + frn.h / 4);
		y = frn.y + (int)(y0*(-frn.h) / 11 + frn.h);
		// вывод очередного пикселя (функция SetPixel)
		// нарисовать цветом frn.color пиксель с координатами(x, y)
		SetPixel(hDC, x, y, frn.color);
	}
}

// --- Функция окна
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
	{
		InvalidateRect(hWnd, NULL, TRUE);

		HDC hDC;
		PAINTSTRUCT ps;

		hDC = BeginPaint(hWnd, &ps);

		RECT r;
		GetClientRect(hWnd, &r);

		FERN frn;
		frn.h = (rand() % ((r.bottom - 20) - 20)) + 20;
		frn.x = (rand() % ((r.right - (frn.h / 2)) - 0)) + 0;
		frn.y = (rand() % ((r.bottom - frn.h) - 0)) + 0;
		frn.color = RGB(rand() % 255, rand() % 255, rand() % 255);

		draw_fern(hDC, frn);

		EndPaint(hWnd, &ps);
	}; break;

	case WM_LBUTTONDOWN:
	{
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
	}; break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}; break;

	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0l;
}
