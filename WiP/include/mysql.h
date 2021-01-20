#pragma once

#include <mysql.h>
#include <vector>
#include <string>

std::string f_sql_str2Utf8(MYSQL* mysql, WCHAR* wStr, int wLen);

void f_sql_create_table2Utf8(HWND hLoginWnd);

void f_sql_create_table(std::string table_name);

void f_sql_insert_into_table2Utf8(HWND hLoginWnd);