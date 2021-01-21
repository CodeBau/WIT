#include <tchar.h>
#include <Windows.h>
#include <windowsx.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cassert>
#include <locale>
#include <mysql.h>

#include "../resource.h"
#include "../include/myfunctions.h"
#include "../include/myvariable.h"
#include "../include/MyBitmap.h"
#include "../include/mysql.h"

//  Declare Windows procedure

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
int red_allert_size_x = 350;

LRESULT CALLBACK LogWin(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LogWinButton(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	//----LogWin----------
	{
		initLogWinClass(hInstance);
		hwnd_LogWin = CreateWindowEx(0, L"LogWinClass", L"WiP", WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, scrcentralx, scrcentraly, windx, windy, nullptr, nullptr, hInstance, 0);

		const int EditLogin_maximum_length = 320;
		const int EditPassword_maximum_length = 20;
		const int EditOrganization_maximum_length = 30;

		hwnd_LogWin_EditLogin = CreateWindowEx(0, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | DT_CENTER | WS_TABSTOP | ES_AUTOHSCROLL, ((windx - edit_size_x) / 2), 75, edit_size_x, edit_size_y, hwnd_LogWin, nullptr, hInstance, nullptr);
		HFONT hFont = CreateFont(20, 0, 0, 0, 300, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, TEXT("Calibri"));
		SendMessage(hwnd_LogWin_EditLogin, WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessage(hwnd_LogWin_EditLogin, EM_SETLIMITTEXT, EditLogin_maximum_length, 0);

		hwnd_LogWin_EditPassword = CreateWindowEx(0, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | DT_CENTER | WS_TABSTOP | ES_AUTOHSCROLL, ((windx - edit_size_x) / 2), 125, edit_size_x, edit_size_y, hwnd_LogWin, nullptr, hInstance, nullptr);
		SendMessage(hwnd_LogWin_EditPassword, WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessage(hwnd_LogWin_EditPassword, EM_SETLIMITTEXT, EditPassword_maximum_length, 0);

		hwnd_LogWin_EditOrganization = CreateWindowEx(0, L"Edit", L"", WS_CHILD | WS_BORDER | DT_CENTER | WS_TABSTOP | ES_AUTOHSCROLL, ((windx - edit_size_x) / 2), 175, edit_size_x, edit_size_y, hwnd_LogWin, nullptr, hInstance, nullptr);
		SendMessage(hwnd_LogWin_EditOrganization, WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessage(hwnd_LogWin_EditOrganization, EM_SETLIMITTEXT, EditOrganization_maximum_length, 0);

		hwnd_LogWin_CheckBoxLoginData = CreateWindowEx(0, L"Button", L"Zapamiêtaj dane logowania", WS_CHILD | WS_VISIBLE | BS_CHECKBOX | WS_TABSTOP, ((windx - 210) / 2), 175, 210, 25, hwnd_LogWin, (HMENU)4, hInstance, nullptr);
		CheckDlgButton(hwnd_LogWin_CheckBoxLoginData, 4, BST_CHECKED);
		SendMessage(hwnd_LogWin_CheckBoxLoginData, WM_SETFONT, WPARAM(hFont), TRUE);

		hwnd_LogWin_CheckBoxAutoLogin = CreateWindowEx(0, L"Button", L"Autologowanie", WS_CHILD | WS_VISIBLE | BS_CHECKBOX | WS_TABSTOP, ((windx - 210) / 2), 200, 210, 25, hwnd_LogWin, (HMENU)5, hInstance, nullptr);
		CheckDlgButton(hwnd_LogWin_CheckBoxAutoLogin, 5, BST_CHECKED);
		SendMessage(hwnd_LogWin_CheckBoxAutoLogin, WM_SETFONT, WPARAM(hFont), TRUE);

		//--- LogWinButton ------
		initLogWinButtonClass(hInstance);
		CreateWindowEx(0, L"LogWinButtonClass", L"B_CENTRAL", WS_CHILD | WS_VISIBLE | WS_TABSTOP, ((windx - button_size_x) / 2), (windy - 6 * window_grid), button_size_x, button_size_y, hwnd_LogWin, (HMENU)1, hInstance, nullptr);
		CreateWindowEx(0, L"LogWinButtonClass", L"B_LEFT", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 15, 340, 100, 15, hwnd_LogWin, (HMENU)2, hInstance, nullptr);
		CreateWindowEx(0, L"LogWinButtonClass", L"B_RIGHT", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 130, 340, 100, 15, hwnd_LogWin, (HMENU)3, hInstance, nullptr);

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

LRESULT CALLBACK LogWin(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE: {
		auto* data = new GlobalData();
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<intptr_t>(data));
	}break;

	case WM_TIMER:
	{
		switch (wParam)
		{
		case 1001:
		{
			KillTimer(hWnd, 1001);
			auto* data = reinterpret_cast<GlobalData*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			data->red_allert = L"";

			InvalidateRect(hwnd_LogWin, NULL, true);
			UpdateWindow(hwnd_LogWin);
		};
		}
	}break;//WM_TIMER

	case WM_CTLCOLORSTATIC:
	{
		HDC hDC = (HDC)wParam;
		HWND hWndControl = (HWND)lParam;
		if (hwnd_LogWin_CheckBoxLoginData == hWndControl)
		{
			SetTextColor(hDC, RGB(0, 72, 0));
			SetBkMode(hDC, TRANSPARENT);
			return (LRESULT)GetStockObject(NULL_BRUSH);
		}
		else if (hwnd_LogWin_CheckBoxAutoLogin == hWndControl)
		{
			SetTextColor(hDC, RGB(0, 72, 0));
			SetBkMode(hDC, TRANSPARENT);
			return (LRESULT)GetStockObject(NULL_BRUSH);
		}
	}break;//WM_CTLCOLORSTATIC

	case WM_PAINT:
	{
		auto* data = reinterpret_cast<GlobalData*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		switch (data->flag_LogWin)
		{
		case 'a': //login
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);

			HBRUSH hBrush = ::CreateSolidBrush(RGB(0, 72, 0));

			RECT rc;

			SetRect(&rc, ((windx - button_size_x) / 2), 50, ((windx - button_size_x) / 2) + button_size_x, 75);
			//::FillRect(ps.hdc, &rc, hBrush);
			HFONT hFont = CreateFont(20, 0, 0, 0, 300, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, TEXT("Calibri"));
			SelectObject(ps.hdc, hFont);
			SetTextColor(ps.hdc, RGB(0, 72, 0));
			SetBkMode(ps.hdc, TRANSPARENT);
			DrawText(ps.hdc, L"Podaj login:", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			SetRect(&rc, ((windx - button_size_x) / 2), 100, ((windx - button_size_x) / 2) + button_size_x, 125);
			DrawText(ps.hdc, L"Podaj has³o:", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			
			if (data->red_allert != L"")
			{
				SetTextColor(ps.hdc, RGB(255, 43, 0));
				SetRect(&rc, ((windx - red_allert_size_x) / 2), 150, ((windx - red_allert_size_x) / 2) + red_allert_size_x, 175);
				const wchar_t* szRed_allert_text = data->red_allert.c_str();
				DrawText(ps.hdc, szRed_allert_text, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			}
			EndPaint(hWnd, &ps);
		}break;

		case 'b': // registration
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);

			HBRUSH hBrush = ::CreateSolidBrush(RGB(0, 72, 0));

			RECT rc;
			SetRect(&rc, ((windx - 450) / 2), 50, ((windx - 450) / 2) + 450, 75);
			//::FillRect(ps.hdc, &rc, hBrush);
			HFONT hFont = CreateFont(20, 0, 0, 0, 300, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, TEXT("Calibri"));
			SelectObject(ps.hdc, hFont);
			SetTextColor(ps.hdc, RGB(0, 72, 0));
			SetBkMode(ps.hdc, TRANSPARENT);
			DrawText(ps.hdc, L"Podaj adres e-mail (adres e-mail jest jednoczeœnie loginem):", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			SetRect(&rc, ((windx - button_size_x) / 2), 100, ((windx - button_size_x) / 2) + button_size_x, 125);
			DrawText(ps.hdc, L"Utwórz has³o logowania:", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			SetRect(&rc, ((windx - button_size_x) / 2), 150, ((windx - button_size_x) / 2) + button_size_x, 175);
			DrawText(ps.hdc, L"Podaj nazwê organizacji:", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			if (data->red_allert != L"")
			{
				SetTextColor(ps.hdc, RGB(255, 43, 0));
				SetRect(&rc, ((windx - red_allert_size_x) / 2), 200, ((windx - red_allert_size_x) / 2) + red_allert_size_x, 225);
				const wchar_t* szRed_allert_text = data->red_allert.c_str();
				DrawText(ps.hdc, szRed_allert_text, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			}
			EndPaint(hWnd, &ps);
		}break;

		case 'c': //c-password_reminder
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);

			HBRUSH hBrush = ::CreateSolidBrush(RGB(0, 72, 0));

			RECT rc;
			SetRect(&rc, ((windx - 450) / 2), 50, ((windx - 450) / 2) + 450, 75);
			//::FillRect(ps.hdc, &rc, hBrush);
			HFONT hFont = CreateFont(20, 0, 0, 0, 300, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, TEXT("Calibri"));
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
		{
			int checked = IsDlgButtonChecked(hWnd, 4);
			if (checked) {
				CheckDlgButton(hWnd, 4, BST_UNCHECKED);
				InvalidateRect(hWnd, NULL, true);
				UpdateWindow(hWnd);
				std::cout << "costam\n";
			}
			else {
				CheckDlgButton(hWnd, 4, BST_CHECKED);
				InvalidateRect(hWnd, NULL, true);
				UpdateWindow(hWnd);
				std::cout << "costam\n";
			}
		}break;
		case 5:
		{
			int checked = IsDlgButtonChecked(hWnd, 5);
			if (checked) {
				CheckDlgButton(hWnd, 4, BST_UNCHECKED);
				CheckDlgButton(hWnd, 5, BST_UNCHECKED);
				EnableWindow(hwnd_LogWin_CheckBoxLoginData, TRUE);
				InvalidateRect(hWnd, NULL, true);
				UpdateWindow(hWnd);
			}
			else {
				CheckDlgButton(hWnd, 4, BST_CHECKED);
				CheckDlgButton(hWnd, 5, BST_CHECKED);
				EnableWindow(hwnd_LogWin_CheckBoxLoginData, FALSE);
				InvalidateRect(hWnd, NULL, true);
				UpdateWindow(hWnd);
			}
		}break;
		}break;//(LOWORD(wParam))
	}break;//WM_COMMAND

	case WM_DESTROY:
	{
		auto* data = reinterpret_cast<GlobalData*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		delete data;
		PostQuitMessage(0);
	}break;//WM_DESTROY:

	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}break;//default:
	}//(message)

	return 0;
}//CALLBACK LogWin

LRESULT CALLBACK LogWinButton(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int normal_font = 20;
	int small_font = 16;

	switch (message)
	{
	case WM_CREATE: {
		auto* data = new GlobalData();
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<intptr_t>(data));
	}break;

	case WM_DESTROY: {
		auto* data = reinterpret_cast<GlobalData*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		delete data;
	}break;

	case WM_PAINT: {
		auto* data = reinterpret_cast<GlobalData*>(GetWindowLongPtr(GetParent(hWnd), GWLP_USERDATA));
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);

		RECT rc;
		GetClientRect(hWnd, &rc);
		Rectangle(ps.hdc, rc.left, rc.top, rc.right, rc.bottom);

		wchar_t text[512];
		GetWindowText(hWnd, text, 512);
		SetBkMode(ps.hdc, TRANSPARENT);

		if (data->down)
		{
			SetTextColor(ps.hdc, RGB(240, 240, 240));
			HBRUSH hBrush = ::CreateSolidBrush(RGB(0, 72, 0));
			::FillRect(ps.hdc, &rc, hBrush);

			switch (data->flag_LogWin)
			{
			case 'a':
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

			case 'b':
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

			case 'c':
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

			switch (data->flag_LogWin)
			{
			case 'a':
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

			case 'b':
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

			case 'c':
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
		auto* data = reinterpret_cast<GlobalData*>(GetWindowLongPtr(GetParent(hWnd), GWLP_USERDATA));
		data->down = true;
		SetCapture(hWnd);
		InvalidateRect(hWnd, nullptr, false);
	}break;//WM_LBUTTONDOWN

	case WM_LBUTTONUP: {
		auto* data = reinterpret_cast<GlobalData*>(GetWindowLongPtr(GetParent(hWnd), GWLP_USERDATA));
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
		auto* data = reinterpret_cast<GlobalData*>(GetWindowLongPtr(GetParent(hWnd), GWLP_USERDATA));

		switch (data->flag_LogWin)
		{
		case 'a': //login
		{
			switch (wParam)
			{
			case 1:
			{
				//::MessageBox(hwnd_LogWin, L"Tutaj bedzie proba logowania", L"", MB_OK);

				wchar_t login_text[512];
				GetWindowText(hwnd_LogWin_EditLogin, login_text, 512);
				wchar_t password_text[512];
				GetWindowText(hwnd_LogWin_EditPassword, password_text, 512);

				if (f_wchar_t_lenght(login_text) != 0 && f_wchar_t_lenght(password_text) != 0)
				{
					if (f_at_in_login(login_text) == -1)
					{
						SetTimer(hwnd_LogWin, 1001, 2500, nullptr);
						data->red_allert = L"login jest loginem";

					}
					else if (f_at_in_login(login_text) != -1)
					{
						SetTimer(hwnd_LogWin, 1001, 2500, nullptr);
						data->red_allert = L"login jest mailem";
					}
				}
				else if (f_wchar_t_lenght(login_text) == 0 && f_wchar_t_lenght(password_text) == 0)
				{
					SetTimer(hwnd_LogWin, 1001, 2500, nullptr);
					data->red_allert = L"Uzupe³nij pola";
				}
				else if (f_wchar_t_lenght(login_text) == 0 && f_wchar_t_lenght(password_text) != 0)
				{
					SetTimer(hwnd_LogWin, 1001, 2500, nullptr);
					data->red_allert = L"Uzupe³nij login";

				}
				else if (f_wchar_t_lenght(login_text) != 0 && f_wchar_t_lenght(password_text) == 0)
				{
					SetTimer(hwnd_LogWin, 1001, 2500, nullptr);
					data->red_allert = L"Uzupe³nij has³o";
				}

				//::MessageBox(hwnd_LogWin, wide_string1, L"nowe", MB_OK);

				InvalidateRect(hwnd_LogWin, NULL, true);
				UpdateWindow(hwnd_LogWin);
			}break;

			case 2:
			{
				//::MessageBox(hwnd_LogWin, L"UTWÓRZ KONTO", L"", MB_OK);
				data->flag_LogWin = 'b';
				ShowWindow(hwnd_LogWin_CheckBoxLoginData, SW_HIDE);
				ShowWindow(hwnd_LogWin_CheckBoxAutoLogin, SW_HIDE);

				ShowWindow(hwnd_LogWin_EditOrganization, SW_SHOW);

				ShowWindow(hwnd_LogWin_EditPassword, SW_SHOW);

				data->red_allert = L"";

				InvalidateRect(hwnd_LogWin, NULL, true);
				UpdateWindow(hwnd_LogWin);
			}break;

			case 3:
			{
				//::MessageBox(hwnd_LogWin, L"PRZYPOMNIJ HAS£O", L"", MB_OK);
				data->flag_LogWin = 'c';
				ShowWindow(hwnd_LogWin_CheckBoxLoginData, SW_HIDE);
				ShowWindow(hwnd_LogWin_CheckBoxAutoLogin, SW_HIDE);

				ShowWindow(hwnd_LogWin_EditOrganization, SW_HIDE);

				ShowWindow(hwnd_LogWin_EditPassword, SW_HIDE);

				data->red_allert = L"";

				InvalidateRect(hwnd_LogWin, NULL, true);
				UpdateWindow(hwnd_LogWin);
			}break;
			}// switch (wParam)
		}break;

		case 'b': // registration
		{
			switch (wParam)
			{
			case 1:
			{
				//::MessageBox(hwnd_LogWin, L"Tutaj bêdzie próba utworzenia konta", L"", MB_OK);
				wchar_t login_text[512];
				GetWindowText(hwnd_LogWin_EditLogin, login_text, 512);
				wchar_t password_text[512];
				GetWindowText(hwnd_LogWin_EditPassword, password_text, 512);
				wchar_t organization_text[512];
				GetWindowText(hwnd_LogWin_EditOrganization, organization_text, 512);

				if (f_wchar_t_lenght(login_text) != 0 && f_wchar_t_lenght(password_text) != 0 && f_wchar_t_lenght(organization_text) != 0)
				{
					if (f_at_in_login(login_text) == -1) //login ist just login
					{
						SetTimer(hwnd_LogWin, 1001, 2500, nullptr);
						data->red_allert = L"SprawdŸ poprawnoœæ adresu e-mail";
					}
					
					else if (f_wchar_t_lenght(password_text) < 6)
					{
						SetTimer(hwnd_LogWin, 1001, 2500, nullptr);
						data->red_allert = L"Has³o musi mieæ minimum 6 znaków";
					}

					else if (f_wchar_t_lenght(organization_text) < 3)
					{
						SetTimer(hwnd_LogWin, 1001, 2500, nullptr);
						data->red_allert = L"Nazwa organizacji musi mieæ minimum 3 znaki";
					}
					else if (f_at_in_login(login_text) != -1 && f_wchar_t_lenght(password_text) >= 6 && f_wchar_t_lenght(organization_text) >= 3)
					{
						SetTimer(hwnd_LogWin, 1001, 2500, nullptr);
						data->red_allert = L"Wszystko siê zgadza mo¿na zak³adaæ organizacjê";
						f_sql_create_organizations_table();
						f_sql_check_organization(hwnd_LogWin_EditOrganization);
						//f_sql_insert_organization(hwnd_LogWin_EditOrganization);
					}
				}
				else if (f_wchar_t_lenght(login_text) == 0 && f_wchar_t_lenght(password_text) == 0 && f_wchar_t_lenght(organization_text) == 0)
				{
					SetTimer(hwnd_LogWin, 1001, 2500, nullptr);
					data->red_allert = L"Uzupe³nij pola";
				}
				else if ((f_wchar_t_lenght(login_text) != 0 && f_wchar_t_lenght(password_text) == 0 && f_wchar_t_lenght(organization_text) == 0) || (f_wchar_t_lenght(login_text) == 0 && f_wchar_t_lenght(password_text) != 0 && f_wchar_t_lenght(organization_text) == 0) || (f_wchar_t_lenght(login_text) == 0 && f_wchar_t_lenght(password_text) == 0 && f_wchar_t_lenght(organization_text) != 0))
				{
					SetTimer(hwnd_LogWin, 1001, 2500, nullptr);
					data->red_allert = L"Uzupe³nij brakuj¹ce pola";
				}
				else if (f_wchar_t_lenght(login_text) == 0 && f_wchar_t_lenght(password_text) != 0 && f_wchar_t_lenght(organization_text) != 0)
				{
					SetTimer(hwnd_LogWin, 1001, 2500, nullptr);
					data->red_allert = L"Uzupe³nij login";
				}
				else if (f_wchar_t_lenght(login_text) != 0 && f_wchar_t_lenght(password_text) == 0 && f_wchar_t_lenght(organization_text) != 0)
				{
					SetTimer(hwnd_LogWin, 1001, 2500, nullptr);
					data->red_allert = L"Uzupe³nij has³o";
				}
				else if (f_wchar_t_lenght(login_text) != 0 && f_wchar_t_lenght(password_text) != 0 && f_wchar_t_lenght(organization_text) == 0)
				{
					SetTimer(hwnd_LogWin, 1001, 2500, nullptr);
					data->red_allert = L"Uzupe³nij nazwê organizacji";
				}

				InvalidateRect(hwnd_LogWin, NULL, true);
				UpdateWindow(hwnd_LogWin);
			}break;

			case 2:
			{
				//::MessageBox(hwnd_LogWin, L"LOGUJ", L"", MB_OK);
				data->flag_LogWin = 'a';
				ShowWindow(hwnd_LogWin_CheckBoxLoginData, SW_SHOW);
				ShowWindow(hwnd_LogWin_CheckBoxAutoLogin, SW_SHOW);

				ShowWindow(hwnd_LogWin_EditOrganization, SW_HIDE);

				ShowWindow(hwnd_LogWin_EditPassword, SW_SHOW);

				data->red_allert = L"";

				InvalidateRect(hwnd_LogWin, NULL, true);
				UpdateWindow(hwnd_LogWin);
			}break;

			case 3:
			{
				//::MessageBox(hwnd_LogWin, L"PRZYPOMNIJ HAS£O", L"", MB_OK);
				data->flag_LogWin = 'c';
				ShowWindow(hwnd_LogWin_CheckBoxLoginData, SW_HIDE);
				ShowWindow(hwnd_LogWin_CheckBoxAutoLogin, SW_HIDE);

				ShowWindow(hwnd_LogWin_EditOrganization, SW_HIDE);

				ShowWindow(hwnd_LogWin_EditPassword, SW_HIDE);

				data->red_allert = L"";

				InvalidateRect(hwnd_LogWin, NULL, true);
				UpdateWindow(hwnd_LogWin);
			}break;
			}// switch (wParam)
		}break;

		case 'c': //
		{
			switch (wParam)
			{
			case 1:
			{
				::MessageBox(hwnd_LogWin, L"Tutaj bêdzie próba przypomnienia has³a", L"", MB_OK);
			}break;

			case 2:
			{
				//::MessageBox(hwnd_LogWin, L"LOGUJ", L"", MB_OK);
				data->flag_LogWin = 'a';
				ShowWindow(hwnd_LogWin_CheckBoxLoginData, SW_SHOW);
				ShowWindow(hwnd_LogWin_CheckBoxAutoLogin, SW_SHOW);

				ShowWindow(hwnd_LogWin_EditOrganization, SW_HIDE);

				ShowWindow(hwnd_LogWin_EditPassword, SW_SHOW);

				data->red_allert = L"";

				InvalidateRect(hwnd_LogWin, NULL, true);
				UpdateWindow(hwnd_LogWin);
			}break;

			case 3:
			{
				//::MessageBox(hwnd_LogWin, L"UTWÓRZ KONTO", L"", MB_OK);
				data->flag_LogWin = 'b';
				ShowWindow(hwnd_LogWin_CheckBoxLoginData, SW_HIDE);
				ShowWindow(hwnd_LogWin_CheckBoxAutoLogin, SW_HIDE);

				ShowWindow(hwnd_LogWin_EditOrganization, SW_SHOW);

				ShowWindow(hwnd_LogWin_EditPassword, SW_SHOW);

				data->red_allert = L"";

				InvalidateRect(hwnd_LogWin, NULL, true);
				UpdateWindow(hwnd_LogWin);
			}break;
			}// switch (wParam)
		}break;//case 4:
		}//switch (data->struct_flag_LogWin)
	}break;//WM_COMMAND
	}//switch (message)

	return DefWindowProc(hWnd, message, wParam, lParam);
}//CALLBACK LogWinButton

int main() {
	return WinMain(GetModuleHandle(nullptr), nullptr, GetCommandLineA(), SW_NORMAL);
}