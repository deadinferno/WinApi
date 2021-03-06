// lab01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <windows.h>
#include <iostream>

#define BrushWinMain 0
#define BushWinPopup 1
#define BushWinChild 2

LRESULT CALLBACK WndProc(HWND hWndMain, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndPopProc(HWND hWndPop, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndChildProc(HWND hWndPop, UINT msg, WPARAM wParam, LPARAM lParam);

const wchar_t MainWindowsClass[] = L"MainWindowsClass";		// Название класса главных окон
const wchar_t PopupWindowsClass[] = L"PopupWindowsClass"; 	// Название класса всплывающих окон
const wchar_t ChildWindowsClass[] = L"ChildWindowsClass"; 	// Название класса дочерних окон

const wchar_t MainWindowTitle[] = L"Главное окно"; 	//  Заголовок главного окна
const wchar_t PopupWindowTitle[] = L"Всплывающее окно"; 	// Заголовок всплывающего окна
const wchar_t ChildWindowTitle[] = L"Дочернее окно"; 	// Заголовок дочернего окна

HBRUSH brush[] = {
	CreateSolidBrush(RGB(255, 255, 255)),
	CreateSolidBrush(RGB(255, 0, 0)),
	CreateSolidBrush(RGB(0, 255, 0))
};

// --- Функция WinMain
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASS wcMain, wcPop, wcChild; 	// Contains the window class attributes that are registered by the RegisterClass function
	HWND hWndMain, hWndPop, hWndPop2, hWndChild; 	// Дескрипторы окон

	MSG msg; 	// Contains message information from a thread's message queue.
	
	if ((hWndMain = FindWindow(MainWindowsClass, MainWindowTitle)) != NULL)
	{
		if (IsIconic(hWndMain))
			ShowWindow(hWndMain, SW_RESTORE);

		SetForegroundWindow(hWndMain);

		return FALSE;
	}

	memset(&wcMain, 0, sizeof(wcMain));
	memset(&wcPop, 0, sizeof(wcPop));
	memset(&wcChild, 0, sizeof(wcChild));

	wcMain.lpszClassName = MainWindowsClass;		// Имя класса главного окна
	wcPop.lpszClassName = PopupWindowsClass;		// Имя класса всплывающего окна
	wcChild.lpszClassName = ChildWindowsClass;		// Имя класса всплывающего окна

	wcMain.lpfnWndProc = (WNDPROC)WndProc;	// Адрес оконной функции
	wcPop.lpfnWndProc = (WNDPROC)WndPopProc;
	wcChild.lpfnWndProc = (WNDPROC)WndChildProc;

	/*CS_HREDRAW - Redraws the entire window if a movement or size
	adjustment changes the width of the client area.*/
	wcMain.style = CS_HREDRAW | CS_VREDRAW;	// Стиль класса 
	wcPop.style = CS_HREDRAW | CS_VREDRAW;
	wcChild.style = CS_HREDRAW | CS_VREDRAW;

	wcMain.hInstance = hInstance;		// Экземпляр приложения
	wcPop.hInstance = hInstance;
	wcChild.hInstance = hInstance;

	wcMain.hIcon = LoadIcon(NULL, IDI_QUESTION);
	wcPop.hIcon = LoadIcon(NULL, IDI_ASTERISK);

	wcMain.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcPop.hCursor = LoadCursor(NULL, IDC_IBEAM);
	wcChild.hCursor = LoadCursor(NULL, IDC_CROSS);

	wcMain.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wcPop.hbrBackground = GetSysColorBrush(COLOR_3DSHADOW);
	wcChild.hbrBackground = GetSysColorBrush(COLOR_3DHIGHLIGHT);
	//wcMain.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);

	wcMain.lpszMenuName = NULL;		// Ресурс меню окон
	wcPop.lpszMenuName = NULL;

	wcMain.cbClsExtra = 0;			// Дополнительная память
	wcMain.cbWndExtra = 0;			// Дополнительная память

	wcPop.cbClsExtra = 0;
	wcPop.cbWndExtra = 0;			

	wcChild.cbClsExtra = 0;
	wcChild.cbWndExtra = 0;

	// Pегистрация класса окна.
	RegisterClass(&wcMain);
	RegisterClass(&wcPop);
	RegisterClass(&wcChild);

	// Создаем главное окно приложения.
	hWndMain = CreateWindow(
		MainWindowsClass, 			// Имя класса окон
		MainWindowTitle,			// Заголовок окна 
		WS_OVERLAPPEDWINDOW | 
		WS_VSCROLL | WS_HSCROLL, 		// Стиль окна
		100,			// X-координаты 
		100,			// Y-координаты 
		CW_USEDEFAULT,			// Ширина окна
		CW_USEDEFAULT,			// Высота окна
		NULL,			// Дескриптор окна-родителя
		NULL,			// Дескриптор меню окна
		hInstance,		// Дескриптор экземпляра приложения
		NULL);		// Дополнительная информация

					/*If the function succeeds, the return value is a handle to the new window.
					If the function fails, the return value is NULL.To get extended error information, call GetLastError.*/

	if (!hWndMain)
	{
		MessageBox(NULL,
			L"Не удалось получить дескриптор окна.", MainWindowTitle, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	/* Создаем всплывающее окно приложения с родителем */
	hWndPop = CreateWindow(
		PopupWindowsClass, 			// Имя класса окон
		PopupWindowTitle,			// Заголовок окна 
		WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX |
		WS_MAXIMIZEBOX | WS_VISIBLE, 		// Стиль окна
		100, 100, 300, 300,
		hWndMain,			// Дескриптор окна-родителя
		NULL,			// Дескриптор меню окна
		hInstance,		// Дескриптор экземпляра приложения
		NULL);		// Дополнительная информация

	if (!hWndPop)
	{
		DestroyWindow(hWndMain);
		return FALSE;
	}

	/* Создаем всплывающее окно приложения без родителя*/
	hWndPop2 = CreateWindow(
		PopupWindowsClass, 			// Имя класса окон
		PopupWindowTitle,			// Заголовок окна 
		WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX |
		WS_MAXIMIZEBOX | WS_VISIBLE, 		// Стиль окна
		400, 100, 300, 300,
		NULL,			// Дескриптор окна-родителя
		NULL,			// Дескриптор меню окна
		hInstance,		// Дескриптор экземпляра приложения
		NULL);		// Дополнительная информация

	if (!hWndPop2)
	{
		DestroyWindow(hWndMain);
		return FALSE;
	}

	/* Создаем дочернее окно для главного */
	hWndChild = CreateWindow(
		ChildWindowsClass, 			// Имя класса окон
		ChildWindowTitle,			// Заголовок окна 
		WS_CHILDWINDOW | WS_CAPTION | WS_MINIMIZEBOX |
		WS_MAXIMIZEBOX, 550, 350, 250, 250,
		hWndMain,			// Дескриптор окна-родителя
		NULL,			// Дескриптор меню окна
		hInstance,		// Дескриптор экземпляра приложения
		NULL);		// Дополнительная информация

	if (!hWndChild)
	{
		DestroyWindow(hWndMain);
		DestroyWindow(hWndPop);
		return FALSE;
	}

	// Отображаем главное окно.
	ShowWindow(hWndMain, SW_SHOWNORMAL);

	/* Отображаем всплывающее */
	ShowWindow(hWndPop, SW_SHOWNORMAL);

	/* Отображаем всплывающее 2*/
	ShowWindow(hWndPop2, SW_SHOWNORMAL);

	/* Отображаем дочернее */
	ShowWindow(hWndChild, SW_SHOWNORMAL);

	// Обновляем содержимое клиентской области окна. Т.е. посылаем сообщение WM_PAINT
	UpdateWindow(hWndMain);
	UpdateWindow(hWndPop);
	UpdateWindow(hWndPop2);
	UpdateWindow(hWndChild);

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

// --- Рисовка метрик
void draw_metrics(int iBrush, LPCWSTR str, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;

	TCHAR	display[255],
		window[255],
		windowWH[255],
		position[255];

	int w = GetSystemMetrics(SM_CXSCREEN), // ширина дисплея
		h = GetSystemMetrics(SM_CYSCREEN); // высота дисплея

	wsprintf(display, TEXT("Размер экрана:   %d x %d"), w, h);

	RECT FAR r;
	GetClientRect(hWnd, &r); // определение координат внутренней области окна, относительно левого верхнего угла внутренней области окна
	wsprintf(window, TEXT("Рабочая область: %d x %d"), r.right, r.bottom);

	GetWindowRect(hWnd, &r); // Эта функция выдает информацию о расположении и размере прямоугольной области, ограничивающей окно, с учетом заголовка, рамки и полос просмотра. Все координаты отсчитываются от верхнего левого угла экрана.
	wsprintf(windowWH, TEXT("Размер окна:     %d x %d"), r.right - r.left, r.bottom - r.top);
	wsprintf(position, TEXT("Положение окна:  %d x %d"), r.left, r.top);

	hDC = BeginPaint(hWnd, &ps);

	if (hDC == NULL)
	{
		EndPaint(hWnd, &ps);
		MessageBox(hWnd, L"Не удалось получить дескриптор экземпляра окна, завершаем приложение.", L"Ошибка",
			MB_OK | MB_ICONERROR);
		PostQuitMessage(1);
	}

	//SetBkColor(hDC, rgb[iBrush]);

	TextOut(hDC, 20, 20, str, wcslen(str));
	TextOut(hDC, 20, 50, display, _tcslen(display));
	TextOut(hDC, 20, 70, window, _tcslen(window));
	TextOut(hDC, 20, 90, windowWH, _tcslen(windowWH));
	TextOut(hDC, 20, 110, position, _tcslen(position));

	EndPaint(hWnd, &ps);
}

// --- Оконная процедура главного окна
LRESULT CALLBACK WndProc(HWND hWndMain, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LPCWSTR lpWideStr = L"First Windows aplication";
	LPCWSTR x;
	RECT wndRect;

	switch (msg)
	{
		// Необходимо обновить содержимое клиентской области окна.
	case WM_PAINT:
	{
		draw_metrics(BrushWinMain, MainWindowsClass, hWndMain, wParam, lParam);
		
	}; break;

	case WM_MOVE:
	{
		InvalidateRect(hWndMain, NULL, TRUE);
		UpdateWindow(hWndMain);
	} break;

	// Нажата левая клавиша мыши в клиентской области окна.
	case WM_LBUTTONDOWN:
	{
		MessageBox(hWndMain, L"32-bit aplication", L"Window",
			MB_OK | MB_ICONINFORMATION);

		/*uType[in]
		Type: UINT
		The contents and behavior of the dialog box.
		This parameter can be a combination of flags.*/


	}; break;

	// Пользователь удалил окно.
	case WM_DESTROY:
	{
		// Indicates to the system that a thread has made a request to terminate(quit).It is typically used in response to a WM_DESTROY message.
		PostQuitMessage(0);
	}; break;

	// Необработанные сообщения передаем в стандартную
	// функцию обработки сообщений по умолчанию.
	default: return DefWindowProc(hWndMain, msg, wParam, lParam);
	}
	return 0L;
}

// --- Оконная процедура всплывающего окна
LRESULT CALLBACK WndPopProc(HWND hWndPop, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LPCWSTR lpWideStr = L"Текст внутри всплывающего окна";

	switch (msg)
	{
	case WM_PAINT:
	{
		draw_metrics(BrushWinMain, PopupWindowsClass, hWndPop, wParam, lParam);

	}; break;

	case WM_MOVE:
	{
		InvalidateRect(hWndPop, NULL, TRUE);
		UpdateWindow(hWndPop);
	} break;

	case WM_LBUTTONDOWN:
	{
		MessageBox(hWndPop, L"Клик внутри всплывающего окна", L"Window",
			MB_OK | MB_ICONINFORMATION);
	}; break;

	// Пользователь удалил окно.
	case WM_DESTROY:
	{
		//PostQuitMessage(0);
	}; break;

	default: return DefWindowProc(hWndPop, msg, wParam, lParam);
	}
	return 0L;
}

// --- Оконная процедура дочернего окна
LRESULT CALLBACK WndChildProc(HWND hWndChild, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LPCWSTR lpWideStr = L"Текст внутри дочернего окна";

	switch (msg)
	{
	case WM_PAINT:
	{
		draw_metrics(BrushWinMain, PopupWindowsClass, hWndChild, wParam, lParam);

	}; break;

	case WM_MOVE:
	{
		InvalidateRect(hWndChild, NULL, TRUE);
		UpdateWindow(hWndChild);
	} break;

	case WM_LBUTTONDOWN:
	{
		MessageBox(hWndChild, L"Клик внутри дочернего окна", L"Window",
			MB_OK | MB_ICONINFORMATION);
	}; break;

	// Пользователь удалил окно.
	case WM_DESTROY:
	{
		//PostQuitMessage(0);
	}; break;

	default: return DefWindowProc(hWndChild, msg, wParam, lParam);
	}
	return 0L;
}

/*

	1.	Что определяет (характеризует) стиль класса окна? Что определяет стиль окна? 
		•	Стиль окна определяет внешний вид окна и его поведение. 
		•	Для класса окна также определяется понятие стиля - стиль класса определяет 
		внешний вид и поведение всех окон, созданных на базе данного класса.
	***************************************************************************************

	2.	Как задается стиль класса окна?
		Он задается в виде отдельных битов, для которых определены символические 
		константы с префиксом CS_. Определенный в классе окна стиль класса окна 
		используется при создании всех окон на базе этого класса.
	***************************************************************************************

	3.	Для чего задаются стили класса CS_HREDRAW, CS_VREDRAW?
		Как уже говорилось, если для класса заданы стили CS_HREDRAW и CS_VREDRAW, 
		то при изменении размеров окна функция окна может получить сообщение WM_PAINT. 
		В этом случае функция окна должна перерисовать часть окна или все окно. 
	***************************************************************************************

	4.	Для чего используется стиль класса CS_DBLCLKS? 
		Стиль CS_DBLCLKS используется при необходимости отслеживать двойные щелчки мышью. 
		При этом в функцию окна посылаются сообщения WM_LBUTTONDBLCLK и WM_RBUTTONDBLCLK. 
		Если этот стиль не будет задан, функция окна получит только идущие парами сообщения 
		об одиночном нажатии клавиш мыши WM_LBUTTONDOWN и WM_RBUTTONDOWN.
	***************************************************************************************

	5.	Какая характеристика используется для уточнения внешнего вида и поведения 
	окна, создаваемого на базе какого-либо класса?
		Определенный в классе окна стиль класса окна используется при создании всех 
		окон на базе этого класса. Для дальнейшего уточнения внешнего вида и поведения 
		окна используется другая характеристика - стиль окна. Стиль окна указывается при 
		создании окна функцией CreateWindow. Для определения стиля окна используются 
		символические константы с префиксом WS_.
	***************************************************************************************

	6.	Какой внешний вид и поведение обычно характерен для перекрывающихся окон 
	(назвать стиль класса)? 
		Перекрывающиеся окна имеют заголовок (title bar), рамку и внутреннюю часть 
		окна (client region). Дополнительно перекрывающиеся окна могут иметь системное 
		меню, кнопки для максимального увеличения размера окна и свертки окна в пиктограмму, 
		вертикальную и горизонтальную полосу просмотра и меню. 
		Перекрывающиеся окна обычно используются в качестве главного окна приложения.
		Для определения стиля перекрывающегося окна существует символическая 
		константа WS_OVERLAPPEDWINDOW
	***************************************************************************************

	7.	Что такое окно-владелец и подчиненное окно? Чем поведение подчиненного окна 
	отличается от поведения обычного перекрывающегося окна?
		...
	***************************************************************************************

	8.	Для чего чаще всего используются временные (всплывающие, popup) окна?
		Другим базовым стилем является стиль временных окон, которые обычно используются 
		для вывода информационных сообщений и остаются на экране непродолжительное время. 
		Временные окна, в отличие от перекрывающихся, могут не иметь заголовка.
	***************************************************************************************

	9.	Какой внешний вид обычно имеют временные окна (назвать стиль класса)? 
		Временные окна, в отличие от перекрывающихся, могут не иметь заголовка. 
		Если для временного меню заголовок определен, тогда оно может иметь и системное меню.
		Часто для создания выпадающих окон, имеющих рамку, используется стиль 
		WS_POPUPWINDOW, определенный как поразрядное ИЛИ нескольких констант
	***************************************************************************************

	10.	Где располагается начало системы координат для перекрывающихся и временных окон?
		В верхнем левом углу экрана.
	***************************************************************************************

	11.	Для чего обычно используются дочерние окна?
		Дочерние окна чаще всего используются приложениями Windows. Эти окна нужны для создания 
		таких органов управления, как кнопки или списки. Все органы управления - дочерние окна.
	***************************************************************************************

	12.	Где располагается начало системы координат для дочерних окон?
		Во внутреннем левом верхнем угле окна-родителя.
	***************************************************************************************

	13.	Какой внешний вид обычно имеют дочерние окна (назвать стиль класса)?
		Стиль дочернего окна определяется константой WS_CHILDWINDOW. 
		В отличие от перекрывающихся и временных окон дочерние окна, как правило, 
		не имеют рамки, заголовка, кнопок минимизации и максимального увеличения 
		размеров окна, а также полос просмотра. Дочерние окна сами рисуют все, что 
		в них должно быть изображено.
	***************************************************************************************

	14.	Каковы особенности поведения дочерних окон?
		...
	***************************************************************************************

	15.	Каким простым способом можно сообщить Windows о том, что окно должно иметь 
	горизонтальную и/или вертикальную полосу прокрутки? В чем недостатки такого метода?
		Добавить в окно приложения вертикальную или горизонтальную полосу прокрутки очень 
		просто. Все, что необходимо сделать, это включить стиль WS_VSCROLL и WS_HSCROLL или 
		оба сразу в описание стиля создаваемого функцией CreateWindow окна.
	***************************************************************************************

	16.	Какое значение имеют понятия диапазон полосы прокрутки и ее текущее 
	положение? Как можно изменять эти характеристики?
		Каждая полоса прокрутки имеет соответствующий диапазон (range) – два целых числа,
		отражающих минимальное и максимальное значения, и положение (position) 
		– местоположение бегунка внутри диапазона. По умолчанию устанавливается 
		следующий диапазон прокрутки – минимум 0 и максимум 100, но диапазон легко 
		изменить на любое другое значение с помощью функции SetScrollRange 
		(для полос прокрутки окна hWnd)
	***************************************************************************************

	17.	Какова сфера ответственности Windows в организации работы 
	пользователя с полосой просмотра?
		...
	***************************************************************************************

	18.	Какова сфера ответственности приложения в организации работы 
	пользователя с полосой просмотра?
		...
	***************************************************************************************

	19.	Какие сообщения приходят окну от полос просмотра?
		Windows посылает оконной процедуре сообщения WM_VSCROLL и WM_HSCROLL, 
		когда на полосе прокрутки щелкают мышью или перетаскивают бегунок.
	***************************************************************************************

	20.	Организован ли на системном уровне интерфейс клавиатуры для работы с полосами прокрутки?
		Замечание. Согласно документации, сообщения с кодами SB_TOP и SB_BOTTOM показывают, 
		что полоса прокрутки переведена в свое максимальное или минимальное положение. Однако, 
		если приложение самостоятельно не реализовывает интерфейс для работы с полосой прокрутки 
		при помощи клавиатуры и не посылает само себе такие сообщения по мере необходимости, то 
		оно никогда его не получит от Windows.
	***************************************************************************************

	21.	При помощи какой функции можно определить системные метрики Windows?
		Метрики системных компонентов Windows можно определить при помощи функции GetSystemMetrics. 
		Единственный аргумент этой функции задает параметр, значение которого необходимо определить
		int GetSystemMetrics(int nIndex);
	***************************************************************************************

	22.	Когда окно получает сообщение WM_SIZE и какие дополнительные параметры передаются окну 
	вместе с этим сообщением?
		Первый метод основан на том факте, что при изменении размера окна функция окна получает 
		сообщение WM_SIZE, параметры которого несут с собой информацию о новых размерах. Параметры 
		сообщения WM_SIZE описывают новый размер окна и способ, которым оно изменило размер:
		wParam - способ, при помощи которого окно изменило свой размер;
		LOWORD(lParam) - новая ширина клиентской части окна;
		HIWORD(lParam) - новая высота клиентской части окна.
		Параметр wParam может принимать одно из нескольких значений, символические имена которых 
		начинаются с префикса SIZE_ (например, SIZE_MAXIMIZED – окно было максимизировано).
	***************************************************************************************

	23.	Для чего предназначена функция GetClientRect?
		Для определения рабочей области окна
	***************************************************************************************

	24.	Когда окно получает сообщение WM_MOVE и какие дополнительные параметры передаются окну 
	вместе с этим сообщением?
		При перемещении окна функция окна получает сообщение WM_MOVE, вместе с ним она получает 
		новые координаты внутренней области окна:
		wParam - не используется;
		LOWORD(lParam) - X-координата верхнего левого угла клиентской части окна;
		HIWORD(lParam) - Y-координата верхнего левого угла клиентской части окна.
	***************************************************************************************

	25.	Для чего предназначена функция GetWindowRect?
		Определение расположения и размеров окна.
	***************************************************************************************

	26.	Как определить метрики шрифта, установленного в контексте отображения окна?
		Для получения информации о шрифте, выбранном в контекст устройства, предназначена функция:
		BOOL GetTextMetrics(HDC hdc, TEXTMETRIC FAR* lptm);


*/
