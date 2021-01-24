#include "../include/Buttons.h"
#include "../include/myfunctions.h"

const wchar_t* Buttons::s_className = L"Buttons";

void Buttons::create(int x, int y, int w, int h, const wchar_t* name, HWND parent, HMENU menu, int font_size)
{
	button_font_size = font_size;

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
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = s_className;
		wcex.hIconSm = nullptr;

		RegisterClassEx(&wcex);
	}

	baseCreate(0, s_className, name,
		WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		x, y, w, h,
		parent, menu);
}

LRESULT Buttons::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_PAINT: {
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		OnPaint(ps);
		EndPaint(hWnd, &ps);
	}break;//WM_PAINT

	case WM_LBUTTONDOWN: {
		m_down = true;
		SetCapture(hWnd);
		InvalidateRect(hWnd, nullptr, false);
	}break;//WM_LBUTTONDOWN

	case WM_LBUTTONUP: {
		m_down = false;
		ReleaseCapture();
		InvalidateRect(hWnd, nullptr, false);

		auto x = GET_X_LPARAM(lParam);
		auto y = GET_Y_LPARAM(lParam);

		RECT rc;
		GetClientRect(hWnd, &rc);

		if (isInsideRect(rc, x, y))
		{
			// SendMessage(hWnd, WM_COMMAND, (WPARAM)GetMenu(hWnd), 0);

			// should send to parent to handle WM_COMMAND, so this button can be reused
			SendMessage(GetParent(hWnd), WM_COMMAND, (WPARAM)GetMenu(hWnd), (LPARAM)hWnd);
		}
	}break;//WM_LBUTTONUP

	}//switch (message)

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void Buttons::OnPaint(PAINTSTRUCT ps) 
{
	
	RECT rc;
	GetClientRect(hwnd(), &rc);
	Rectangle(ps.hdc, rc.left, rc.top, rc.right, rc.bottom);

	wchar_t text[512];
	GetWindowText(hwnd(), text, 512);
	SetBkMode(ps.hdc, TRANSPARENT);

	std::wcout << text << "\n";
	if (Buttons::m_down)
	{
		SetTextColor(ps.hdc, RGB(240, 240, 240));
		HBRUSH hBrush = ::CreateSolidBrush(RGB(0, 72, 0));
		::FillRect(ps.hdc, &rc, hBrush);
		font_and_text_set(button_font_size, 700, ps, text, rc);
	}
	else
	{
		SetTextColor(ps.hdc, RGB(0, 72, 0));
		SelectObject(ps.hdc, GetStockObject(WHITE_BRUSH));
		font_and_text_set(button_font_size, 300, ps, text, rc);
	}
}

void Buttons::font_and_text_set(int x, int y, PAINTSTRUCT& ps, LPCWSTR text, RECT rc)
{
	hFont = CreateFont(x, 0, 0, 0, y, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, TEXT("Calibri"));
	SelectObject(ps.hdc, hFont);
	DrawText(ps.hdc, text, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

