#include "..\include\MyWindowBase.h"


void MyWindowBase::baseCreate(
	DWORD dwExStyle, 
	LPCWSTR lpClassName,
	LPCWSTR lpWindowName,
	DWORD dwStyle,
	int x, int y, int w, int h,
	HWND hWndParent,
	HMENU hMenu)
{

	HINSTANCE hInst = GetModuleHandle(nullptr);
	m_hwnd = CreateWindowEx(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, w, h, hWndParent, hMenu, hInst, this);
}

LRESULT MyWindowBase::s_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message)
	{
		case WM_CREATE: {
			auto* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
			LONG_PTR data = reinterpret_cast<intptr_t>(cs->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, data);
		}break;

		case WM_DESTROY: {
			auto* p = reinterpret_cast<MyWindowBase*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			if (p) {
				p->OnDestroy();
			}

			SetWindowLongPtr(hWnd, GWLP_USERDATA, 0); // clean up
			return DefWindowProc(hWnd, message, wParam, lParam);
		}break;
	}

	auto* p = reinterpret_cast<MyWindowBase*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (!p) {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}		

	return p->WndProc(hWnd, message, wParam, lParam);
}

LRESULT MyWindowBase::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hWnd, message, wParam, lParam);
}


