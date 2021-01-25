#include "../include/PswdRmdWindow.h"
#include "../include/myfunctions.h"

const wchar_t* PswdRmdWindow::s_className = L"PswdRmdWindow";

void PswdRmdWindow::create()
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

	hwnd_PswdRmdWin_EditLogin = CreateWindowEx(0, L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | DT_CENTER | WS_TABSTOP | ES_AUTOHSCROLL, ((windx - edit_size_x) / 2), 75, edit_size_x, edit_size_y, hwnd(), nullptr, hInstance, nullptr);

	m_font = CreateFont(20, 0, 0, 0, 300, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, TEXT("Calibri"));
	m_brush = ::CreateSolidBrush(RGB(0, 72, 0));

	SendMessage(hwnd_PswdRmdWin_EditLogin, WM_SETFONT, WPARAM(m_font), TRUE);
	SendMessage(hwnd_PswdRmdWin_EditLogin, EM_SETLIMITTEXT, EditLogin_maximum_length, 0);


	// ------ Buttons --------

	m_centralButton.create((windx - button_size_x) / 2, (windy - 6 * window_grid), button_size_x, button_size_y, L"przypomnij has³o", hwnd(), (HMENU)ChildId::CenterButton, normal_font);
	m_leftButton.create(15, 340, 100, 15, L"zaloguj", hwnd(), (HMENU)ChildId::LeftButton, small_font);
	m_rightButton.create(130, 340, 100, 15, L"utwórz konto", hwnd(), (HMENU)ChildId::RightButton, small_font);

}

void PswdRmdWindow::OnDestroy() {

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

LRESULT PswdRmdWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

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

void PswdRmdWindow::OnCommand(WPARAM wParam, LPARAM lParam) {
	auto id = static_cast<ChildId>(LOWORD(wParam));
	switch (id)
	{
		case ChildId::CenterButton: {
			// TODO
			std::cout << "CenterButton\n";
		} break;

	
	}
}

void PswdRmdWindow::OnPaint(PAINTSTRUCT& ps) {
	SelectObject(ps.hdc, m_font);
	SetTextColor(ps.hdc, RGB(0, 72, 0));
	SetBkMode(ps.hdc, TRANSPARENT);

	RECT rc;
	SetRect(&rc, ((windx - 450) / 2), 50, ((windx - 450) / 2) + 450, 75);
	DrawText(ps.hdc, L"Podaj adres e-mail:", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

}

void PswdRmdWindow::ShowWin(bool istrue)
{
	if (istrue == true)
	{
		ShowWindow(hwnd(), SW_SHOW);
		UpdateWindow(hwnd());
	}
	else if (istrue == false)
	{
		ShowWindow(hwnd(), SW_HIDE);
		UpdateWindow(hwnd());
	}
}
