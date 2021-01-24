#include "../include/LogWindow.h"
#include "../include/RegWindow.h"
#include "../include/myfunctions.h"

const wchar_t* LogWindow::s_className = L"LogWindow";

void LogWindow::create()
{
	static bool registered = false;
	if (!registered) {
		registered = true;

		WNDCLASSEX wcex = {};
	//	my_bzero(wcex); // wcex = {}; already set zero
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = &s_WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance =  GetModuleHandle(nullptr);
		wcex.hIcon = nullptr;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = s_className;
		wcex.hIconSm = nullptr;

		RegisterClassEx(&wcex);
	}

	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);
	int x = (cx - windx) / 2;
	int y = (cy - windy) / 2;

	baseCreate(0, s_className, L"WiP-logowanie", //nazwa okna
		WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,
		x, y, windx, windy, 
		nullptr, nullptr);

	HWND hwnd_LogWin = hwnd();

//----- Child Controls -----
	int edit_size_x = 225;
	int edit_size_y = 25;
	int window_grid = 25;

	const int EditLogin_maximum_length = 320;
	const int EditPassword_maximum_length = 20;
	const int EditOrganization_maximum_length = 30;

	int normal_font = 20;
	int small_font = 16;

	HINSTANCE hInstance = GetModuleHandle(nullptr);

	hwnd_LogWin_EditLogin = CreateWindowEx(0, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | DT_CENTER | WS_TABSTOP | ES_AUTOHSCROLL, ((windx - edit_size_x) / 2), 75, edit_size_x, edit_size_y, hwnd(), nullptr, hInstance, nullptr);
	
	m_font = CreateFont(20, 0, 0, 0, 300, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, TEXT("Calibri"));
	m_brush = ::CreateSolidBrush(RGB(0, 72, 0));

	SendMessage(hwnd_LogWin_EditLogin, WM_SETFONT, WPARAM(m_font), TRUE);
	SendMessage(hwnd_LogWin_EditLogin, EM_SETLIMITTEXT, EditLogin_maximum_length, 0);

	hwnd_LogWin_EditPassword = CreateWindowEx(0, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | DT_CENTER | WS_TABSTOP | ES_AUTOHSCROLL, ((windx - edit_size_x) / 2), 125, edit_size_x, edit_size_y, hwnd(), nullptr, hInstance, nullptr);
	SendMessage(hwnd_LogWin_EditPassword, WM_SETFONT, WPARAM(m_font), TRUE);
	SendMessage(hwnd_LogWin_EditPassword, EM_SETLIMITTEXT, EditPassword_maximum_length, 0);

	hwnd_LogWin_CheckBoxLoginData = CreateWindowEx(0, L"Button", L"Zapamiêtaj dane logowania", WS_CHILD | WS_VISIBLE | BS_CHECKBOX | WS_TABSTOP, ((windx - 210) / 2), 175, 210, 25, hwnd(), (HMENU)ChildId::LoginCheckBox, hInstance, nullptr);
	CheckDlgButton(hwnd_LogWin_CheckBoxLoginData, 4, BST_CHECKED);
	SendMessage(hwnd_LogWin_CheckBoxLoginData, WM_SETFONT, WPARAM(m_font), TRUE);

	hwnd_LogWin_CheckBoxAutoLogin = CreateWindowEx(0, L"Button", L"Autologowanie", WS_CHILD | WS_VISIBLE | BS_CHECKBOX | WS_TABSTOP, ((windx - 210) / 2), 200, 210, 25, hwnd(), (HMENU)ChildId::AutoLoginCheckBox, hInstance, nullptr);
	CheckDlgButton(hwnd_LogWin_CheckBoxAutoLogin, 5, BST_CHECKED);
	SendMessage(hwnd_LogWin_CheckBoxAutoLogin, WM_SETFONT, WPARAM(m_font), TRUE);

// ------ Buttons --------

	m_centralButton.create((windx - button_size_x) / 2, (windy - 6 * window_grid), button_size_x, button_size_y,L"zaloguj", hwnd(), (HMENU)ChildId::CenterButton, normal_font);
	m_leftButton.create(15,  340, 100, 15, L"utwórz konto", hwnd(), (HMENU)ChildId::LeftButton, small_font);
	m_rightButton.create(130 ,340, 100, 15, L"przypomnij has³o", hwnd(), (HMENU)ChildId::RightButton, small_font);
}

void LogWindow::OnDestroy() {

	// remember to delete object
	if (m_font) {
		DeleteObject(m_font);
		m_font = nullptr;
	}
	if (m_brush) {
		DeleteObject(m_brush);
		m_brush = nullptr;
	}

	PostQuitMessage(0);
}

