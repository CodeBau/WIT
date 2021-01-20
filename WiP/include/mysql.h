#pragma once

#include <mysql.h>
#include <vector>
#include <string>

std::string f_sql_str2Utf8(MYSQL* mysql, WCHAR* wStr, int wLen);

void f_sql_create_table2Utf8(HWND hLoginWnd);

void f_sql_create_organizations_table();

void f_sql_insert_organization(HWND hWnd);

void f_sql_check_organization(HWND hWnd);