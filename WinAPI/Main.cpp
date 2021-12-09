#include <Windows.h>
#include "resource.h"

CONST CHAR G_SZ_CLASS_NAME[] = "MyWindowClass"; //Имя класса окна 

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCMDLine, int nCmdShow)
{
	//hIbstance - *.exe - модуль или *.dll-модуль программы.
	//hPrevInst - используется, когда приложение запущено несколько раз.
	//LPSTR lpCMDLine - параметры командной строки, с которыми запущено приложение.
	//                                      здесь могут быть открываемые файлы, ключи запуска и т.д.
	//int nCmdShow - состояние окна при запуске (свернуто на панель задач, развернуто на весь экран т т.д.)
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	//1) Регистрация класса окна:
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX); // cb - count bytes
	wc.style = 0;
	wc.lpfnWndProc = WndProc; // lpfn - LongPointer to Function (Указатель на функцию).
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0; // Дополнительная память для класс и для окна.
	wc.hInstance = hInstance; // Какому *.exe-модулю будет принадлежать окно.
	//wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	//wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE (IDI_ICON1));// Small icon
	wc.hIcon = (HICON)LoadImage(NULL, "Network.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE),
	wc.hIconSm = (HICON)LoadImage(NULL, "Save.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE),
	
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW); 
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = G_SZ_CLASS_NAME;
	wc.lpszMenuName = NULL;


	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Class registration failed!", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	//2) Создание окна:

	int screen_width = GetSystemMetrics(SM_CXSCREEN); // Получаем разрешение экрана шире.
	int screen_height = GetSystemMetrics(SM_CYSCREEN); // Получаем разрешение экрана шире.
	int window_width = screen_width - screen_width / 4;
	int window_height = screen_height - screen_height / 4;

	HWND hwnd = CreateWindowEx // Возвращает HWND созданного окна, усли окно не было создано, функция возвращает NULL.
	(
		WS_EX_CLIENTEDGE,
		G_SZ_CLASS_NAME, // Имя класса окна.
		G_SZ_CLASS_NAME, // Заголовок окна.
		WS_OVERLAPPEDWINDOW, // Стиль окна WS_OVERLAPPEDWINDOW - это главное окно приложения.
		screen_width / 8, screen_height /8, // 100, 100, // Начальная позиция, эти пиксели определяют положение верхнего левого угла.
		window_width,window_height, // Размер окна в пикселях.
		NULL, //Родительское окно
		NULL, //Menu
		hInstance,
		NULL
	);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Class registration failed!", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// 3) The message loop:
	MSG msg; // Сообщение.
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		//Создаются и инициализируются элементы окна.
		break;
	case WM_CLOSE:
	{
		int answer = MessageBox(hwnd, "Вы действительно хотите закрыть окно?", "Question", MB_ICONWARNING | MB_YESNO);
		if (answer == IDYES)
			DestroyWindow(hwnd); //Деактивирует окно и убирает фокус с указанного окна.
			//Фокус - часть окна, которая принимает команды с клавиатуры.
		break;
	}
	case WM_DESTROY:
		//MessageBox(hwnd, "Нум ладно...", "Info", MB_ICONINFORMATION);
		PostQuitMessage(0); // Сигнализирует системе, что поток (процесс) завершается.
		break;
	default:return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}