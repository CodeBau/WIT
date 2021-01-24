#pragma once

#include <Windows.h>
#include <windowsx.h>

class MyNonCopyable {
public:
	MyNonCopyable() = default;
	MyNonCopyable(const MyNonCopyable&) = delete;
	void operator=(const MyNonCopyable&) = delete;
};

class MyWindowBase : public MyNonCopyable
{
public:
	HWND hwnd() const { return m_hwnd; }

protected:
	void baseCreate(
		DWORD dwExStyle, 
		LPCWSTR lpClassName,
		LPCWSTR lpWindowName,
		DWORD dwStyle,
		int x, int y, int w, int h,
		HWND hWndParent,
		HMENU hMenu);

	static LRESULT CALLBACK s_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnDestroy() {}

	void my_InvalidateRect() { InvalidateRect(m_hwnd, nullptr, true); }

private:

	HWND m_hwnd = nullptr;
};