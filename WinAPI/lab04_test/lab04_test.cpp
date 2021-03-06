// --- Обязательный включаемый файл
#include <windows.h>
#include <math.h>
#include <time.h>

#include "Define.h"
#include "Flower.h"
#include "Uzor.h"
#include "Star.h"
#include "Fern.h"
#include "SnowFlake.h"
#include "Tree.h"
#include "resource.h"

// --- Глобальные переменные
int Width = 1000, Height = 600;				// Размеры окна
HINSTANCE hInst; 							// Дескриптор экземпляра приложения
char ClassName[] = "Window"; 				// Название класса окна
char AppTitle[] = "Лабораторная работа №4";	// Заголовок

int picture = NULL;			// Номер рисунка
Timer timer;				// Таймер

// Объявление структур, хранящих характерискики рисунков
FLOWER		flw;
UZOR		uzr;
STAR		str;
FERN		frn;
SNOWFLAKE	snow;
TREE		tr;

// --- Функция окна
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	RECT FAR rw;				// Характеристики внутренней (рабочей) области окна
	GetClientRect(hWnd, &rw);	// Определение координат внутренней области окна, относительно левого верхнего угла внутренней области окна

	switch (msg)
	{
	case WM_CREATE:				// При создании окна (CreateWindow)
	{
		srand(time(0));
		//picture = rand() % 6 + 1;
		picture = PIC_FLOWER;

		flw.newPos(rw);
		uzr.newPos(rw);
		str.newPos(rw);
		frn.newPos(rw);
		snow.newPos(rw);
		tr.newPos(rw);

		timer.Init(hWnd, TIMER_POS_AND_COL, 2000);

	}; break;

	case WM_COMMAND:			// Меню. Обработка кнопок.
	{
		switch (LOWORD(wParam))	// Меню
		{
		case PIC_FLOWER:		picture = PIC_FLOWER;		break;
		case PIC_UZOR:			picture = PIC_UZOR;			break;
		case PIC_STAR:			picture = PIC_STAR;			break;
		case PIC_FERN:			picture = PIC_FERN;			break;
		case PIC_SNOWFLAKE:		picture = PIC_SNOWFLAKE;	break;
		case PIC_TREE:			picture = PIC_TREE;			break;
		}

		switch (LOWORD(wParam))	// Клавиши
		{
		case ID_ACCELERATOR40003:	timer.Update(timer.dTimer * 1.1);	 break;
		case ID_ACCELERATOR40004:	timer.Update(timer.dTimer * 0.9);	 break;
		}

	}; break;

	case WM_TIMER:				// Таймер
	{
		//wParam - идентификатор таймера
		//lParam - адрес функции
		switch (wParam)
		{
		case TIMER_POS_AND_COL:
		{
			// Обновляем координаты и цвет
			switch (picture)
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


	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_LEFT:
		{
			switch (picture)
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
			switch (picture)
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

		if (timer.nTimer == TIMER_POS_AND_COL)
			timer.Update(timer.dTimer);	// Обновление таймера

										// Перерисовываем
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
	}; break;
	// Отрисовка клиентской области окна.
	case WM_PAINT:
	{
		if (picture != NULL)
		{
			HDC hDC;
			PAINTSTRUCT ps;
			hDC = BeginPaint(hWnd, &ps);	// Получить контекст окна

			switch (picture)
			{
			case PIC_FLOWER:		flw.Draw(hDC);		break;
			case PIC_UZOR:			uzr.Draw(hDC);		break;
			case PIC_STAR:			str.Draw(hDC);		break;
			case PIC_FERN:			frn.Draw(hDC);		break;
			case PIC_SNOWFLAKE:		snow.Draw(hDC);		break;
			case PIC_TREE:			tr.Draw(hDC);		break;
			}

			EndPaint(hWnd, &ps);			// Освободить контекст окна
		}
	}; break;

	// Нажата левая клавиша мыши в клиентской области окна.
	case WM_LBUTTONDOWN:
	{
		// Обновляем координаты
		switch (picture)
		{
		case PIC_FLOWER:		flw.newPos(rw);		break;
		case PIC_UZOR:			uzr.newPos(rw);		break;
		case PIC_STAR:			str.newPos(rw);		break;
		case PIC_FERN:			frn.newPos(rw);		break;
		case PIC_SNOWFLAKE:		snow.newPos(rw);	break;
		case PIC_TREE:			tr.newPos(rw);		break;
		}

		if (timer.nTimer == TIMER_POS_AND_COL)
			timer.Update(timer.dTimer);	// Обновление таймера

										// Перерисовываем
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
	}; break;

	// Нажата правая клавиша мыши в клиентской области окна.
	case WM_RBUTTONDOWN:
	{
		switch (picture)
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

		if (timer.nTimer == TIMER_POS_AND_COL)
			timer.Update(timer.dTimer);	// Обновление таймера

										// Перерисовываем
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
	}; break;
	// Пользователь удалил окно.
	case WM_DESTROY:
	{
		timer.Kill();
		PostQuitMessage(0);
	}; break;

	// Необработанные сообщения передаем в стандартную функцию обработки сообщений по умолчанию.
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0l;
}


// --- Функция WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc;
	HWND hWnd;
	MSG msg;
	hInst = hInstance;

	if ((hWnd = FindWindow(ClassName, NULL)) != NULL)
	{
		if (IsIconic(hWnd)) ShowWindow(hWnd, SW_RESTORE);
		SetForegroundWindow(hWnd);
		return FALSE;
	}

	memset(&wc, 0, sizeof(wc));
	wc.lpszClassName = ClassName;									// Имя класса окон
	wc.lpfnWndProc = (WNDPROC)WndProc;								// Обработка сообщений. Адрес оконной функции
	wc.style = CS_HREDRAW | CS_VREDRAW;								// Стиль класса
	wc.hInstance = hInstance;										// Экземпляр приложения
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON4));						// Пиктограмма для окон
	wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR3));				// Курсор мыши для окон	
	LoadString(hInstance, 121, AppTitle, sizeof(AppTitle));							// Название заголовка окна
	HACCEL haccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1)); // Загрузка таблицы акселераторов

	RegisterClass(&wc);		// Pегистрация класса окна


							// Создаем главное окно приложения.
	hWnd = CreateWindow(ClassName, AppTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		Width, Height, NULL, NULL, hInst, NULL);
	if (!hWnd)
	{
		// Окно не создано, выдаем предупреждение.
		MessageBox(NULL, TEXT("Create: error"), AppTitle, MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	HMENU hMenu = CreateMenu();
	AppendMenu(hMenu, MF_STRING, PIC_FLOWER, "Flower");
	AppendMenu(hMenu, MF_STRING, PIC_UZOR, "Uzor");
	AppendMenu(hMenu, MF_STRING, PIC_STAR, "Star");
	AppendMenu(hMenu, MF_STRING, PIC_FERN, "Fern");
	AppendMenu(hMenu, MF_STRING, PIC_SNOWFLAKE, "Snowflake");
	AppendMenu(hMenu, MF_STRING, PIC_TREE, "Tree");
	SetMenu(hWnd, hMenu);

	ShowWindow(hWnd, nCmdShow);			// Отображаем окно.
	UpdateWindow(hWnd);					// Обновляем содержимое клиентской области окна.

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!haccel || !TranslateAccelerator(hWnd, haccel, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}