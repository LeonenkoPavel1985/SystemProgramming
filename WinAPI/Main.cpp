#include<Windows.h>
#include<string>
#include"resource.h"

CONST CHAR G_SZ_CLASS_NAME[] = "MyWindowClass";	//Имя класса окна

//https://docs.microsoft.com/en-us/windows/win32/dataxchg/about-atom-tables
ATOM RegisterWindowClass(WNDPROC wndProc, HINSTANCE hInstance);
HWND CreateWnd(HINSTANCE);
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	//hInstance - *.exe - модуль или *.dll-модуль программы.
	//hPrevInst - используется, когда приложение запущено несколько раз.
	//LPSTR lpCmdLine - параметры командной строки, с которыми запущено приложение.
	//					здесь можут быть открываемые файлы, ключи запуска и т.д.
	//int nCmdShow - состояние окна при запуске (свернуто на панель задач, развернуто на весь экран, и т.д.).
	////////////////////////////////////////////////////////////////////////////////////////////////////////

	//1) Регистрация класса окна:
	if (!RegisterWindowClass(WndProc, hInstance))
	{
		//MessageBox(NULL, "Class registration failed!", "Error", MB_OK | MB_ICONERROR);
		DWORD dwMessageID = GetLastError();	//Получаем код ошибки.
		LPSTR lpBuffer = NULL;		//Создаем буфер, в который сохраним сообщение об ошибке.
		FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM
			| FORMAT_MESSAGE_ALLOCATE_BUFFER //Выделяет память для буфера, в котором будет хранится сообщение об ошибке.
			| FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwMessageID,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&lpBuffer,
			1024,
			NULL
		);
		MessageBox(NULL, lpBuffer, "Error after RegisterWindowClass function", MB_OK | MB_ICONERROR);
		LocalFree(lpBuffer);	//Освобождаем память, выделенную функцией FormatMessage.
		return 0;
	}

	//2) Создание окна:
	HWND hwnd = CreateWnd(hInstance);
	if (hwnd == NULL)
	{
		//MessageBox(NULL, "Window creation failed", "Error", MB_OK | MB_ICONERROR);
		DWORD dwMessageID = GetLastError();	//Получаем код ошибки.
		LPSTR lpBuffer = NULL;		//Создаем буфер, в который сохраним сообщение об ошибке.
		FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM
			| FORMAT_MESSAGE_ALLOCATE_BUFFER //Выделяет память для буфера, в котором будет хранится сообщение об ошибке.
			| FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwMessageID,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&lpBuffer,
			1024,
			NULL
		);
		MessageBox(NULL, lpBuffer, "Error - Window creation failed", MB_OK | MB_ICONERROR);
		LocalFree(lpBuffer);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//3) The message loop:
	MSG msg;	//сообщение.
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

