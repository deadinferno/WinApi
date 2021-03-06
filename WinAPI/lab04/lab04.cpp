#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "resource.h"
#include "figures.h"

#pragma region defines
#define W_WIDTH 800
#define W_HEIGHT 600
#define PIC_FLOWER			1
#define PIC_UZOR			2
#define PIC_STAR			3
#define PIC_FERN			4
#define PIC_SNOWFLAKE		5
#define PIC_TREE			6
#define TIMER_ID	64
#pragma endregion

struct Timer
{
	HWND hwnd;		// Дескриптор окна
	int nTimer;		// Номер таймера
	int dTimer;		// Время простоя, мс

	void Init(HWND hWnd, int id, int milliseconds)
	{
		hwnd = hWnd;							// Дескриптор окна
		nTimer = id;							// Номер таймера
		dTimer = milliseconds;					// Время простоя, мс
		SetTimer(hWnd, nTimer, dTimer, NULL);	// Инициализация таймера
	}
	void Update(int new_time)
	{
		if (new_time <= 0)
			return;
		Kill();							// Уничтожаем таймер
		Init(hwnd, nTimer, new_time);	// Создаем новый
	}
	void Kill()
	{
		KillTimer(hwnd, nTimer);
	}
};

int pic = NULL;
Timer timer;

FLOWER		flw;
UZOR		uzr;
STAR		str;
FERN		frn;
SNOWFLAKE	snow;
TREE		tr;

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
#pragma region Объявления/Инициализации/Подгрузка
	WNDCLASS MainWindowClass; 	// Структура для информации о классе окна
	HWND MainWindow; 	// Дескриптор главного окна приложения
	HMENU hMenu;
	MSG msg;
	TCHAR Title[255]; // Заголовок

	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1)); // Загрузка таблицы акселераторов
	LoadString(hInstance, IDS_STRING103, Title, sizeof(Title) / sizeof(TCHAR)); // Загрузка заголовка

	if ((MainWindow = FindWindow(MainWindowClassName, NULL)) != NULL)
	{
		if (IsIconic(MainWindow)) ShowWindow(MainWindow, SW_RESTORE);
		SetForegroundWindow(MainWindow);
		return FALSE;
	}
#pragma endregion
#pragma region Регистрация_класса

	memset(&MainWindowClass, 0, sizeof(MainWindowClass));
	MainWindowClass.lpszClassName = MainWindowClassName;		// Имя класса окон
	MainWindowClass.lpfnWndProc = (WNDPROC)MainWindowProc;	// Адрес оконной функции
	MainWindowClass.style = CS_HREDRAW | CS_VREDRAW;	// Стиль класса 
	MainWindowClass.hInstance = hInstance;		// Экземпляр приложения
	MainWindowClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)); // Пиктограмма для окон
	MainWindowClass.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));	// Курсор мыши для окон
	MainWindowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// Кисть для фона
	MainWindowClass.lpszMenuName = NULL;	// Ресурс меню окон
	MainWindowClass.cbClsExtra = 0;			// Дополнительная память
	MainWindowClass.cbWndExtra = 0;			// Дополнительная память

	// Pегистрация класса окна.
	RegisterClass(&MainWindowClass);

