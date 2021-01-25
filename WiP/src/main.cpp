#include <tchar.h>
#include <Windows.h>
#include <windowsx.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cassert>
#include <locale>
// #include <mysql.h>

#include "../resource.h"
#include "../include/myfunctions.h"
#include "../include/myvariable.h"
#include "../include/MyBitmap.h"
#include "../include/mysql.h"

#include "../include/LogWindow.h"
#include "../include/RegWindow.h"
#include "../include/PswdRmdWindow.h"

//  Declare Windows procedure

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	LogWindow myLogWin;
	myLogWin.create();
	
	RegWindow myRegWin;
	myRegWin.create();
	
	PswdRmdWindow myPswdRmdWin;
	myPswdRmdWin.create();

	myLogWin.LogWinRegWinRef =  & myRegWin;


	myLogWin.ShowWin(true);
	myRegWin.ShowWin(false); 
	myPswdRmdWin.ShowWin(false);

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) > 0)
	{
		if (!IsDialogMessage(hwnd_LogWin, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

int main() {
	return WinMain(GetModuleHandle(nullptr), nullptr, GetCommandLineA(), SW_NORMAL);
}