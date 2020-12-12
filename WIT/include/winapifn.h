#pragma once

#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <string>

/*see console.cpp for function definitions */
//extern void CreateConsole(); 
extern std::string T2str(TCHAR x[]);
extern void printT(TCHAR x[]);
//extern void loadImages(HBITMAP img);

/*function that prints everything (! except CHAR!) to the console.*/
template <typename T>
void print(const T& x)
{
	std::cout << x << '\n';
}