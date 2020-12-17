#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif


#include <tchar.h>
#include <Windows.h>
#include <windowsx.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cassert>

#include "../include/myfunctions.h"
#include "../include/MyBitmap.h"
#include "../resource.h"


/*  Declare Windows procedure  */

HWND hwnd;
HFONT hFont;

const wchar_t* szWindowClass = L"WiT";
const wchar_t* szButtonClass = L"MyButton";

int red = 0;
int my_counter = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


struct MyButtonData {
    bool down = false;
    int yourData = 0;
};

bool isInsideRect(RECT& rc, LONG x, LONG y) {
    return (x >= rc.left && x <= rc.right && y >= rc.top && y <= rc.bottom);
}

LRESULT CALLBACK ButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


void initButtonClass(HINSTANCE hInstance) {
    WNDCLASSEX wcex;

    my_bzero(wcex);
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = ButtonProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szButtonClass;
    wcex.hIconSm = nullptr;

    RegisterClassEx(&wcex);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    //Window Class
    WNDCLASSEX wcex;
    //ZeroMemory(&wcex, sizeof(wcex)); // bzero
    //memset(&wcex, 0 sizeof(wcex));

    my_bzero(wcex);
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    //wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = nullptr;

    RegisterClassEx(&wcex);

    int scrx = GetSystemMetrics(SM_CXSCREEN);
    int scry = GetSystemMetrics(SM_CYSCREEN);
    int windx = 600;
    int windy = 400;
    int scrcentralx = (scrx / 2) - (windx / 2);
    int scrcentraly = (scry / 2) - (windy / 2);

    //----Window----------
    hwnd = CreateWindowEx(0,
        szWindowClass,
        L"MyWindow Title",
        WS_OVERLAPPEDWINDOW,
        //WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, /* default window */
        scrcentralx,       /* Windows decides the position */
        scrcentraly,       /* where the window ends up on the screen */
        windx,                 /* The programs width */
        windy,                 /* and height in pixels */
        nullptr,
        nullptr,
        hInstance,
        0);
    //--- Button ------
    initButtonClass(hInstance);
    int window_grid = 25;
    int button_size_x = 175;
    int button_size_y = 25;
    float button_level_in_windows = 0.6;

    CreateWindowEx(0, szButtonClass, L"CENTRAL", WS_CHILD | WS_VISIBLE, ((windx - button_size_x) / 2), windy * button_level_in_windows, button_size_x, button_size_y, hwnd, (HMENU)1, hInstance, nullptr);
    CreateWindowEx(0, szButtonClass, L"LEFT", WS_CHILD | WS_VISIBLE, ((windx - 2 * button_size_x - window_grid) / 2), (windy * button_level_in_windows + 2 * window_grid), button_size_x, button_size_y, hwnd, (HMENU)2, hInstance, nullptr);
    CreateWindowEx(0, szButtonClass, L"RIGHT", WS_CHILD | WS_VISIBLE, (((windx - 2 * button_size_x - window_grid) / 2) + window_grid + button_size_x), (windy * button_level_in_windows + 2 * window_grid), button_size_x, button_size_y, hwnd, (HMENU)3, hInstance, nullptr);

    //CreateWindowEx(0, L"Button", L"Test", WS_CHILD | WS_VISIBLE, 10, 100, 200, 40, hwnd, (HMENU)10, hInstance, nullptr);
    //CreateWindowEx(0, L"Edit", L"Text Input...", WS_CHILD | WS_VISIBLE | WS_BORDER, 220, 100, 200, 40, hwnd, nullptr, hInstance, nullptr);

    //-----------------------
    ShowWindow(hwnd, SW_NORMAL);
    UpdateWindow(hwnd);

    // Main message loop:  
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{    
    switch (message)
    {
   
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK ButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int flag_loging_window = 1; /* 1-log, 2-registration, 3-password remind*/

    switch (message)
    {
    case WM_CREATE: {

        auto* data = new MyButtonData();
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<intptr_t>(data));
    }break;

    case WM_DESTROY: {
        auto* data = reinterpret_cast<MyButtonData*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        delete data;
    }break;

    case WM_PAINT: {
        auto* data = reinterpret_cast<MyButtonData*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        PAINTSTRUCT ps;
        BeginPaint(hWnd, &ps);

        RECT rc;
        HBRUSH hBrush;
        GetClientRect(hWnd, &rc);
        Rectangle(ps.hdc, rc.left, rc.top, rc.right, rc.bottom);
        hFont = CreateFont(20, 0, 0, 0, NULL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, TEXT("Calibri Light"));
        SelectObject(ps.hdc, hFont);

        if (data->down)
        {
            SetTextColor(ps.hdc, RGB(240, 240, 240));
            hBrush = ::CreateSolidBrush(RGB(0, 72, 0));
            ::FillRect(ps.hdc, &rc, hBrush);
            hFont = CreateFont(20, 0, 0, 0, 500, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, TEXT("Calibri Light"));
            SelectObject(ps.hdc, hFont);
        }
        else
        {
            SetTextColor(ps.hdc, RGB(256, 256, 256));
            hFont = CreateFont(20, 0, 0, 0, NULL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, TEXT("Calibri Light"));
            SelectObject(ps.hdc, hFont);
            SelectObject(ps.hdc, GetStockObject(WHITE_BRUSH));
        }

        wchar_t text[512];
        GetWindowText(hWnd, text, 512);

        switch (flag_loging_window)
        {
        case 1:
        {
            if (wcscmp(text, L"CENTRAL") == 0)
            {
                SetBkMode(ps.hdc, TRANSPARENT);
                DrawText(ps.hdc, L"zaloguj", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            }
            else if (wcscmp(text, L"LEFT") == 0)
            {
                SetBkMode(ps.hdc, TRANSPARENT);
                DrawText(ps.hdc, L"utwórz konto", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            }
            else if (wcscmp(text, L"RIGHT") == 0)
            {
                SetBkMode(ps.hdc, TRANSPARENT);
                DrawText(ps.hdc, L"przypomnij has³o", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            }
        }break;

        case 2:
        {
            if (wcscmp(text, L"CENTRAL") == 0)
            {
                SetBkMode(ps.hdc, TRANSPARENT);
                DrawText(ps.hdc, L"utwórz konto", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            }
            else if (wcscmp(text, L"LEFT") == 0)
            {
                SetBkMode(ps.hdc, TRANSPARENT);
                DrawText(ps.hdc, L"zaloguj", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            }
            else if (wcscmp(text, L"RIGHT") == 0)
            {
                SetBkMode(ps.hdc, TRANSPARENT);
                DrawText(ps.hdc, L"przypomnij has³o", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            }

        }break;

        case 3:
        {
            if (wcscmp(text, L"CENTRAL") == 0)
            {
                SetBkMode(ps.hdc, TRANSPARENT);
                DrawText(ps.hdc, L"przypomnij has³o", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            }
            else if (wcscmp(text, L"LEFT") == 0)
            {
                SetBkMode(ps.hdc, TRANSPARENT);
                DrawText(ps.hdc, L"zaloguj", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            }
            else if (wcscmp(text, L"RIGHT") == 0)
            {
                SetBkMode(ps.hdc, TRANSPARENT);
                DrawText(ps.hdc, L"utwórz konto", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            }
        }break;

        }

        EndPaint(hWnd, &ps);

    }break;

    case WM_LBUTTONDOWN: {
        auto* data = reinterpret_cast<MyButtonData*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        data->down = true;
        SetCapture(hWnd);
        InvalidateRect(hWnd, nullptr, false);
    }break;

    case WM_LBUTTONUP: {
        auto* data = reinterpret_cast<MyButtonData*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        data->down = false;
        ReleaseCapture();
        InvalidateRect(hWnd, nullptr, false);

        auto x = GET_X_LPARAM(lParam);
        auto y = GET_Y_LPARAM(lParam);

        RECT rc;
        GetClientRect(hWnd, &rc);

        if (isInsideRect(rc, x, y)) 
        {
            SendMessage(hWnd, WM_COMMAND, (WPARAM)GetMenu(hWnd), 0);
        }
    }break;

    case WM_COMMAND:
    {
        switch (flag_loging_window)
        {
        case 1:
        {
            switch (wParam)
            {
            case 1:
            {
                ::MessageBox(hwnd, L"LOGUJ", L"", MB_OK);
                flag_loging_window = 1;
                InvalidateRect(GetParent(hWnd), NULL, FALSE);
            }break;

            case 2:
            {
                ::MessageBox(hwnd, L"UTWÓRZ KONTO", L"", MB_OK);
                flag_loging_window = 2;
                InvalidateRect(GetParent(hWnd), NULL, FALSE);
            }break;

            case 3:
            {
                ::MessageBox(hwnd, L"PRZYPOMNIJ HAS£O", L"", MB_OK);
                flag_loging_window = 3;
                InvalidateRect(GetParent(hWnd), NULL, FALSE);
            }break;
            }// switch (wParam)
        }break;

        case 2:
        {
            switch (wParam)
            {
            case 1:
            {
                ::MessageBox(hwnd, L"UTWÓRZ KONTO", L"", MB_OK);
                flag_loging_window = 2;
                InvalidateRect(GetParent(hWnd), NULL, FALSE);
            }break;

            case 2:
            {
                ::MessageBox(hwnd, L"LOGUJ", L"", MB_OK);
                flag_loging_window = 1;
                InvalidateRect(GetParent(hWnd), NULL, FALSE);
            }break;

            case 3:
            {
                ::MessageBox(hwnd, L"PRZYPOMNIJ HAS£O", L"", MB_OK);
                flag_loging_window = 3;
                InvalidateRect(GetParent(hWnd), NULL, FALSE);
            }break;
            }// switch (wParam)

        }break;

        case 3:
        {
            switch (wParam)
            {
            case 1:
            {
                ::MessageBox(hwnd, L"PRZYPOMNIJ HAS£O", L"", MB_OK);
                flag_loging_window = 3;
                InvalidateRect(GetParent(hWnd), NULL, FALSE);
            }break;

            case 2:
            {
                ::MessageBox(hwnd, L"LOGUJ", L"", MB_OK);
                flag_loging_window = 1;
                InvalidateRect(GetParent(hWnd), NULL, FALSE);
            }break;

            case 3:
            {
                ::MessageBox(hwnd, L"UTWÓRZ KONTO", L"", MB_OK);
                flag_loging_window = 2;
                InvalidateRect(GetParent(hWnd), NULL, FALSE);
            }break;
            }// switch (wParam)
        }break;
        }  //flag
    }break;//COMMAND
    

    } // switch    
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int main() {
    return WinMain(GetModuleHandle(nullptr), nullptr, GetCommandLineA(), SW_NORMAL);
}