// lab01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <windows.h>
#include <iostream>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// --- Глобальные переменные
HINSTANCE hInst; 		// Дескриптор экземпляра приложения
const wchar_t ClassName[] = L"Window"; 		// Название класса окна
const wchar_t AppTitle[] = L"Application Win32"; 	// Заголовок главного окна

// --- Функция WinMain
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASS wc; 	// Contains the window class attributes that are registered by the RegisterClass function
	HWND hWnd; 	// Дескриптор главного окна приложения
	MSG msg; 	// Contains message information from a thread's message queue.

	nCmdShow = SW_SHOWMAXIMIZED;

	hInst = hInstance;

	if ((hWnd = FindWindow(ClassName, AppTitle)) != NULL)
	{
		if (IsIconic(hWnd))
			ShowWindow(hWnd, SW_RESTORE);

		SetForegroundWindow(hWnd);

		return FALSE;
	}

	memset(&wc, 0, sizeof(wc));
	wc.lpszClassName = ClassName;		// Имя класса окон
	wc.lpfnWndProc = (WNDPROC)WndProc;
	// Адрес оконной функции
	wc.style = CS_HREDRAW | CS_VREDRAW;	// Стиль класса 
	/*CS_HREDRAW
	Redraws the entire window if a movement or size adjustment changes the width of the client area.*/

	wc.hInstance = hInstance;		// Экземпляр приложения
	wc.hIcon = LoadIcon(NULL, IDI_QUESTION);
	wc.hCursor = LoadCursor(NULL, IDC_WAIT);
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	//wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.lpszMenuName = NULL;		// Ресурс меню окон
	wc.cbClsExtra = 0;			// Дополнительная память
	wc.cbWndExtra = 0;			// Дополнительная память
								// Pегистрация класса окна.
	RegisterClass(&wc);

	// Создаем главное окно приложения.
	hWnd = CreateWindow(
		ClassName, 			// Имя класса окон
		AppTitle,			// Заголовок окна 
		WS_OVERLAPPEDWINDOW, 		// Стиль окна
		CW_USEDEFAULT,			// X-координаты 
		CW_USEDEFAULT,			// Y-координаты 
		CW_USEDEFAULT,			// Ширина окна
		CW_USEDEFAULT,			// Высота окна
		NULL,			// Дескриптор окна-родителя
		NULL,			// Дескриптор меню окна
		hInst,		// Дескриптор экземпляра приложения
		NULL);		// Дополнительная информация

					/*If the function succeeds, the return value is a handle to the new window.
					If the function fails, the return value is NULL.To get extended error information, call GetLastError.*/

	if (!hWnd)
	{
		MessageBox(NULL,
			L"Не удалось получить дескриптор окна.", AppTitle, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// Отображаем окно.
	ShowWindow(hWnd, nCmdShow); // nCmdShow
						
	// Обновляем содержимое клиентской области окна. Т.е. посылаем сообщение WM_PAINT
	UpdateWindow(hWnd);

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

// --- Оконная процедура
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LPCWSTR lpWideStr = L"First Windows aplication";

	switch (msg)
	{
		// Необходимо обновить содержимое клиентской области окна.
	case WM_PAINT:
	{
		HDC hDC;
		PAINTSTRUCT ps;

		// The BeginPaint function prepares the specified window for painting and fills a PAINTSTRUCT 
		// structure with information about the painting.
		hDC = BeginPaint(hWnd, &ps);

		if (hDC == NULL)
		{
			EndPaint(hWnd, &ps);
			MessageBox(hWnd, L"Не удалось получить дескриптор экземпляра окна, завершаем приложение.", L"Ошибка",
				MB_OK | MB_ICONERROR);
			PostQuitMessage(1);
			return 1L;
		}

		/*	Parameters
		hwnd[in]
		Handle to the window to be repainted.
		lpPaint[out]
		Pointer to the PAINTSTRUCT structure that will receive painting information.
		Return value
		If the function succeeds, the return value is the handle to a display device context for the specified window.
		If the function fails, the return value is NULL, indicating that no display device context is available.*/

		// Получить контекст окна
		// The TextOut function writes a character string at the specified location, using the currently 
		// selected font, background color, and text color.
		TextOut(hDC, 350, 350, lpWideStr, wcslen(lpWideStr));

		/*	Parameters

		hdc[in]
		A handle to the device context.
		nXStart[in]
		The x - coordinate, in logical coordinates, of the reference point that the system uses to align the string.
		nYStart[in]
		The y - coordinate, in logical coordinates, of the reference point that the system uses to align the string.
		lpString[in]
		A pointer to the string to be drawn.The string does not need to be zero - terminated, because cchString specifies the length of the string.
		cchString[in]
		The length of the string pointed to by lpString, in characters.
		Return value
		If the function succeeds, the return value is nonzero.
		If the function fails, the return value is zero.*/

		// Нарисовать текст
		// The EndPaint function marks the end of painting in the specified window. 
		// This function is required for each call to the BeginPaint function, but only after painting is complete.
		EndPaint(hWnd, &ps);
		// Освободить контекст окна
	}; break;

	// Нажата левая клавиша мыши в клиентской области окна.
	case WM_LBUTTONDOWN:
	{
		MessageBox(hWnd, L"32-bit aplication", L"Window",
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
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0L;
}

/*
- Оконная процедура отвечает за обработку всех сообщений, получаемых окном
- Оконную процедуру вызывает функция окна
7.	Может ли приложение создавать окна, принадлежащие классам, которые приложение самостоятельно не регистрировало?
-- Ответ: может, т.к. приложение создает локальные классы, а существуют так же глобальные и системные.
-- т.е. порядок такой:
-- Search the list of application local classes for a class with the specified name whose instance handle matches the module's instance handle. (Several modules can use the same name to register local classes in the same process.)
-- If the name is not in the application local class list, search the list of application global classes.
-- If the name is not in the application global class list, search the list of system classes.
8.	Что такое дескриптор окна? Для чего он необходим?
-- Идентификатор окна, через который можно управлять различными параметрами окна и его содержимым.
9.	После создания окна, оно обязательно должно появиться на экране? Что необходимо сделать для отображения окна?
-- Необязательно. ShowWindow(hWnd, SW_SHOWNORMAL); - С помощью этой функции можно его отобразить на экране
10.	Для чего служит цикл обработки сообщений? Откуда приложение извлекает очередное сообщение? Каким образом и кем сообщения создаются?
-- После создания и отображения окна функция WinMain должна подготовить приложение к получению
-- информации от пользователя через клавиатуру и мышь.
-- Windows поддерживает очередь сообщений (message queue) для каждой программы, работающей в
-- данный момент в системе. Когда происходит ввод информации, Windows преобразовывает ее в сообщение, которое
-- помещается в очередь сообщений приложения.
-- Программа извлекает сообщения из очереди сообщений, выполняя блок команд, известный как
-- цикл обработки сообщений (message loop).
11.	Сколько очередей сообщений существует? Для кого предназначены сообщения?
-- Цикл обработки начинается с извлечения сообщений из очереди при помощи функции
-- BOOL GetMessage(MSG FAR* lpmsg,HWND hwnd, UINT uMsgFilterMin,UINT uMsgFilterMax);
-- Если при вызове этой функции указать вторым аргументом NULL, то программа будет получать сообщения от всех окон, созданных программой.
-- т.е. у каждого окна
-- бля короче на потом вопрос 77777777777777777777777777
-- три топора
12.	Каким образом цикл обработки сообщений прерывается, давая возможность приложению завершить свою работу?
-- Когда в цикл посылается сообщение WM_QUIT цикл завершается.
-- В нашей программой это сообщение посылается функцией PostQuitMessage(0);
13.	Кто обычно посылает сообщение (и какое), которое прерывает цикл обработки сообщений?
-- Обычно его посылает пользователь, посылая сообщение WM_DESTROY, которое уничтожает окно
-- При уничтожении окна вызываем функцию PostQuitMessage(0), которая посылает в цикл сообщение о завершении цикла
14.	Что должно сделать приложение, если оно не обрабатывает некоторое сообщение?
-- Вернуть управление системе??? Скорее всего ждать очередного сообщения, цикл-то продолжает работать
-- короче три топора 77777777777777777777777777777
-- Все сообщения, не обрабатываемые оконной процедурой, должны передаваться функции
-- Windows DefWindowProc (такой механизм позволяет Windows обрабатывать окно совместно с приложением).
-- При этом значение, возвращаемое функцией DefWindowProc, должно быть возвращаемым значением оконной процедуры.
-- всё ещё три топора 777
15.	Из каких источников оконная процедура получает сообщения?
-- Функция окна получает сообщение из двух источников: из цикла обработки сообщений и от Windows:
-- Из цикла обработки сообщений поступают сообщения ввода: перемещение и нажатие клавиш мыши, нажатие и
-- отпускание клавиш клавиатуры и, если установлен генератор событий таймера, сообщения от таймера.
-- Windows посылает функции окна сообщения поддержки окна напрямую, минуя очередь приложения и
-- цикл обработки сообщений. Эти сообщения обычно вызваны событиями, требующими немедленной реакции по изменению вида окна.
16.	Какое сообщение приходит окну, если часть его рабочей области (или вся она) требует перерисовки? Что такое недействительная область?
-- WM_PAINT крайне важно для программирования под Windows. Оно сообщает программе, что часть или
-- вся рабочая область окна недействительна (invalid) и ее следует перерисовать.
17.	В каких ситуациях рабочая область может становиться недействительной?
-- минимизированное окно
-- перекрытие окнами

18.	С вызова какой функции должна начинаться обработка сообщения WM_PAINT? Вызовом какой функции она должна заканчиваться? Для чего вызываются эти функции?
-- начинается BeginPaint(hWnd, &ps);
-- заканчивается EndPaint
При обработке вызова BeginPaint, Windows:
	обновляет фон рабочей области с помощью кисти, которая указывалась при регистрации класса окна;
	делает всю рабочую область действительной (не требующей перерисовки);
	возвращает дескриптор контекста устройства (этот дескриптор необходим для вывода в рабочую область текста и графики).

-- При использовании дескриптора контекста устройства, возвращаемого функцией BeginPaint, приложение
-- не может рисовать вне рабочей области.
-- Функция EndPaint освобождает дескриптор устройства, после чего его нельзя использовать.

19.	Как обычно реагирует стандартное главное окно приложения на получение сообщения о его удалении? Что при этом происходит с приложением?
-- Главное окно стандартно реагирует на это сообщение, вызывая функцию
-- PostQuitMessage(0);
-- Эта функция ставит сообщение WM_QUIT в очередь сообщений приложения. Это заставляет
-- функцию WinMain прервать цикл обработки сообщений и выйти в систему, завершив работу приложения.
20.	Когда приходит сообщение об удалении окна? Что произойдет, если остальные окна приложения будут реагировать на это сообщение так же, как и главное окно?
-- Сообщение WM_DESTROY является еще одним важным сообщением.
-- Оно показывает, что Windows находится в процессе ликвидации окна в ответ на
-- полученную от пользователя команду (пользователь вызывает поступление этого сообщения, если нажмет
-- клавишей мыши по пиктограмме “Close”, выберет пункт “Close” из системного меню или нажмет комбинацию клавиш Alt+F4).
-- Главное окно стандартно реагирует на это сообщение, вызывая функцию
-- PostQuitMessage(0);


*/
