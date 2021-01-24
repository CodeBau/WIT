#pragma once

#include <tchar.h>
#include <windows.h>
#include <iostream>

/*my_common.h*/
template<class T>
inline void my_bzero(T& s) {
	memset(&s, 0, sizeof(s));
}

HFONT f_myfont(int font_size, LPCWSTR font_type);

int f_wchar_t_lenght(wchar_t text[]);

int f_at_in_login(wchar_t text[]);

bool isInsideRect(RECT& rc, LONG x, LONG y);

inline void font_and_text_set(int x, int y, PAINTSTRUCT& ps, LPCWSTR text, RECT rc) {}