
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

HWND hwnd_LogWin, hwnd_ServerLogWin, hwnd_ServerLogWin_EditLogin, hwnd_ServerLogWin_EditPassword, hwnd_LogWin_EditLogin, hwnd_LogWin_EditPassword, hwnd_LogWin_CheckBoxLoginData, hwnd_LogWin_EditOrganization;
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
int edit_size_x = 225;
int edit_size_y = 25;


struct LogWinButtonData {
    bool down = false;
    int yourData = 0;
};

bool isInsideRect(RECT& rc, LONG x, LONG y) {
    return (x >= rc.left && x <= rc.right && y >= rc.top && y <= rc.bottom);
}

void font_and_text_set(int x, int y, PAINTSTRUCT &ps, LPCWSTR text, RECT rc)
{
    hFont = CreateFont(x, 0, 0, 0, y, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, TEXT("Calibri"));
    SelectObject(ps.hdc, hFont);
    DrawText(ps.hdc, text, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}


LRESULT CALLBACK ServerLogWin(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ServerLogWinButton(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


LRESULT CALLBACK LogWin(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LogWinButton(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);



void initServerLogWinClass(HINSTANCE hInstance) {
    WNDCLASSEX wcex;
    my_bzero(wcex);
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = ServerLogWin;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"ServerLogWinClass";
    wcex.hIconSm = nullptr;

    RegisterClassEx(&wcex);
}

void initServerLogWinButtonClass(HINSTANCE hInstance) {
    WNDCLASSEX wcex;
    my_bzero(wcex);
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = ServerLogWinButton;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"ServerLogWinButtonClass";
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

void initLogWinButtonClass(HINSTANCE hInstance) {
    WNDCLASSEX wcex;
    my_bzero(wcex);
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = LogWinButton;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"LogWinButtonClass";
    wcex.hIconSm = nullptr;

    RegisterClassEx(&wcex);
}




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    
    //----ServerLogWin----------
    {
    initServerLogWinClass(hInstance);
    hwnd_ServerLogWin = CreateWindowEx(0, L"ServerLogWinClass", L"WiP", WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, scrcentralx, scrcentraly, windx, windy, nullptr, nullptr, hInstance, 0);

    hwnd_ServerLogWin_EditLogin = CreateWindowEx(0, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | DT_CENTER, 100, 50, button_size_x, button_size_y, hwnd_ServerLogWin, nullptr, hInstance, nullptr);
    hFont = CreateFont(20, 0, 0, 0, 300, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, TEXT("Calibri"));
    SendMessage(hwnd_ServerLogWin_EditLogin, WM_SETFONT, WPARAM(hFont), TRUE);
    hwnd_ServerLogWin_EditPassword = CreateWindowEx(0, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 100, 112, button_size_x, button_size_y, hwnd_ServerLogWin, nullptr, hInstance, nullptr);
    

    //--- LogWinButton ------
    initServerLogWinButtonClass(hInstance);
    CreateWindowEx(0, L"ServerLogWinButtonClass", L"B_LOG", WS_CHILD | WS_VISIBLE, 325, 105, button_size_x, button_size_y, hwnd_ServerLogWin, (HMENU)1, hInstance, nullptr);
    CreateWindowEx(0, L"ServerLogWinButtonClass", L"B_OFFLINE", WS_CHILD | WS_VISIBLE, 325, 283, button_size_x, button_size_y, hwnd_ServerLogWin, (HMENU)2, hInstance, nullptr);
    

    //-----------------------
    ShowWindow(hwnd_ServerLogWin, SW_NORMAL);
    UpdateWindow(hwnd_ServerLogWin);
    }

    //----LogWin----------
    {
    initLogWinClass(hInstance);
    hwnd_LogWin = CreateWindowEx(0,L"LogWinClass",L"WiP",WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,scrcentralx,scrcentraly,windx,windy,nullptr,nullptr,hInstance,0);

    const int EditLogin_maximum_length = 320;
    const int EditPassword_maximum_length = 20;
    const int EditOrganization_maximum_length = 30;

    hwnd_LogWin_EditLogin = CreateWindowEx(0, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | DT_CENTER | WS_TABSTOP |ES_AUTOHSCROLL, ((windx - edit_size_x) / 2), 75, edit_size_x, edit_size_y, hwnd_LogWin, nullptr, hInstance, nullptr);
    hFont = CreateFont(20, 0, 0, 0, 300, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, TEXT("Calibri"));
    SendMessage(hwnd_LogWin_EditLogin, WM_SETFONT, WPARAM(hFont), TRUE);
    SendMessage(hwnd_LogWin_EditLogin, EM_SETLIMITTEXT, EditLogin_maximum_length, 0);


    hwnd_LogWin_EditPassword = CreateWindowEx(0, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | DT_CENTER | WS_TABSTOP | ES_AUTOHSCROLL, ((windx - edit_size_x) / 2), 125, edit_size_x, edit_size_y, hwnd_LogWin, nullptr, hInstance, nullptr);
    SendMessage(hwnd_LogWin_EditPassword, WM_SETFONT, WPARAM(hFont), TRUE);
    SendMessage(hwnd_LogWin_EditPassword, EM_SETLIMITTEXT, EditPassword_maximum_length, 0);

    
    hwnd_LogWin_EditOrganization = CreateWindowEx(0, L"Edit", L"", WS_CHILD | WS_BORDER | DT_CENTER | WS_TABSTOP | ES_AUTOHSCROLL, ((windx - edit_size_x) / 2), 175, edit_size_x, edit_size_y, hwnd_LogWin, nullptr, hInstance, nullptr);
    SendMessage(hwnd_LogWin_EditOrganization, WM_SETFONT, WPARAM(hFont), TRUE);
    SendMessage(hwnd_LogWin_EditOrganization, EM_SETLIMITTEXT, EditOrganization_maximum_length, 0);



    hwnd_LogWin_CheckBoxLoginData = CreateWindowEx(0, L"Button", L"", WS_CHILD | WS_VISIBLE | BS_CHECKBOX | WS_TABSTOP, 182, 163, 25, 25, hwnd_LogWin, (HMENU)4, hInstance, nullptr);
    CheckDlgButton(hwnd_LogWin_CheckBoxLoginData, 4, BST_CHECKED);
    
    //--- LogWinButton ------
    initLogWinButtonClass(hInstance);
    CreateWindowEx(0, L"LogWinButtonClass", L"B_CENTRAL", WS_CHILD | WS_VISIBLE | WS_TABSTOP, ((windx - button_size_x) / 2), (windy -6*window_grid), button_size_x, button_size_y, hwnd_LogWin, (HMENU)1, hInstance, nullptr);
    CreateWindowEx(0, L"LogWinButtonClass", L"B_LEFT", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 15, 340, 100, 15, hwnd_LogWin, (HMENU)2, hInstance, nullptr);
    CreateWindowEx(0, L"LogWinButtonClass", L"B_RIGHT", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 130, 340, 100, 15, hwnd_LogWin, (HMENU)3, hInstance, nullptr);

    //CreateWindowEx(0, L"Button", L"Test", WS_CHILD | WS_VISIBLE, 10, 100, 200, 40, hwnd, (HMENU)10, hInstance, nullptr);

    //-----------------------
    ShowWindow(hwnd_LogWin, SW_NORMAL);
    UpdateWindow(hwnd_LogWin);

    }

    // Main message loop:  
    MSG msg;
    while (GetMessage(&msg, 0, 0, 0) > 0)
    {
        if (!IsDialogMessage(hwnd_LogWin, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

int flag_LogWin = 2; // 1-servlogin, 2-acclogin, 3-registration, 4-password_remind

LRESULT CALLBACK ServerLogWin(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        BeginPaint(hWnd, &ps);

        HBRUSH hBrush;
        hBrush = ::CreateSolidBrush(RGB(0, 72, 0));

        RECT rc;
        SetRect(&rc, 100, 25, 100 + button_size_x, 25 + button_size_y);
        //::FillRect(ps.hdc, &rc, hBrush);

        hFont = CreateFont(20, 0, 0, 0, 300, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, TEXT("Calibri"));
        SelectObject(ps.hdc, hFont);
        SetTextColor(ps.hdc, RGB(0, 72, 0));
        SetBkMode(ps.hdc, TRANSPARENT);
        DrawText(ps.hdc, L"Podaj adres serwera:", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

        SetRect(&rc, 100, 87, 100 + button_size_x, 87 + button_size_y);
        DrawText(ps.hdc, L"Podaj has³o do serwera:", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

        SetRect(&rc, 100, 149, 110 + button_size_x, 149 + button_size_y);
        DrawText(ps.hdc, L"Zapamiêtaj dane logowania", -1, &rc, DT_SINGLELINE | DT_VCENTER);

        SetRect(&rc, 100, 186, 110 + button_size_x, 186 + button_size_y);
        DrawText(ps.hdc, L"Autologowanie", -1, &rc, DT_SINGLELINE | DT_VCENTER);

        SetRect(&rc, 50, 245, 500, 246);
        ::FillRect(ps.hdc, &rc, hBrush);

        SetRect(&rc, 100, 270, 100 + button_size_x, 270 + 2*button_size_y);
        DrawText(ps.hdc, L"Zawsze uruchamiaj \n w trybie offline", -1, &rc, DT_WORDBREAK | DT_VCENTER);


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

LRESULT CALLBACK ServerLogWinButton(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

        if (wcscmp(text, L"B_LOG") == 0)
        {
            DrawText(ps.hdc, L"zaloguj", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }
        else if (wcscmp(text, L"B_OFFLINE") == 0)
        {
            DrawText(ps.hdc, L"tryb offline", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
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
        switch (wParam)
        {
            case 1:
            {
                ::MessageBox(hwnd_ServerLogWin, L"LOGUJ", L"", MB_OK);
                flag_LogWin = 2;
                InvalidateRect(GetParent(hWnd), NULL, FALSE);
            }break;
            case 2:
            {
                ::MessageBox(hwnd_ServerLogWin, L"OFFLINE", L"", MB_OK);
                flag_LogWin = 2;
                InvalidateRect(GetParent(hWnd), NULL, FALSE);


            }break;
        }// switch (wParam)

        
    }break;//WM_COMMAND


    } // switch    
    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK LogWin(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{    
    switch (message)
    {
        case WM_PAINT: {

            switch (flag_LogWin)
            {
            case 2:
            {
                PAINTSTRUCT ps;
                BeginPaint(hWnd, &ps);

                HBRUSH hBrush;
                hBrush = ::CreateSolidBrush(RGB(0, 72, 0));

                RECT rc;
                SetRect(&rc, ((windx - button_size_x) / 2), 50, ((windx - button_size_x) / 2) + button_size_x, 75);
                //::FillRect(ps.hdc, &rc, hBrush);
                hFont = CreateFont(20, 0, 0, 0, 300, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, TEXT("Calibri"));
                SelectObject(ps.hdc, hFont);
                SetTextColor(ps.hdc, RGB(0, 72, 0));
                SetBkMode(ps.hdc, TRANSPARENT);
                DrawText(ps.hdc, L"Podaj login:", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

                SetRect(&rc, ((windx - button_size_x) / 2), 100, ((windx - button_size_x) / 2) + button_size_x, 125);
                DrawText(ps.hdc, L"Podaj has³o:", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

                SetRect(&rc, 208, 163, 393, 188);
                DrawText(ps.hdc, L"Zapamiêtaj dane logowania", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                EndPaint(hWnd, &ps);
            }break;

            case 3:
            {
                PAINTSTRUCT ps;
                BeginPaint(hWnd, &ps);

                HBRUSH hBrush;
                hBrush = ::CreateSolidBrush(RGB(0, 72, 0));

                RECT rc;
                SetRect(&rc, ((windx - 450) / 2), 50, ((windx - 450) / 2) + 450, 75);
                //::FillRect(ps.hdc, &rc, hBrush);
                hFont = CreateFont(20, 0, 0, 0, 300, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, TEXT("Calibri"));
                SelectObject(ps.hdc, hFont);
                SetTextColor(ps.hdc, RGB(0, 72, 0));
                SetBkMode(ps.hdc, TRANSPARENT);
                DrawText(ps.hdc, L"Podaj adres e-mail (adres e-mail jest jednoczeœnie loginem):", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

                SetRect(&rc, ((windx - button_size_x) / 2), 100, ((windx - button_size_x) / 2) + button_size_x, 125);
                DrawText(ps.hdc, L"Utwórz has³o logowania:", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

                SetRect(&rc, ((windx - button_size_x) / 2), 150, ((windx - button_size_x) / 2) + button_size_x, 175);
                DrawText(ps.hdc, L"Podaj nazwê organizacji:", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                EndPaint(hWnd, &ps);
            }break;

            case 4:
            {
                PAINTSTRUCT ps;
                BeginPaint(hWnd, &ps);

                HBRUSH hBrush;
                hBrush = ::CreateSolidBrush(RGB(0, 72, 0));

                RECT rc;
                SetRect(&rc, ((windx - 450) / 2), 50, ((windx - 450) / 2) + 450, 75);
                //::FillRect(ps.hdc, &rc, hBrush);
                hFont = CreateFont(20, 0, 0, 0, 300, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, TEXT("Calibri"));
                SelectObject(ps.hdc, hFont);
                SetTextColor(ps.hdc, RGB(0, 72, 0));
                SetBkMode(ps.hdc, TRANSPARENT);
                DrawText(ps.hdc, L"Podaj adres e-mail:", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                EndPaint(hWnd, &ps);
            }break;
        }


    }break;//WM_PAINT


    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        { 
        case 4:
            int checked = IsDlgButtonChecked(hWnd, 4);
            if (checked) {
                CheckDlgButton(hWnd, 4, BST_UNCHECKED);
            }
            else {
                CheckDlgButton(hWnd, 4, BST_CHECKED);
            }
        }
        break;//WM_COMMAND
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK LogWinButton(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int normal_font = 20;
    int small_font = 16;

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

        wchar_t text[512];
        GetWindowText(hWnd, text, 512);
        SetBkMode(ps.hdc, TRANSPARENT);

        if (data->down)
        {
            SetTextColor(ps.hdc, RGB(240, 240, 240));
            hBrush = ::CreateSolidBrush(RGB(0, 72, 0));
            ::FillRect(ps.hdc, &rc, hBrush);
           
            switch (flag_LogWin)
            {
            case 2:
            {
                if (wcscmp(text, L"B_CENTRAL") == 0)
                {
                    font_and_text_set(normal_font, 700, ps, L"zaloguj", rc);
                }
                else if (wcscmp(text, L"B_LEFT") == 0)
                {
                    font_and_text_set(small_font, 700, ps, L"utwórz konto", rc);
                }
                else if (wcscmp(text, L"B_RIGHT") == 0)
                {
                    font_and_text_set(small_font, 700, ps, L"przypomnij has³o", rc);
                }
            }break;

            case 3:
            {
                if (wcscmp(text, L"B_CENTRAL") == 0)
                {
                    font_and_text_set(normal_font, 700, ps, L"utwórz konto", rc);
                }
                else if (wcscmp(text, L"B_LEFT") == 0)
                {
                    font_and_text_set(small_font, 700, ps, L"zaloguj", rc);
                }
                else if (wcscmp(text, L"B_RIGHT") == 0)
                {
                    font_and_text_set(small_font, 700, ps, L"przypomnij has³o", rc);
                }

            }break;

            case 4:
            {
                if (wcscmp(text, L"B_CENTRAL") == 0)
                {
                    font_and_text_set(normal_font, 700, ps, L"przypomnij has³o", rc);
                }
                else if (wcscmp(text, L"B_LEFT") == 0)
                {
                    font_and_text_set(small_font, 700, ps, L"zaloguj", rc);
                }
                else if (wcscmp(text, L"B_RIGHT") == 0)
                {
                    font_and_text_set(small_font, 700, ps, L"utwórz konto", rc);
                }
            }break;

            }

        }
        else
        {
            SetTextColor(ps.hdc, RGB(0, 72, 0));
            SelectObject(ps.hdc, GetStockObject(WHITE_BRUSH));

            switch (flag_LogWin)
            {
            case 2:
            {
                if (wcscmp(text, L"B_CENTRAL") == 0)
                {
                    font_and_text_set(normal_font, 300, ps, L"zaloguj", rc);
                }
                else if (wcscmp(text, L"B_LEFT") == 0)
                {
                    font_and_text_set(small_font, 300, ps, L"utwórz konto", rc);
                }
                else if (wcscmp(text, L"B_RIGHT") == 0)
                {
                    font_and_text_set(small_font, 300, ps, L"przypomnij has³o", rc);
                }
            }break;

            case 3:
            {
                if (wcscmp(text, L"B_CENTRAL") == 0)
                {
                    font_and_text_set(normal_font, 300, ps, L"utwórz konto", rc);
                }
                else if (wcscmp(text, L"B_LEFT") == 0)
                {
                    font_and_text_set(small_font, 300, ps, L"zaloguj", rc);
                }
                else if (wcscmp(text, L"B_RIGHT") == 0)
                {
                    font_and_text_set(small_font, 300, ps, L"przypomnij has³o", rc);
                }

            }break;

            case 4:
            {
                if (wcscmp(text, L"B_CENTRAL") == 0)
                {
                    font_and_text_set(normal_font, 300, ps, L"przypomnij has³o", rc);
                }
                else if (wcscmp(text, L"B_LEFT") == 0)
                {
                    font_and_text_set(small_font, 300, ps, L"zaloguj", rc);
                }
                else if (wcscmp(text, L"B_RIGHT") == 0)
                {
                    font_and_text_set(small_font, 300, ps, L"utwórz konto", rc);
                }
            }break;

            }
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
            case 2:
            {
                switch (wParam)
                {
                    case 1:
                    {
                        //::MessageBox(hwnd_LogWin, L"Tutaj bedzie proba logowania", L"", MB_OK);

                        wchar_t login_text[512];
                        GetWindowText(hwnd_LogWin_EditLogin, login_text, 512);

                        std::wstring ws(login_text);
                        // your new String
                        std::string str(ws.begin(), ws.end());
                        // Show String
                        std::cout << "mail: " << str << std::endl;

                        ::MessageBox(hwnd_LogWin, login_text, L"", MB_OK);

                    }break;

                    case 2:
                    {
                        InvalidateRect(GetParent(hWnd), NULL, FALSE);

                        //::MessageBox(hwnd_LogWin, L"UTWÓRZ KONTO", L"", MB_OK);
                        flag_LogWin = 3;
                        ShowWindow(hwnd_LogWin_CheckBoxLoginData, SW_SHOW);
                        ShowWindow(hwnd_LogWin_CheckBoxLoginData, SW_HIDE);

                        ShowWindow(hwnd_LogWin_EditOrganization, SW_SHOW);

                        ShowWindow(hwnd_LogWin_EditPassword, SW_SHOW);

                    }break;

                    case 3:
                    {
                        InvalidateRect(GetParent(hWnd), NULL, FALSE);

                        //::MessageBox(hwnd_LogWin, L"PRZYPOMNIJ HAS£O", L"", MB_OK);
                        flag_LogWin = 4;
                        ShowWindow(hwnd_LogWin_CheckBoxLoginData, SW_SHOW);
                        ShowWindow(hwnd_LogWin_CheckBoxLoginData, SW_HIDE);

                        ShowWindow(hwnd_LogWin_EditOrganization, SW_HIDE);

                        ShowWindow(hwnd_LogWin_EditPassword, SW_HIDE);

                    }break;

                }// switch (wParam)
            }break;

            case 3:
            {
                switch (wParam)
                {
                    case 1:
                    {
                        ::MessageBox(hwnd_LogWin, L"Tutaj bêdzie próba utworzenia konta", L"", MB_OK);
                        
                    }break;

                    case 2:
                    {
                        InvalidateRect(GetParent(hWnd), NULL, FALSE);

                        //::MessageBox(hwnd_LogWin, L"LOGUJ", L"", MB_OK);
                        flag_LogWin = 2;
                        ShowWindow(hwnd_LogWin_CheckBoxLoginData, SW_HIDE);
                        ShowWindow(hwnd_LogWin_CheckBoxLoginData, SW_SHOW);

                        ShowWindow(hwnd_LogWin_EditOrganization, SW_HIDE);

                        ShowWindow(hwnd_LogWin_EditPassword, SW_SHOW);

                    }break;

                    case 3:
                    {
                        InvalidateRect(GetParent(hWnd), NULL, FALSE);

                        //::MessageBox(hwnd_LogWin, L"PRZYPOMNIJ HAS£O", L"", MB_OK);
                        flag_LogWin = 4;
                        ShowWindow(hwnd_LogWin_CheckBoxLoginData, SW_SHOW);
                        ShowWindow(hwnd_LogWin_CheckBoxLoginData, SW_HIDE);

                        ShowWindow(hwnd_LogWin_EditOrganization, SW_HIDE);

                        ShowWindow(hwnd_LogWin_EditPassword, SW_HIDE);

                    }break;
                }// switch (wParam)

            }break;

            case 4:
            {
                switch (wParam)
                {
                    case 1:
                    {
                        ::MessageBox(hwnd_LogWin, L"Tutaj bêdzie próba przypomnienia has³a", L"", MB_OK);
                        
                    }break;

                    case 2:
                    {
                        InvalidateRect(GetParent(hWnd), NULL, FALSE);

                        //::MessageBox(hwnd_LogWin, L"LOGUJ", L"", MB_OK);
                        flag_LogWin = 2;
                        ShowWindow(hwnd_LogWin_CheckBoxLoginData, SW_HIDE);
                        ShowWindow(hwnd_LogWin_CheckBoxLoginData, SW_SHOW);

                        ShowWindow(hwnd_LogWin_EditOrganization, SW_HIDE);

                        ShowWindow(hwnd_LogWin_EditPassword, SW_SHOW);

                    }break;

                    case 3:
                    {
                        InvalidateRect(GetParent(hWnd), NULL, FALSE);

                        //::MessageBox(hwnd_LogWin, L"UTWÓRZ KONTO", L"", MB_OK);
                        flag_LogWin = 3;
                        ShowWindow(hwnd_LogWin_CheckBoxLoginData, SW_SHOW);
                        ShowWindow(hwnd_LogWin_CheckBoxLoginData, SW_HIDE);

                        ShowWindow(hwnd_LogWin_EditOrganization, SW_SHOW);

                        ShowWindow(hwnd_LogWin_EditPassword, SW_SHOW);

                    }break;
                }// switch (wParam)
            }break;

        }  //flag
        InvalidateRect(GetParent(hWnd), NULL, FALSE);

    }break;//WM_COMMAND
    

    } // switch    
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int main() {
    return WinMain(GetModuleHandle(nullptr), nullptr, GetCommandLineA(), SW_NORMAL);
}
