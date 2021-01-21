#pragma once

#include <windows.h>
#include <string>

HWND hwnd_LogWin, hwnd_ServerLogWin, hwnd_ServerLogWin_EditLogin, hwnd_ServerLogWin_EditPassword, hwnd_LogWin_EditLogin, hwnd_LogWin_EditPassword, hwnd_LogWin_CheckBoxLoginData, hwnd_LogWin_CheckBoxAutoLogin, hwnd_LogWin_EditOrganization, TIMER_5_sec;

struct GlobalData
{
	bool down = false;
	std::wstring red_allert = L"";
	char flag_LogWin = 'a'; //a - login, b- registration, c - password_reminder
};