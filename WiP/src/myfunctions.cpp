#include "../include/myfunctions.h"

HFONT f_myfont(int font_size, LPCWSTR font_type)
{
    return CreateFont(font_size, 0, 0, 0, NULL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, font_type);
}

int f_wchar_t_lenght(wchar_t text[])
{
	int i;
	for (i = 0; i <= 511; i++)
	{
		 if (text[i] == 0)
         {
             break;
         }
	}
	return i;
}

int f_at_in_login(wchar_t text[])
{
    int i;
    for (i = 0; i <= 511; i++)
    {
        if (text[i] == 64)
        {
            break;
        }
        else if (text[i] == 0)
        {
            i = -1;
            break;
        }
    }
    return i;
}

bool isInsideRect(RECT& rc, LONG x, LONG y)
{
    return (x >= rc.left && x <= rc.right && y >= rc.top && y <= rc.bottom);
}





