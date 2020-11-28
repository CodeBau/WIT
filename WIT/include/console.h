#pragma once

#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <string>

extern void CreateConsole(); 
extern std::string T2str(TCHAR x[]);
extern void printT(TCHAR x[]);
template <typename T>
void print(const T& x)
{
	std::cout << x << '\n';
}