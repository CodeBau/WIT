#pragma once

#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <vector>

class WindowsFlag
{
public:
    int login_window_flag = 1;
    std::vector <std::pair<PCWSTR, bool>> window_hide_flag;
    std::vector <std::pair<PCWSTR, bool>> window_eneble_flag;
};
