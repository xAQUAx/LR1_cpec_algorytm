﻿// WindowsProject6.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "WindowsProject6.h"
#include "Figure.h"
#include <chrono>
#include "Windows.h"
#include <vector>
#include <iostream>

typedef std::string(*LPGETPLUGINFIGURENAME)();
typedef std::shared_ptr<Figure>(*LPGETPLUGINFIGURE)();

std::wstring DllsPath()
{
    WCHAR buffer[MAX_PATH]{ 0 };
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
    return ((std::wstring(buffer).substr(0, pos)).append(L"\\*.dll"));
}

std::vector<std::pair<int, HMODULE>> EnumeratePlugin()
{
    std::vector<std::pair<int, HMODULE>> result;
    int index = 1;
    WIN32_FIND_DATA wfd;
    HANDLE hSearchHandle;

    std::wstring searchTemplate = DllsPath();
    hSearchHandle = FindFirstFile(searchTemplate.c_str(), &wfd);
    if (hSearchHandle == INVALID_HANDLE_VALUE)
        return result;

    do
    {
        HMODULE dll = LoadLibrary(wfd.cFileName);
        if (dll == NULL)
        {
            continue;
        }

        if (GetProcAddress(dll, "GetPluginFigureName") != NULL)
        {
            result.push_back(std::make_pair(index, dll));
            index++;
        }
        else
        {
            FreeLibrary(dll);
        }
    } while (FindNextFile(hSearchHandle, &wfd) != 0);

    FindClose(hSearchHandle);

    return result;
}

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

std::vector<std::shared_ptr<Figure>> figures;
std::vector<std::pair<int, HMODULE>> dlls = EnumeratePlugin();
std::chrono::steady_clock::time_point prevFrameTime;

void Initialization()
{
    HMODULE activeModule = NULL;
    for (auto pluginInfo : dlls)
    {
        if (pluginInfo.second != NULL)
        {
            activeModule = pluginInfo.second;
            LPGETPLUGINFIGURE GetPluginFigure = (LPGETPLUGINFIGURE)GetProcAddress(activeModule, "GetPluginFigure");
            figures.push_back(GetPluginFigure());
        }

    }
}

void removeDllsFigures()
{
    figures.clear();
    for (auto dll : dlls)
    {
        FreeLibrary(dll.second);
    }
}

void OnIdle()
{
  auto curTime = std::chrono::steady_clock::now();
  auto timeMks = std::chrono::duration_cast<std::chrono::microseconds>(curTime - prevFrameTime).count();
  prevFrameTime = curTime;
  for (auto f : figures)
  {
      f->Move(timeMks);
  }
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT6, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    Initialization();


    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT6));

    MSG msg;
        
    prevFrameTime = std::chrono::steady_clock::now();

    // Цикл основного сообщения:
    while (true)
    {
      if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
      {
        if (msg.message == WM_QUIT)
          break;
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }
      }
      else
      {
        OnIdle();
        InvalidateRect(msg.hwnd, NULL, TRUE);
        Sleep(10);
      }
    }
    
    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT6));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT6);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                removeDllsFigures();
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...

            for (auto f : figures)
            {
                f->Draw(hdc);
            }

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_SIZE:
      {
        long width = LOWORD(lParam);
        long height = HIWORD(lParam);

        for (auto f : figures)
        {
            f->SetBounds({ 0, 0, width, height });
        }
        break;
      }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
