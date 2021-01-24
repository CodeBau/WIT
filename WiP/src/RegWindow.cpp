#include "../include/RegWindow.h"
#include "../include/myfunctions.h"

const wchar_t* RegWindow::s_className = L"RegWindow";

void RegWindow::create()
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
		wcex.hInstance = GetModuleHandle(nullptr);
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

	baseCreate(0, s_className, L"WiP-rejestracja", //nazwa okna
		WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,
		x, y, windx, windy,
		nullptr, nullptr);

	HWND hwnd_RegWin = hwnd();

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

	hwnd_RegWin_EditLogin = CreateWindowEx(0, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | DT_CENTER | WS_TABSTOP | ES_AUTOHSCROLL, ((windx - edit_size_x) / 2), 75, edit_size_x, edit_size_y, hwnd(), nullptr, hInstance, nullptr);

	m_font = CreateFont(20, 0, 0, 0, 300, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, TEXT("Calibri"));
	m_brush = ::CreateSolidBrush(RGB(0, 72, 0));

	SendMessage(hwnd_RegWin_EditLogin, WM_SETFONT, WPARAM(m_font), TRUE);
	SendMessage(hwnd_RegWin_EditLogin, EM_SETLIMITTEXT, EditLogin_maximum_length, 0);

	hwnd_RegWin_EditPassword = CreateWindowEx(0, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | DT_CENTER | WS_TABSTOP | ES_AUTOHSCROLL, ((windx - edit_size_x) / 2), 125, edit_size_x, edit_size_y, hwnd(), nullptr, hInstance, nullptr);
	SendMessage(hwnd_RegWin_EditPassword, WM_SETFONT, WPARAM(m_font), TRUE);
	SendMessage(hwnd_RegWin_EditPassword, EM_SETLIMITTEXT, EditPassword_maximum_length, 0);

	hwnd_RegWin_EditOrganization = CreateWindowEx(0, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | DT_CENTER | WS_TABSTOP | ES_AUTOHSCROLL, ((windx - edit_size_x) / 2), 175, edit_size_x, edit_size_y, hwnd(), nullptr, hInstance, nullptr);
	SendMessage(hwnd_RegWin_EditOrganization, WM_SETFONT, WPARAM(m_font), TRUE);
	SendMessage(hwnd_RegWin_EditOrganization, EM_SETLIMITTEXT, EditOrganization_maximum_length, 0);

	// ------ Buttons --------

	m_centralButton.create((windx - button_size_x) / 2, (windy - 6 * window_grid), button_size_x, button_size_y, L"utwórz konto", hwnd(), (HMENU)ChildId::CenterButton, normal_font);
	m_leftButton.create(15, 340, 100, 15, L"zaloguj", hwnd(), (HMENU)ChildId::LeftButton, small_font);
	m_rightButton.create(130, 340, 100, 15, L"przypomnij has³o", hwnd(), (HMENU)ChildId::RightButton, small_font);


	//ShowWindow(hwnd(), SW_NORMAL);
	//UpdateWindow(hwnd());
}

void RegWindow::OnDestroy() {

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

LRESULT RegWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

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
		} break;
		}
	}break;//WM_TIMER

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