#pragma endregion
#pragma region Создание_окна

	// Создаем главное окно приложения.
	MainWindow = CreateWindow(
		MainWindowClassName, 			// Имя класса окон
		Title,			// Заголовок окна 
		WS_OVERLAPPEDWINDOW, 		// Стиль окна
		256,			// X-координаты 
		256,			// Y-координаты 
		W_WIDTH,			// Ширина окна
		W_HEIGHT,			// Высота окна
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
#pragma region Добавление_меню

	hMenu = CreateMenu();
	AppendMenu(hMenu, MF_STRING, PIC_FLOWER, L"Flower");
	AppendMenu(hMenu, MF_STRING, PIC_UZOR, L"Uzor");
	AppendMenu(hMenu, MF_STRING, PIC_STAR, L"Star");
	AppendMenu(hMenu, MF_STRING, PIC_FERN, L"Fern");
	AppendMenu(hMenu, MF_STRING, PIC_SNOWFLAKE, L"Snowflake");
	AppendMenu(hMenu, MF_STRING, PIC_TREE, L"Tree");
	SetMenu(MainWindow, hMenu);

#pragma endregion
#pragma region Loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!hAccel || !TranslateAccelerator(MainWindow, hAccel, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}
#pragma endregion

// --- Функция окна
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	RECT FAR rw;				// Характеристики внутренней (рабочей) области окна
	GetClientRect(hWnd, &rw);	// Определение координат внутренней области окна, относительно левого верхнего угла внутренней области окна

	switch (msg)
	{
	case WM_CREATE:
	{
		srand(time(0));

		pic = PIC_TREE;

		flw.newPos(rw);
		uzr.newPos(rw);
		str.newPos(rw);
		frn.newPos(rw);
		snow.newPos(rw);
		tr.newPos(rw);

		timer.Init(hWnd, TIMER_ID, 500);
	}; break;
	case WM_PAINT:
	{
		if (pic != NULL)
		{
			HDC hDC;
			PAINTSTRUCT ps;
			hDC = BeginPaint(hWnd, &ps);	// Получить контекст окна

			switch (pic)
			{
			case PIC_FLOWER:		flw.Draw(hDC);	break;
			case PIC_UZOR:			uzr.Draw(hDC);	break;
			case PIC_STAR:			str.Draw(hDC);	break;
			case PIC_FERN:			frn.Draw(hDC);	break;
			case PIC_SNOWFLAKE:		snow.Draw(hDC);	break;
			case PIC_TREE:			tr.Draw(hDC);	break;
			}

			EndPaint(hWnd, &ps);			// Освободить контекст окна
		}
	}; break;

	case WM_TIMER:
	{
		//wParam - идентификатор таймера
		//lParam - адрес функции
		switch (wParam)
		{
		case TIMER_ID:
		{
			// Обновляем координаты и цвет
			switch (pic)
			{
			case PIC_FLOWER:
			{
				flw.color = RGB(rand() % 255, rand() % 255, rand() % 255);

				flw.x = rand() % (rw.right - flw.r * 2) + flw.r;
				flw.y = rand() % (rw.bottom - flw.r * 2) + flw.r;

				if (flw.y < flw.r)				flw.r = rand() % flw.y;
				if (flw.x < flw.r)				flw.r = rand() % flw.x;
				if (rw.bottom - flw.y < flw.r)	flw.r = rand() % (rw.bottom - flw.y);
				if (rw.right - flw.x < flw.r)	flw.r = rand() % (rw.right - flw.x);

			}; break;

			case PIC_UZOR:
			{
				uzr.color = RGB(rand() % 255, rand() % 255, rand() % 255);

				uzr.x = rand() % (rw.right - uzr.w);
				uzr.y = rand() % (rw.bottom - uzr.h);

				if (rw.bottom - uzr.y < uzr.h)	uzr.h = rand() % (rw.bottom - uzr.y);
				if (rw.right - uzr.x < uzr.w)	uzr.w = rand() % (rw.right - uzr.x);

			}; break;

			case PIC_STAR:
			{
				str.color = RGB(rand() % 255, rand() % 255, rand() % 255);

				str.x = rand() % (rw.right - str.r);
				str.y = rand() % (rw.bottom - str.r);

				if (str.y < str.r)				str.r = rand() % str.y;
				if (str.x < str.r)				str.r = rand() % str.x;
				if (rw.bottom - str.y < str.r)	str.r = rand() % (rw.bottom - str.y);
				if (rw.right - str.x < str.r)	str.r = rand() % (rw.right - str.x);

			}; break;

			case PIC_FERN:
			{
				frn.color = RGB(rand() % 255, rand() % 255, rand() % 255);

				frn.x = rand() % (rw.right - frn.h / 2);
				frn.y = rand() % (rw.bottom - frn.h);

				if (rw.bottom - frn.y < frn.h)		frn.h = rand() % (rw.bottom - frn.y);
				if (rw.right - frn.x < frn.h / 2)	frn.h = rand() % (rw.right - frn.x) - frn.h / 2;

			}; break;

			case PIC_SNOWFLAKE:
			{
				snow.color = RGB(rand() % 255, rand() % 255, rand() % 255);

				snow.x = rand() % (rw.right - snow.r * 2) + snow.r;
				snow.y = rand() % (rw.bottom - snow.r * 2) + snow.r;

				if (snow.y < snow.r)				snow.r = rand() % snow.y;
				if (snow.x < snow.r)				snow.r = rand() % snow.x;
				if (rw.bottom - snow.y < snow.r)	snow.r = rand() % (rw.bottom - snow.y);
				if (rw.right - snow.x < snow.r)		snow.r = rand() % (rw.right - snow.x);

			}; break;

			case PIC_TREE:
			{
				tr.color = RGB(rand() % 255, rand() % 255, rand() % 255);

				tr.x = rand() % (rw.right - 11 * tr.h) + 8 * tr.h;
				tr.y = rand() % (rw.bottom - 8 * tr.h) + 8 * tr.h;

			}; break;
			}

			// Перерисовываем
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
		}; break;
		}

	}; break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))	// Меню
		{
		case PIC_FLOWER:		pic = PIC_FLOWER;		break;
		case PIC_UZOR:			pic = PIC_UZOR;			break;
		case PIC_STAR:			pic = PIC_STAR;			break;
		case PIC_FERN:			pic = PIC_FERN;			break;
		case PIC_SNOWFLAKE:		pic = PIC_SNOWFLAKE;	break;
		case PIC_TREE:			pic = PIC_TREE;			break;

		case ID_SLOW:	timer.Update(timer.dTimer * 1.1);	 break;
		case ID_FAST:	timer.Update(timer.dTimer * 0.9);	 break;
		}
	}; break;

	case WM_LBUTTONDOWN:
	{
		// Обновляем координаты
		switch (pic)
		{
		case PIC_FLOWER:		flw.newPos(rw);		break;
		case PIC_UZOR:			uzr.newPos(rw);		break;
		case PIC_STAR:			str.newPos(rw);		break;
		case PIC_FERN:			frn.newPos(rw);		break;
		case PIC_SNOWFLAKE:		snow.newPos(rw);	break;
		case PIC_TREE:			tr.newPos(rw);		break;
		}

		if (timer.nTimer == TIMER_ID)
			timer.Update(timer.dTimer);	// Обновление таймера

										// Перерисовываем
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
	}; break;

	case WM_RBUTTONDOWN:
	{
		switch (pic)
		{
		case PIC_FLOWER:
		{
			flw.newPos(rw);

			flw.x = LOWORD(lParam);
			flw.y = HIWORD(lParam);

			if (flw.y < flw.r)				flw.r = rand() % flw.y;
			if (flw.x < flw.r)				flw.r = rand() % flw.x;
			if (rw.bottom - flw.y < flw.r)	flw.r = rand() % (rw.bottom - flw.y);
			if (rw.right - flw.x < flw.r)	flw.r = rand() % (rw.right - flw.x);

		}; break;

		case PIC_UZOR:
		{
			uzr.newPos(rw);

			uzr.x = LOWORD(lParam);
			uzr.y = HIWORD(lParam);

			if (rw.bottom - uzr.y < uzr.h)	uzr.h = rand() % (rw.bottom - uzr.y);
			if (rw.right - uzr.x < uzr.w)	uzr.w = rand() % (rw.right - uzr.x);

		}; break;

		case PIC_STAR:
		{
			str.newPos(rw);

			str.x = LOWORD(lParam);
			str.y = HIWORD(lParam);

			if (str.y < str.r)				str.r = rand() % str.y;
			if (str.x < str.r)				str.r = rand() % str.x;
			if (rw.bottom - str.y < str.r)	str.r = rand() % (rw.bottom - str.y);
			if (rw.right - str.x < str.r)	str.r = rand() % (rw.right - str.x);

		}; break;

		case PIC_FERN:
		{
			frn.newPos(rw);

			frn.x = LOWORD(lParam);
			frn.y = HIWORD(lParam);

			if (rw.bottom - frn.y < frn.h)		frn.h = rand() % (rw.bottom - frn.y);
			if (rw.right - frn.x < frn.h / 2)	frn.h = rand() % (rw.right - frn.x) - frn.h / 2;

		}; break;

		case PIC_SNOWFLAKE:
		{
			snow.newPos(rw);

			snow.x = LOWORD(lParam);
			snow.y = HIWORD(lParam);

			if (snow.y < snow.r)				snow.r = rand() % snow.y;
			if (snow.x < snow.r)				snow.r = rand() % snow.x;
			if (rw.bottom - snow.y < snow.r)	snow.r = rand() % (rw.bottom - snow.y);
			if (rw.right - snow.x < snow.r)		snow.r = rand() % (rw.right - snow.x);

		}; break;

		case PIC_TREE:
		{
			tr.newPos(rw);

			tr.x = LOWORD(lParam);
			tr.y = HIWORD(lParam);

			if (tr.y < tr.h)				tr.h = rand() % tr.y;
			if (tr.x < tr.h)				tr.h = rand() % tr.x;
			if (rw.bottom - tr.y < tr.h)	tr.h = rand() % (rw.bottom - tr.y);
			if (rw.right - tr.x < tr.h)		tr.h = rand() % (rw.right - tr.x);

		}; break;
		}

		if (timer.nTimer == TIMER_ID)
			timer.Update(timer.dTimer);	// Обновление таймера

										// Перерисовываем
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
	}; break;

	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_LEFT:
		{
			switch (pic)
			{
			case PIC_FLOWER:		flw.r -= 10;			break;
			case PIC_UZOR:			uzr.h--;			break;
			case PIC_STAR:			str.r--;			break;
			case PIC_FERN:			frn.h--;			break;
			case PIC_SNOWFLAKE:		snow.r--;			break;
			case PIC_TREE:			tr.h--;				break;
			}
		}; break;
		case VK_RIGHT:
		{
			switch (pic)
			{
			case PIC_FLOWER:		flw.r += 10;			break;
			case PIC_UZOR:			uzr.h++;			break;
			case PIC_STAR:			str.r++;			break;
			case PIC_FERN:			frn.h++;			break;
			case PIC_SNOWFLAKE:		snow.r++;			break;
			case PIC_TREE:			tr.h++;				break;
			}
		}; break;
		}

		if (timer.nTimer == TIMER_ID)
			timer.Update(timer.dTimer);	// Обновление таймера

		// Перерисовываем
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
	}; break;

	case WM_DESTROY:
	{
		timer.Kill();
		PostQuitMessage(0);
	}; break;

	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0l;
}
