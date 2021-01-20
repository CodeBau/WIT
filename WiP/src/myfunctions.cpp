#include "../include/myfunctions.h"

/*function to convert TCHAR to a string*/
std::string f_TCHAR2str(TCHAR x[])
{
    std::wstring arr_w(x);
    std::string arr_s(arr_w.begin(), arr_w.end());
    return arr_s;
}

/*function that prints CHAR to the console*/
void f_printT(TCHAR x[])
{
    std::wcout << x << '\n';
}

///*function loading images*/
//void loadImages(HBITMAP img)
//{
//    /*0,0 - default(orginal from file) size of img*/
//    img = (HBITMAP)LoadImageW(NULL, L"Logo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//
//}

HFONT f_myfont(int font_size, LPCWSTR font_type)
{
    return CreateFont(font_size, 0, 0, 0, NULL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, NULL, NULL, NULL, NULL, font_type);
}


std::string f_wchar_t2str (wchar_t text[512])
{
    std::wstring ws_login_text(text);// your new String
    std::string str_login_text(ws_login_text.begin(), ws_login_text.end());// Show String
    return str_login_text;
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