ATOM RegisterWindowClass(WNDPROC WndProc, HINSTANCE hInstance)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);	//cb - count bytes
	wc.style = 0;
	wc.lpfnWndProc = WndProc;//lpfn - LongPointer to Function (Указатель на функцию).
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;	//Дополнительная память для класса и для окна.
	wc.hInstance = hInstance;//Какому *.exe-модулю будет принадлежать окно.
	//wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	//wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));//Small icon
	wc.hIcon = (HICON)LoadImage(NULL, "Network.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));//Small icon
	//wc.hIcon = (HICON)LoadImage(NULL, "Network.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE),
	//wc.hIconSm = (HICON)LoadImage(NULL, "Save.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE),

	//wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDB_PNG1));
	wc.hCursor = LoadCursorFromFile("light.cur");
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = G_SZ_CLASS_NAME;
	wc.lpszMenuName = NULL;

	ATOM atom = RegisterClassEx(&wc);

	//if (!atom)
	//{
	//	//MessageBox(NULL, "Class registration failed!", "Error", MB_OK | MB_ICONERROR);
	//	DWORD dwMessageID = GetLastError();	//Получаем код ошибки.
	//	LPSTR lpBuffer = NULL; //Создаем буфер, в который сохраним сообщение об ошибке.
	//	FormatMessage(
	//		FORMAT_MESSAGE_FROM_SYSTEM
	//		| FORMAT_MESSAGE_IGNORE_INSERTS
	//		| FORMAT_MESSAGE_ALLOCATE_BUFFER, //Выделяет память для буфера, в котором будет хранится сообщение об ошибке.
	//		NULL,
	//		dwMessageID,
	//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	//		(LPSTR)&lpBuffer,//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//		1024,
	//		NULL
	//	);
	//	MessageBox(NULL, lpBuffer, "Error", MB_OK | MB_ICONERROR);

	//	LocalFree(lpBuffer);
	//	
	//	//MessageBox(NULL, "Class registration failed!", "Error", MB_OK | MB_ICONERROR);
	//}
	return atom;
}
HWND CreateWnd(HINSTANCE hInstance)
{
	int screen_width = GetSystemMetrics(SM_CXSCREEN);//Получаем разрешение экрана по ширине.
	int screen_height = GetSystemMetrics(SM_CYSCREEN);//Получаем разрешение экрана по ширине.
	int window_width = screen_width - screen_width / 4;
	int window_height = screen_height - screen_height / 4;

	HWND hwnd = CreateWindowEx//Возвращает HWND созданного окна, если окно не быо создано, функция возвращает NULL.
	(
		WS_EX_CLIENTEDGE,
		G_SZ_CLASS_NAME,//Имя класса окна.
		G_SZ_CLASS_NAME,//Заголовок окна.
		WS_OVERLAPPEDWINDOW,//Стиль окна WS_OVERLAPPEDWINDOW - это главное окно приложения.
		screen_width / 8, screen_height / 8,//100, 100,//Начальная позиция, эти пиксели определяют положение верхнего левого угла.
		window_width, window_height,//Размер окна в пикселях.
		NULL,//Родительское окно.
		NULL,//Menu.
		hInstance,
		NULL
	);
	return hwnd;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		//Создаются и инициализируются элементы окна.
		break;
	case WM_MOVE:
	case WM_SIZE:
	{
		const int SIZE = 256;
		char title[SIZE] = {};// "My Window   Size:%dx%d, Position: %dx%d";
		strcpy(title, G_SZ_CLASS_NAME);


		RECT rect;		//Прямоугольник окна
		GetWindowRect(hwnd, &rect);
		int h_size = rect.right - rect.left;	//Вычисляем размер окна по горизонтали.
		int v_size = rect.bottom - rect.top;	//Вычисляем размер окна по вертикали.

		/*//Преобразуем размеры в строки:
		char sz_h_size[10] = {};
		char sz_v_size[10] = {};
		_itoa(h_size, sz_h_size, 10);
		_itoa(v_size, sz_v_size, 10);
		//Добавляем размеры в строку с заголовком окна:
		strcat(title, " Size: ");
		strcat(title, sz_h_size);
		strcat(title, "x");
		strcat(title, sz_v_size);
		//Добавляем координаты окна:
		char sz_x_position[10] = {};
		char sz_y_position[10] = {};
		_itoa(rect.left, sz_x_position, 10);
		_itoa(rect.top, sz_y_position, 10);
		strcat(title, " Poisition: ");
		strcat(title, sz_x_position);
		strcat(title, "x");
		strcat(title, sz_y_position);*/

		wsprintf(title, "%s Size:%dx%d, Position : %dx%d", G_SZ_CLASS_NAME, h_size, v_size, rect.left, rect.top);
		SetWindowText(hwnd, title);
	}
	break;
	case WM_CLOSE:
	{
		int answer = MessageBox(hwnd, "Вы действительно хотите закрыть окно?", "Question", MB_ICONWARNING | MB_YESNO);
		if (answer == IDYES)
			DestroyWindow(hwnd);//Деактивирует окно и убирает фокус с указанного окна.
		//Фокус - часть окна, которая принимает команды с клавиатуры.
	}
	break;
	case WM_DESTROY:
		//MessageBox(hwnd, "Ну и ладно...", "Info", MB_ICONINFORMATION);
		PostQuitMessage(0);	//Сигнализирует системе, что поток (процесс) завершается.
		break;
	default:return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}