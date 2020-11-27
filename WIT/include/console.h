#pragma once

#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <string>

extern void CreateConsole(); 
extern std::string T2str(TCHAR x[]);

template<typename T>
void print(const T& x)
{
	//getting typename
	std::string temptype = typeid(x).name();

	//checking if it contains typeid TCHAR
	size_t isTCHAR = temptype.find("wchar_t const");

	if (isTCHAR==0)
	{
		std::cout << "JEST TCHAR" <<'\n';
	}
	else
	{
		std::cout << "NIE JEST TCHAR" << '\n';

	}
	//std::cout << x <<'\n';
	
}

/*void print(TCHAR x[])
{
	std::wcout << x << '\n';
}
*/