void RegWindow::OnCommand(WPARAM wParam, LPARAM lParam) {
	auto id = static_cast<ChildId>(LOWORD(wParam));
	switch (id)
	{
		case ChildId::CenterButton: 
			{
				//::MessageBox(hwnd_LogWin, L"Tutaj bêdzie próba utworzenia konta", L"", MB_OK);
				wchar_t login_text[512];
				GetWindowText(hwnd_RegWin_EditLogin, login_text, 512);
				wchar_t password_text[512];
				GetWindowText(hwnd_RegWin_EditPassword, password_text, 512);
				wchar_t organization_text[512];
				GetWindowText(hwnd_RegWin_EditOrganization, organization_text, 512);

				if (f_wchar_t_lenght(login_text) != 0 && f_wchar_t_lenght(password_text) != 0 && f_wchar_t_lenght(organization_text) != 0)
				{
					if (f_at_in_login(login_text) == -1) //login ist just login
					{
						SetTimer(hwnd(), TimerId, 2500, nullptr);
						m_red_allert = L"SprawdŸ poprawnoœæ adresu e-mail";
					}

					else if (f_wchar_t_lenght(password_text) < 6)
					{
						SetTimer(hwnd(), TimerId, 2500, nullptr);
						m_red_allert = L"Has³o musi mieæ minimum 6 znaków";
					}

					else if (f_wchar_t_lenght(organization_text) < 3)
					{
						SetTimer(hwnd(), TimerId, 2500, nullptr);
						m_red_allert = L"Nazwa organizacji musi mieæ minimum 3 znaki";
					}
					else if (f_at_in_login(login_text) != -1 && f_wchar_t_lenght(password_text) >= 6 && f_wchar_t_lenght(organization_text) >= 3)
					{
						SetTimer(hwnd(), TimerId, 2500, nullptr);
						m_red_allert = L"Wszystko siê zgadza mo¿na zak³adaæ organizacjê";
						//f_sql_create_organizations_table();
						//f_sql_check_organization(hwnd_LogWin_EditOrganization);
						//f_sql_insert_organization(hwnd_LogWin_EditOrganization);
					}
				}
				else if (f_wchar_t_lenght(login_text) == 0 && f_wchar_t_lenght(password_text) == 0 && f_wchar_t_lenght(organization_text) == 0)
				{
					SetTimer(hwnd(), TimerId, 2500, nullptr);
					m_red_allert = L"Uzupe³nij pola";
				}
				else if ((f_wchar_t_lenght(login_text) != 0 && f_wchar_t_lenght(password_text) == 0 && f_wchar_t_lenght(organization_text) == 0) || (f_wchar_t_lenght(login_text) == 0 && f_wchar_t_lenght(password_text) != 0 && f_wchar_t_lenght(organization_text) == 0) || (f_wchar_t_lenght(login_text) == 0 && f_wchar_t_lenght(password_text) == 0 && f_wchar_t_lenght(organization_text) != 0))
				{
					SetTimer(hwnd(), TimerId, 2500, nullptr);
					m_red_allert = L"Uzupe³nij brakuj¹ce pola";
				}
				else if (f_wchar_t_lenght(login_text) == 0 && f_wchar_t_lenght(password_text) != 0 && f_wchar_t_lenght(organization_text) != 0)
				{
					SetTimer(hwnd(), TimerId, 2500, nullptr);
					m_red_allert = L"Uzupe³nij login";
				}
				else if (f_wchar_t_lenght(login_text) != 0 && f_wchar_t_lenght(password_text) == 0 && f_wchar_t_lenght(organization_text) != 0)
				{
					SetTimer(hwnd(), TimerId, 2500, nullptr);
					m_red_allert = L"Uzupe³nij has³o";
				}
				else if (f_wchar_t_lenght(login_text) != 0 && f_wchar_t_lenght(password_text) != 0 && f_wchar_t_lenght(organization_text) == 0)
				{
					SetTimer(hwnd(), TimerId, 2500, nullptr);
					m_red_allert = L"Uzupe³nij nazwê organizacji";
				}
				my_InvalidateRect();
			} break;//ChildId::CenterButton: 

		case ChildId::LeftButton:
		{
		
		} break;//ChildId::LeftButton: 

		case ChildId::RightButton:
		{

		} break;//ChildId::RightButton:
	}//switch (id)
}

void RegWindow::OnPaint(PAINTSTRUCT& ps) {
	SelectObject(ps.hdc, m_font);
	SetTextColor(ps.hdc, RGB(0, 72, 0));
	SetBkMode(ps.hdc, TRANSPARENT);

	RECT rc;
	SetRect(&rc, ((windx - 450) / 2), 50, ((windx - 450) / 2) + 450, 75);

	DrawText(ps.hdc, L"Podaj adres e-mail (adres e-mail jest jednoczeœnie loginem):", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	SetRect(&rc, ((windx - button_size_x) / 2), 100, ((windx - button_size_x) / 2) + button_size_x, 125);
	DrawText(ps.hdc, L"Utwórz has³o logowania:", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	SetRect(&rc, ((windx - button_size_x) / 2), 150, ((windx - button_size_x) / 2) + button_size_x, 175);
	DrawText(ps.hdc, L"Podaj nazwê organizacji:", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	if (m_red_allert != L"")
	{
		SetTextColor(ps.hdc, RGB(255, 43, 0));
		SetRect(&rc, ((windx - red_allert_size_x) / 2), 200, ((windx - red_allert_size_x) / 2) + red_allert_size_x, 225);
		const wchar_t* szRed_allert_text = m_red_allert.c_str();
		DrawText(ps.hdc, szRed_allert_text, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}
}

void RegWindow::ShowWin(HWND hWnd)
{
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
}

void RegWindow::HideWin(HWND hWnd)
{
	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);
}