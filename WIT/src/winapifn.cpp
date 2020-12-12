#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <locale.h>

#include "../include/winapifn.h"

/*function to convert TCHAR to a string*/
std::string T2str(TCHAR x[])
{
    std::wstring arr_w(x);
    std::string arr_s(arr_w.begin(), arr_w.end());
    return arr_s;
}

/*function that prints CHAR to the console*/
void printT(TCHAR x[])
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