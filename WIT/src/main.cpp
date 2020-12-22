
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


//  Declare Windows procedure  

HWND hwnd;
HFONT hFont;

int red = 0;
int my_counter = 0;

int scrx = GetSystemMetrics(SM_CXSCREEN);
int scry = GetSystemMetrics(SM_CYSCREEN);
int windx = 600;
int windy = 400;
int scrcentralx = (scrx / 2) - (windx / 2);
int scrcentraly = (scry / 2) - (windy / 2);
int window_grid = 25;
int button_size_x = 175;
int button_size_y = 25;


struct LogWinButtonData {
    bool down = false;
    int yourData = 0;
};

bool isInsideRect(RECT& rc, LONG x, LONG y) {
    return (x >= rc.left && x <= rc.right && y >= rc.top && y <= rc.bottom);
}

LRESULT CALLBACK LogWin(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LogButton(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void initLogButtonClass(HINSTANCE hInstance) {
    WNDCLASSEX wcex;
    my_bzero(wcex);
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = LogButton;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"LogButtonClass";
    wcex.hIconSm = nullptr;

    RegisterClassEx(&wcex);
}

void initLogWinClass(HINSTANCE hInstance) {
    WNDCLASSEX wcex;
    my_bzero(wcex);
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = LogWin;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"LogWinClass";
    wcex.hIconSm = nullptr;

    RegisterClassEx(&wcex);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    //----LogWIn----------
    initLogWinClass(hInstance);
    hwnd = CreateWindowEx(0,L"LogWinClass",L"WiT",WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,scrcentralx,scrcentraly,windx,windy,nullptr,nullptr,hInstance,0);
    //--- LogWinButton ------
    initLogButtonClass(hInstance);
    CreateWindowEx(0, L"LogButtonClass", L"CENTRAL", WS_CHILD | WS_VISIBLE, ((windx - button_size_x) / 2), (windy -6*window_grid), button_size_x, button_size_y, hwnd, (HMENU)1, hInstance, nullptr);
    CreateWindowEx(0, L"LogButtonClass", L"LEFT", WS_CHILD | WS_VISIBLE, ((windx - 2 * button_size_x - window_grid) / 2), (windy - 6*window_grid + 2 * window_grid), button_size_x, button_size_y, hwnd, (HMENU)2, hInstance, nullptr);
    CreateWindowEx(0, L"LogButtonClass", L"RIGHT", WS_CHILD | WS_VISIBLE, (((windx - 2 * button_size_x - window_grid) / 2) + window_grid + button_size_x), (windy - 6*window_grid + 2 * window_grid), button_size_x, button_size_y, hwnd, (HMENU)3, hInstance, nullptr);

    //CreateWindowEx(0, L"Button", L"Test", WS_CHILD | WS_VISIBLE, 10, 100, 200, 40, hwnd, (HMENU)10, hInstance, nullptr);
    CreateWindowEx(0, L"Edit", L"Text Input...", WS_CHILD | WS_VISIBLE | WS_BORDER, ((windx - button_size_x) / 2), 100, 200, 40, hwnd, nullptr, hInstance, nullptr);

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

int flag_LogWin = 1; // 4-servlogin, 1-acclogin, 2-registration, 3-password_remind

LRESULT CALLBACK LogWin(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{    
    switch (message)
    {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        BeginPaint(hWnd, &ps);

        HBRUSH hBrush;
        hBrush = ::CreateSolidBrush(RGB(0, 72, 0));

        RECT rc;
        SetRect(&rc, ((windx - button_size_x) / 2), window_grid, ((windx - button_size_x) / 2)+button_size_x, window_grid+button_size_y);
        //::FillRect(ps.hdc, &rc, hBrush);

        hFont = CreateFont(20, 0, 0, 0, 300, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, TEXT("Calibri"));
        SelectObject(ps.hdc, hFont);
        SetTextColor(ps.hdc, RGB(0, 72, 0));
        SetBkMode(ps.hdc, TRANSPARENT);
        DrawText(ps.hdc, L"przypomnij has這", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

        SetRect(&rc, ((windx - button_size_x) / 2), 3*window_grid, ((windx - button_size_x) / 2) + button_size_x, 3*window_grid + button_size_y);
        DrawText(ps.hdc, L"dupa", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);



        EndPaint(hWnd, &ps);

    }break;//WM_PAINT

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK LogButton(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE: {
        auto* data = new LogWinButtonData();
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<intptr_t>(data));
    }break;

    case WM_DESTROY: {
        auto* data = reinterpret_cast<LogWinButtonData*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        delete data;
    }break;

    case WM_PAINT: {
        auto* data = reinterpret_cast<LogWinButtonData*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        PAINTSTRUCT ps;
        BeginPaint(hWnd, &ps);

        RECT rc;
        HBRUSH hBrush;
        GetClientRect(hWnd, &rc);
        Rectangle(ps.hdc, rc.left, rc.top, rc.right, rc.bottom);

        if (data->down)
        {
            SetTextColor(ps.hdc, RGB(240, 240, 240));
            hBrush = ::CreateSolidBrush(RGB(0, 72, 0));
            ::FillRect(ps.hdc, &rc, hBrush);
            hFont = CreateFont(20, 0, 0, 0, 700, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, TEXT("Calibri"));
            SelectObject(ps.hdc, hFont);
        }
        else
        {
            SetTextColor(ps.hdc, RGB(0, 72, 0));
            hFont = CreateFont(20, 0, 0, 0, 300, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, TEXT("Calibri"));
            SelectObject(ps.hdc, hFont);
            SelectObject(ps.hdc, GetStockObject(WHITE_BRUSH));
        }

        wchar_t text[512];
        GetWindowText(hWnd, text, 512);
        SetBkMode(ps.hdc, TRANSPARENT);

        switch (flag_LogWin)
        {
            case 1:
            {
                if (wcscmp(text, L"CENTRAL") == 0)
                {
                    DrawText(ps.hdc, L"zaloguj", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                }
                else if (wcscmp(text, L"LEFT") == 0)
                {
                    DrawText(ps.hdc, L"utw鏎z konto", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                }
                else if (wcscmp(text, L"RIGHT") == 0)
                {
                    DrawText(ps.hdc, L"przypomnij has這", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                }
            }break;

            case 2:
            {
                if (wcscmp(text, L"CENTRAL") == 0)
                {
                    DrawText(ps.hdc, L"utw鏎z konto", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                }
                else if (wcscmp(text, L"LEFT") == 0)
                {
                    DrawText(ps.hdc, L"zaloguj", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                }
                else if (wcscmp(text, L"RIGHT") == 0)
                {
                    DrawText(ps.hdc, L"przypomnij has這", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                }

            }break;

            case 3:
            {
                if (wcscmp(text, L"CENTRAL") == 0)
                {
                    DrawText(ps.hdc, L"przypomnij has這", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                }
                else if (wcscmp(text, L"LEFT") == 0)
                {
                    DrawText(ps.hdc, L"zaloguj", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                }
                else if (wcscmp(text, L"RIGHT") == 0)
                {
                    DrawText(ps.hdc, L"utw鏎z konto", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                }
            }break;

        }

        EndPaint(hWnd, &ps);

    }break;//WM_PAINT

    case WM_LBUTTONDOWN: {
        auto* data = reinterpret_cast<LogWinButtonData*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        data->down = true;
        SetCapture(hWnd);
        InvalidateRect(hWnd, nullptr, false);
    }break;//WM_LBUTTONDOWN

    case WM_LBUTTONUP: {
        auto* data = reinterpret_cast<LogWinButtonData*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
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
    }break;//WM_LBUTTONUP

    case WM_COMMAND:
    {
        switch (flag_LogWin)
        {
            case 1:
            {
                switch (wParam)
                {
                    case 1:
                    {
                        ::MessageBox(hwnd, L"LOGUJ", L"", MB_OK);
                        flag_LogWin = 1;
                        InvalidateRect(GetParent(hWnd), NULL, FALSE);
                    }break;

                    case 2:
                    {
                        ::MessageBox(hwnd, L"UTW紑Z KONTO", L"", MB_OK);
                        flag_LogWin = 2;
                        InvalidateRect(GetParent(hWnd), NULL, FALSE);
                    }break;

                    case 3:
                    {
                        ::MessageBox(hwnd, L"PRZYPOMNIJ HASΜ", L"", MB_OK);
                        flag_LogWin = 3;
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
                        ::MessageBox(hwnd, L"UTW紑Z KONTO", L"", MB_OK);
                        flag_LogWin = 2;
                        InvalidateRect(GetParent(hWnd), NULL, FALSE);
                    }break;

                    case 2:
                    {
                        ::MessageBox(hwnd, L"LOGUJ", L"", MB_OK);
                        flag_LogWin = 1;
                        InvalidateRect(GetParent(hWnd), NULL, FALSE);
                    }break;

                    case 3:
                    {
                        ::MessageBox(hwnd, L"PRZYPOMNIJ HASΜ", L"", MB_OK);
                        flag_LogWin = 3;
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
                        ::MessageBox(hwnd, L"PRZYPOMNIJ HASΜ", L"", MB_OK);
                        flag_LogWin = 3;
                        InvalidateRect(GetParent(hWnd), NULL, FALSE);
                    }break;

                    case 2:
                    {
                        ::MessageBox(hwnd, L"LOGUJ", L"", MB_OK);
                        flag_LogWin = 1;
                        InvalidateRect(GetParent(hWnd), NULL, FALSE);
                    }break;

                    case 3:
                    {
                        ::MessageBox(hwnd, L"UTW紑Z KONTO", L"", MB_OK);
                        flag_LogWin = 2;
                        InvalidateRect(GetParent(hWnd), NULL, FALSE);
                    }break;
                }// switch (wParam)
            }break;
        }  //flag
    }break;//WM_COMMAND
    

    } // switch    
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int main() {
    return WinMain(GetModuleHandle(nullptr), nullptr, GetCommandLineA(), SW_NORMAL);
}


/*#include <tchar.h>
#include <Windows.h>
#include <windowsx.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cassert>

#include "../include/myfunctions.h"
#include "../include/MyBitmap.h"
#include "../resource.h"
#include "MyWindow.cpp"

int flag_loging_window = 1; // 1-log, 2-registration, 3-password remind

struct MyButtonData {
    bool down = false;
    int yourData = 0;
};

bool isInsideRect(RECT& rc, LONG x, LONG y) {
    return (x >= rc.left && x <= rc.right && y >= rc.top && y <= rc.bottom);
}

class LogClass : public MyWindow<LogClass> {
public:
    PCWSTR  ClassName() const { return L"LogClass"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

};

class ButtonClass : public MyWindow<ButtonClass> {
public:
    PCWSTR  ClassName() const { return L"ButtonClass"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

};

class ZapasoweClass : public MyWindow<ZapasoweClass> {
public:
    PCWSTR  ClassName() const { return L"ZapasoweClass"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR pCmdLine, int nCmdShow)
{
    LogClass logWin;
    int scrx = GetSystemMetrics(SM_CXSCREEN);
    int scry = GetSystemMetrics(SM_CYSCREEN);
    int windx = 600;
    int windy = 400;
    int scrcentralx = (scrx / 2) - (windx / 2);
    int scrcentraly = (scry / 2) - (windy / 2);
    if (!logWin.CreateWindows(L"WiT-Logowanie", WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, scrcentralx, scrcentraly, windx, windy)) { return 0; }
    ShowWindow(logWin.Window(), SW_NORMAL);

    ButtonClass buttonWin;
    int window_grid = 25;
    int button_size_x = 175;
    int button_size_y = 25;
    float button_level_in_windows = 0.6;
    if (!buttonWin.CreateWindows(L"CENTRAL",WS_CHILD | WS_VISIBLE, ((windx - button_size_x) / 2), windy - 2*window_grid, button_size_x, button_size_y, logWin.Window(), (HMENU)1)) { return 0; }
    
    ShowWindow(buttonWin.Window(), SW_NORMAL);

    ZapasoweClass zapasoweWin;
    if (!zapasoweWin.CreateWindows(L"Zapasowe", WS_OVERLAPPEDWINDOW)) { return 0; }
    //ShowWindow(zapasoweWin.Window(), nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}


//------------------------------------------HANDLE


LRESULT LogClass::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(m_hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(m_hwnd, &ps);
    }
    return 0;

    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}


LRESULT ButtonClass::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    switch (uMsg)
    {
        /*case WM_CREATE: {

            auto* data = new MyButtonData();
            SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<intptr_t>(data));
        }break;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(m_hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW));
            EndPaint(m_hwnd, &ps);
        }break;

        /* case WM_LBUTTONDOWN: {
             auto* data = reinterpret_cast<MyButtonData*>(GetWindowLongPtr(m_hwnd, GWLP_USERDATA));
             data->down = true;
             SetCapture(m_hwnd);
             InvalidateRect(m_hwnd, nullptr, false);
         }break;

         case WM_LBUTTONUP: {
             auto* data = reinterpret_cast<MyButtonData*>(GetWindowLongPtr(m_hwnd, GWLP_USERDATA));
             data->down = false;
             ReleaseCapture();
             InvalidateRect(m_hwnd, nullptr, false);

             auto x = GET_X_LPARAM(lParam);
             auto y = GET_Y_LPARAM(lParam);

             RECT rc;
             GetClientRect(m_hwnd, &rc);

             if (isInsideRect(rc, x, y))
             {
                 SendMessage(m_hwnd, WM_COMMAND, (WPARAM)GetMenu(m_hwnd), 0);
             }
         }break;
         
    }
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);


    /*switch (uMsg)
    {
    case WM_CREATE: {

        auto* data = new MyButtonData();
        SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<intptr_t>(data));
    }break;

    case WM_DESTROY: {
        auto* data = reinterpret_cast<MyButtonData*>(GetWindowLongPtr(m_hwnd, GWLP_USERDATA));
        delete data;
    }break;

    case WM_PAINT: {
        auto* data = reinterpret_cast<MyButtonData*>(GetWindowLongPtr(m_hwnd, GWLP_USERDATA));
        PAINTSTRUCT ps;
        BeginPaint(m_hwnd, &ps);

        RECT rc;
        HBRUSH hBrush;
        GetClientRect(m_hwnd, &rc);
        Rectangle(ps.hdc, rc.left, rc.top, rc.right, rc.bottom);
        HFONT hFont = CreateFont(20, 0, 0, 0, NULL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, TEXT("Calibri Light"));
        SelectObject(ps.hdc, hFont);

        if (data->down)
        {
            SetTextColor(ps.hdc, RGB(240, 240, 240));
            hBrush = ::CreateSolidBrush(RGB(0, 72, 0));
            ::FillRect(ps.hdc, &rc, hBrush);
            hFont = CreateFont(20, 0, 0, 0, 500, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, TEXT("Calibri Light"));
            SelectObject(ps.hdc, hFont);
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
        GetWindowText(m_hwnd, text, 512);

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
                DrawText(ps.hdc, L"utw鏎z konto", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            }
            else if (wcscmp(text, L"RIGHT") == 0)
            {
                SetBkMode(ps.hdc, TRANSPARENT);
                DrawText(ps.hdc, L"przypomnij has這", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            }
        }break;

        case 2:
        {
            if (wcscmp(text, L"CENTRAL") == 0)
            {
                SetBkMode(ps.hdc, TRANSPARENT);
                DrawText(ps.hdc, L"utw鏎z konto", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            }
            else if (wcscmp(text, L"LEFT") == 0)
            {
                SetBkMode(ps.hdc, TRANSPARENT);
                DrawText(ps.hdc, L"zaloguj", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            }
            else if (wcscmp(text, L"RIGHT") == 0)
            {
                SetBkMode(ps.hdc, TRANSPARENT);
                DrawText(ps.hdc, L"przypomnij has這", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            }

        }break;

        case 3:
        {
            if (wcscmp(text, L"CENTRAL") == 0)
            {
                SetBkMode(ps.hdc, TRANSPARENT);
                DrawText(ps.hdc, L"przypomnij has這", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            }
            else if (wcscmp(text, L"LEFT") == 0)
            {
                SetBkMode(ps.hdc, TRANSPARENT);
                DrawText(ps.hdc, L"zaloguj", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            }
            else if (wcscmp(text, L"RIGHT") == 0)
            {
                SetBkMode(ps.hdc, TRANSPARENT);
                DrawText(ps.hdc, L"utw鏎z konto", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            }
        }break;

        }

        EndPaint(m_hwnd, &ps);

    }break;

    case WM_LBUTTONDOWN: {
        auto* data = reinterpret_cast<MyButtonData*>(GetWindowLongPtr(m_hwnd, GWLP_USERDATA));
        data->down = true;
        SetCapture(m_hwnd);
        InvalidateRect(m_hwnd, nullptr, false);
    }break;

    case WM_LBUTTONUP: {
        auto* data = reinterpret_cast<MyButtonData*>(GetWindowLongPtr(m_hwnd, GWLP_USERDATA));
        data->down = false;
        ReleaseCapture();
        InvalidateRect(m_hwnd, nullptr, false);

        auto x = GET_X_LPARAM(lParam);
        auto y = GET_Y_LPARAM(lParam);

        RECT rc;
        GetClientRect(m_hwnd, &rc);

        if (isInsideRect(rc, x, y))
        {
            SendMessage(m_hwnd, WM_COMMAND, (WPARAM)GetMenu(m_hwnd), 0);
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
                ::MessageBox(m_hwnd, L"LOGUJ", L"", MB_OK);
                flag_loging_window = 1;
                InvalidateRect(GetParent(m_hwnd), NULL, FALSE);
            }break;

            case 2:
            {
                ::MessageBox(m_hwnd, L"UTW紑Z KONTO", L"", MB_OK);
                flag_loging_window = 2;
                InvalidateRect(GetParent(m_hwnd), NULL, FALSE);
            }break;

            case 3:
            {
                ::MessageBox(m_hwnd, L"PRZYPOMNIJ HASΜ", L"", MB_OK);
                flag_loging_window = 3;
                InvalidateRect(GetParent(m_hwnd), NULL, FALSE);
            }break;
            }// switch (wParam)
        }break;

        case 2:
        {
            switch (wParam)
            {
            case 1:
            {
                ::MessageBox(m_hwnd, L"UTW紑Z KONTO", L"", MB_OK);
                flag_loging_window = 2;
                InvalidateRect(GetParent(m_hwnd), NULL, FALSE);
            }break;

            case 2:
            {
                ::MessageBox(m_hwnd, L"LOGUJ", L"", MB_OK);
                flag_loging_window = 1;
                InvalidateRect(GetParent(m_hwnd), NULL, FALSE);
            }break;

            case 3:
            {
                ::MessageBox(m_hwnd, L"PRZYPOMNIJ HASΜ", L"", MB_OK);
                flag_loging_window = 3;
                InvalidateRect(GetParent(m_hwnd), NULL, FALSE);
            }break;
            }// switch (wParam)

        }break;

        case 3:
        {
            switch (wParam)
            {
            case 1:
            {
                ::MessageBox(m_hwnd, L"PRZYPOMNIJ HASΜ", L"", MB_OK);
                flag_loging_window = 3;
                InvalidateRect(GetParent(m_hwnd), NULL, FALSE);
            }break;

            case 2:
            {
                ::MessageBox(m_hwnd, L"LOGUJ", L"", MB_OK);
                flag_loging_window = 1;
                InvalidateRect(GetParent(m_hwnd), NULL, FALSE);
            }break;

            case 3:
            {
                ::MessageBox(m_hwnd, L"UTW紑Z KONTO", L"", MB_OK);
                flag_loging_window = 2;
                InvalidateRect(GetParent(m_hwnd), NULL, FALSE);
            }break;
            }// switch (wParam)
        }break;
        }  //flag
    }break;//COMMAND


    } // switch
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}


LRESULT ZapasoweClass::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:

        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(m_hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(m_hwnd, &ps);
    }
    return 0;

    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}

int main() {
    return WinMain(GetModuleHandle(nullptr), nullptr, GetCommandLineA(), SW_NORMAL);
}
*/