#pragma once

#include "MyWindowBase.h"

class Buttons : public MyWindowBase
{
public:
	static const wchar_t* s_className;

	void create(int x, int y, int w, int h, const wchar_t* name, HWND parent, HMENU menu, int font_size);

	int button_font_size;

private:
	virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	void OnPaint(PAINTSTRUCT ps);

	bool m_down = false;

	void font_and_text_set(int x, int y, PAINTSTRUCT& ps, LPCWSTR text, RECT rc);

	HFONT hFont;
	HBRUSH hBrush;

};
