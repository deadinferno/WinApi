// lab02_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// --- Обязательный включаемый файл
#include <windows.h>
#include <iostream>


// --- Описание функции главного окна
LRESULT CALLBACK WndProcMain(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcPopup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcChild(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// --- Глобальные переменные
HINSTANCE hInst; 							// Дескриптор экземпляра приложения

const wchar_t strMain[] = L"WindowsMain",
strPopup[] = L"WindowsPopup",
strChild[] = L"WindowsChild";

HBRUSH bush[] = {
	CreateSolidBrush(RGB(255, 255, 255)),
	CreateSolidBrush(RGB(255, 0, 0)),
	CreateSolidBrush(RGB(0, 255, 0))
};

COLORREF rgb[] = {
	RGB(100, 255, 255),
	RGB(0, 150, 200),
	RGB(200, 200, 0)
};

#define BushWinMain 0
#define BushWinPopup 1
#define BushWinChild 2

// --- Функция WinMain
int WINAPI WinMain(HINSTANCE hInstance,
	// Дескриптор экземпляра приложения
	HINSTANCE hPrevInstance, // В Win32 всегда равен NULL
	LPSTR lpCmdLine,
	// Указатель на командную строку. Он // позволяет
	// приложению получать данные из командной строки.
	int nCmdShow
	// Определяет, как приложение первоначально 
	// отображается на дисплее: пиктограммой
	// (nCmdShow = SW_SHOWMINNOACTIVE) 
	// или в виде открытого окна 					
	//(nCmdShow = SW_SHOWNORMAL).
)
{
	WNDCLASS wcMain;
	WNDCLASS wcPopup;
	WNDCLASS wcChild;
	//WNDCLASS wc; 	// Структура для информации о классе окна	
	HWND hWnd; 		// Дескриптор главного окна приложения
	MSG msg; 		// Структура для хранения сообщения
					// Сохраняем дескриптор экземпляра приложения в глобальной
					// переменной, чтобы при необходимости воспользоваться им в
					// функции окна.
	hInst = hInstance;

	// --- Проверяем, было ли приложение запущено ранее.
	// Воспользуемся функцией FindWindow, которая позволяет
	// найти окно верхнего 
	// уровня по имени класса или по заголовку окна:
	// HWND FindWindow(LPCTSTR lpClassName,
	// LPCTSTRlpWindowName);
	// Через параметр lpClassName передается указатель на
	// текстовую строку, в которую необходимо записать имя
	// класса искомого окна. На базе одного и того же класса 
	// можно создать несколько окон. Если необходимо найти 
	// окно с заданным заголовком, то имя заголовка следует
	// передать через параметр lpWindowName. Если же подойдет 
	// любое окно, то параметр lpWindowName может иметь
	// значение NULL.
	if ((hWnd = FindWindow(strMain, NULL)) != NULL)
	{
		// Пользователь может не помнить, какие приложения уже
		// запущены, а какие нет. Когда он запускает приложение, 
		// то ожидает, что на экране появится его главное окно.
		// Поэтому, если приложение было запущено ранее,
		// целесообразно активизировать и выдвинуть на передний
		// план его главное окно. Это именно то, к чему приготовился
		// пользователь.
		if (IsIconic(hWnd)) ShowWindow(hWnd, SW_RESTORE);
		SetForegroundWindow(hWnd);

		// Найдена работающая копия - работа новой копии
		// прекращается.
		return FALSE;
	}

	// --- Работающая копия не найдена - функция WinMain
	// приступает к инициализации. Заполнение структуры
	// WNDCLASS для регистрации класса окна.
	memset(&wcMain, 0, sizeof(wcMain));
	wcMain.lpszClassName = strMain;					// Имя класса окон
	wcMain.lpfnWndProc = (WNDPROC)WndProcMain;		// Обработка сообщений
	wcMain.hbrBackground = (HBRUSH)CreateSolidBrush(rgb[BushWinMain]);

	memset(&wcPopup, 0, sizeof(wcPopup));
	wcPopup.lpszClassName = strPopup;				// Имя класса окон
	wcPopup.lpfnWndProc = (WNDPROC)WndProcPopup;	// Обработка сообщений
	wcPopup.hbrBackground = (HBRUSH)CreateSolidBrush(rgb[BushWinPopup]);

	memset(&wcChild, 0, sizeof(wcChild));
	wcChild.lpszClassName = strChild;				// Имя класса окон
	wcChild.lpfnWndProc = (WNDPROC)WndProcChild;	// Обработка сообщений
	wcChild.hbrBackground = (HBRUSH)CreateSolidBrush(rgb[BushWinChild]);

	// Pегистрация класса окна.
	RegisterClass(&wcMain);
	RegisterClass(&wcPopup);
	RegisterClass(&wcChild);



	// создание главного перекрывающегося окна
	hWnd =
		CreateWindow(strMain, "OVERLAPPEDWINDOW", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	if (hWnd == 0) { DestroyWindow(hWnd); return FALSE; }
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	RECT FAR r;
	GetWindowRect(hWnd, &r);



	// создание временного окна с родителем
	HWND hWndPopupWithParent =
		CreateWindow(strPopup, "POPUPWINDOW", WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE, r.left + 500, r.top + 200, 300, 300, hWnd, NULL, hInstance, NULL);
	if (hWndPopupWithParent == 0) { DestroyWindow(hWndPopupWithParent); return FALSE; }
	ShowWindow(hWndPopupWithParent, nCmdShow);
	UpdateWindow(hWndPopupWithParent);



	// создание временного окна без родителем
	HWND hWndPopupWithoutParent =
		CreateWindow(strPopup, L"POPUPWINDOW", WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE, r.left + 800, r.top + 200, 300, 300, NULL, NULL, hInstance, NULL);
	if (hWndPopupWithoutParent == 0) { DestroyWindow(hWndPopupWithoutParent); return FALSE; }
	ShowWindow(hWndPopupWithoutParent, nCmdShow);
	UpdateWindow(hWndPopupWithoutParent);



	// создание дочернего окна
	HWND hWndChild =
		CreateWindow(strChild, "CHILDWINDOW", WS_CHILDWINDOW | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 0, 150, 300, 300, hWnd, NULL, hInstance, NULL);
	if (hWndChild == 0) { DestroyWindow(hWndChild); return FALSE; }
	ShowWindow(hWndChild, nCmdShow);
	UpdateWindow(hWndChild);



	// Запускаем цикл обработки очереди сообщений.
	// Функция GetMessage получает сообщение из очереди, 
	// выдает false при выборке из очереди сообщения WM_QUIT
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


void fun(int iBush, LPCWSTR str, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;

	TCHAR	dysplay[255],
		window[255],
		wundowWH[255],
		position[255];

	int w = GetSystemMetrics(SM_CXSCREEN), // ширина дисплея
		h = GetSystemMetrics(SM_CYSCREEN); // высота дисплея

	wsprintf(dysplay, TEXT("Размер экрана:   %d x %d"), w, h);

	RECT FAR r;
	GetClientRect(hWnd, &r); // определение координат внутренней области окна, относительно левого верхнего угла внутренней области окна
	wsprintf(window, TEXT("Рабочая область: %d x %d"), r.right, r.bottom);

	GetWindowRect(hWnd, &r); // Эта функция выдает информацию о расположении и размере прямоугольной области, ограничивающей окно, с учетом заголовка, рамки и полос просмотра. Все координаты отсчитываются от верхнего левого угла экрана.
	wsprintf(wundowWH, TEXT("Размер окна:     %d x %d"), r.right - r.left, r.bottom - r.top);
	wsprintf(position, TEXT("Положение окна:  %d x %d"), r.left, r.top);

	hDC = BeginPaint(hWnd, &ps);
	//FillRect(ps.hdc, &ps.rcPaint, bush[iBush]);
	SetBkColor(hDC, rgb[iBush]);

	TextOut(hDC, 20, 20, str, wcslen(str));
	TextOut(hDC, 20, 50, dysplay, _tcslen(dysplay));
	TextOut(hDC, 20, 70, window, _tcslen(window));
	TextOut(hDC, 20, 90, wundowWH, _tcslen(wundowWH));
	TextOut(hDC, 20, 110, position, _tcslen(position));

	EndPaint(hWnd, &ps);
}


// --- Функция главного окна
LRESULT CALLBACK WndProcMain(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{

	case WM_PAINT:
	{
		fun(BushWinMain, strMain, hWnd, wParam, lParam);
	}; break;

	case WM_SIZE:
	{
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
	}; break;

	case WM_MOVE:
	{
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);

	}; break;

	// Пользователь удалил окно.
	case WM_DESTROY:
	{
		// Если данная функция является оконной функцией
		// главного окна, то следует в очередь сообщений
		// приложения послать сообщение WM_QUIT 
		PostQuitMessage(0);
	}; break;

	// Необработанные сообщения передаем в стандартную
	// функцию обработки сообщений по умолчанию.
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0l;
}


// --- Функция всплывающего окна
LRESULT CALLBACK WndProcPopup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
	{
		fun(BushWinPopup, strPopup, hWnd, wParam, lParam);
	}; break;

	case WM_MOVE:
	{
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);

	}; break;

	// Необработанные сообщения передаем в стандартную
	// функцию обработки сообщений по умолчанию.
	default: return DefWindowProc(hWnd, msg, wParam, lParam); //функция по умолчанию.
	}
	return 0l;
}


// --- Функция дочернего окна
LRESULT CALLBACK WndProcChild(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
	{
		fun(BushWinChild, strChild, hWnd, wParam, lParam);
	}; break;

	case WM_MOVE:
	{
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);

	}; break;

	// Необработанные сообщения передаем в стандартную
	// функцию обработки сообщений по умолчанию.
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0l;
}