LRESULT LogWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message)
	{
	case WM_TIMER:
	{
		switch (wParam)
		{
			case TimerId:
			{
				KillTimer(hWnd, TimerId);
				m_red_allert = L"";
				my_InvalidateRect();
				// UpdateWindow(hwnd()); // no need to call this
			} break;
		}
	}break;//WM_TIMER

	case WM_CTLCOLORSTATIC:
	{
		HDC hDC = (HDC)wParam;
		HWND hWndControl = (HWND)lParam;
		if (hwnd_LogWin_CheckBoxLoginData == hWndControl || hwnd_LogWin_CheckBoxAutoLogin == hWndControl)
		{
			SetTextColor(hDC, RGB(0, 72, 0));
			SetBkMode(hDC, TRANSPARENT);
			return (LRESULT)GetStockObject(NULL_BRUSH);
		}
	}break;//WM_CTLCOLORSTATIC

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		OnPaint(ps);
		EndPaint(hWnd, &ps);
	}break;

	case WM_COMMAND:
	{
		OnCommand(wParam, lParam);
	}break;//WM_COMMAND
	}

	return Base::WndProc(hWnd, message, wParam, lParam);
}

void LogWindow::OnCommand(WPARAM wParam, LPARAM lParam) {
	auto id = static_cast<ChildId>(LOWORD(wParam));
	switch (id)
	{
		case ChildId::CenterButton: 
		{
			std::cout << "CenterButton \n";
			//::MessageBox(hwnd_LogWin, L"Tutaj bedzie proba logowania", L"", MB_OK);

			wchar_t login_text[512];
			GetWindowText(hwnd_LogWin_EditLogin, login_text, 512);
			wchar_t password_text[512];
			GetWindowText(hwnd_LogWin_EditPassword, password_text, 512);

			auto login_text_len = wcslen(login_text);
			auto password_text_len = wcslen(password_text);

			if (login_text_len && password_text_len)
			{
				if (f_at_in_login(login_text) == -1)
				{
					SetTimer(hwnd(), TimerId, 2500, nullptr);
					m_red_allert = L"login jest loginem";

				}
				else if (f_at_in_login(login_text) != -1)
				{
					SetTimer(hwnd(), TimerId, 2500, nullptr);
					m_red_allert = L"login jest mailem";
				}
			}
			else if (f_wchar_t_lenght(login_text) == 0 && f_wchar_t_lenght(password_text) == 0)
			{
				SetTimer(hwnd(), TimerId, 2500, nullptr);
				m_red_allert = L"Uzupe³nij pola";
			}
			else if (f_wchar_t_lenght(login_text) == 0 && f_wchar_t_lenght(password_text) != 0)
			{
				SetTimer(hwnd(), TimerId, 2500, nullptr);
				m_red_allert = L"Uzupe³nij login";

			}
			else if (f_wchar_t_lenght(login_text) != 0 && f_wchar_t_lenght(password_text) == 0)
			{
				SetTimer(hwnd(), TimerId, 2500, nullptr);
				m_red_allert = L"Uzupe³nij has³o";
			}

			//::MessageBox(hwnd_LogWin, wide_string1, L"nowe", MB_OK);
			my_InvalidateRect();
		} break;//CenterButton

		case ChildId::LeftButton:
		{
			std::cout << "LeftButton \n";
			myRegWin.create();
			ShowWin(myRegWin.hwnd());
			HideWin(hwnd());

		} break;//ChildId::LeftButton: 

		case ChildId::RightButton:
		{
			std::cout << "RightButton \n";

		} break;//ChildId::RightButton:

		case ChildId::LoginCheckBox:
		{
			bool checked = my_IsDlgButtonChecked(ChildId::LoginCheckBox);
			my_CheckDlgButton(ChildId::LoginCheckBox, !checked);
			my_InvalidateRect();
			std::cout << "costam\n";
		}break;

		case ChildId::AutoLoginCheckBox:
		{
			bool checked = my_IsDlgButtonChecked(ChildId::AutoLoginCheckBox);
			my_CheckDlgButton(ChildId::LoginCheckBox, !checked);
			my_CheckDlgButton(ChildId::AutoLoginCheckBox, !checked);
			EnableWindow(hwnd_LogWin_CheckBoxLoginData, checked);
			my_InvalidateRect();
		}break;
	}
}

void LogWindow::OnPaint(PAINTSTRUCT& ps) {
	SelectObject(ps.hdc, m_font);
	SetTextColor(ps.hdc, RGB(0, 72, 0));
	SetBkMode(ps.hdc, TRANSPARENT);

			RECT rc;

			SetRect(&rc, ((windx - button_size_x) / 2), 50, ((windx - button_size_x) / 2) + button_size_x, 75);
			//::FillRect(ps.hdc, &rc, hBrush);
			// don't create font every single time, it's slow and allocate system resource
			// HFONT hFont = CreateFont(20, 0, 0, 0, 300, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, TEXT("Calibri"));

			DrawText(ps.hdc, L"Podaj login:", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			SetRect(&rc, ((windx - button_size_x) / 2), 100, ((windx - button_size_x) / 2) + button_size_x, 125);
			DrawText(ps.hdc, L"Podaj has³o:", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			
			if (m_red_allert != L"")
			{
				SetTextColor(ps.hdc, RGB(255, 43, 0));
				SetRect(&rc, ((windx - red_allert_size_x) / 2), 150, ((windx - red_allert_size_x) / 2) + red_allert_size_x, 175);
				const wchar_t* szRed_allert_text = m_red_allert.c_str();
				DrawText(ps.hdc, szRed_allert_text, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			}
		
}

void LogWindow::ShowWin(HWND hWnd)
{
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
}

void LogWindow::HideWin(HWND hWnd)
{
	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);
}
