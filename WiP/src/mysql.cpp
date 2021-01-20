#include "../include/mysql.h"
#include "../include/mystaf.h"
//#include "../include/mystaf_example.h"

std::string f_sql_str2Utf8(MYSQL* mysql, WCHAR* wStr, int wLen)
{
	int utf8len = WideCharToMultiByte(CP_UTF8, 0, wStr, wLen, NULL, 0, NULL, NULL);
	if (utf8len > 0)
	{
		std::vector<char> utf8(utf8len);
		utf8len = WideCharToMultiByte(CP_UTF8, 0, wStr, wLen, &utf8[0], utf8len, NULL, NULL);
		if (utf8len > 0)
		{
			std::vector<char> escaped(utf8len * 2 + 1);
			unsigned long escapedLen = mysql_real_escape_string(mysql, &escaped[0], &utf8[0], utf8len);
			if (escapedLen > 0)
				return std::string(&escaped[0], escapedLen);
		}
	}

	return std::string();
}

void f_sql_create_table2Utf8(HWND hLoginWnd)
{
	MYSQL* con{ 0 }, mysql{ 0 };
	MYSQL_RES* res = nullptr;
	mysql_init(&mysql);

	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");
	mysql_real_connect(&mysql, host, user, psw, db, port, NULL, 0);

	WCHAR name[512]{ 0 };
	int lenUser = SendMessage(hLoginWnd, WM_GETTEXT, 512, (LPARAM)name);

	std::string query = "CREATE TABLE IF NOT EXISTS " + f_sql_str2Utf8(&mysql, name, lenUser) + "(id MEDIUMINT NOT NULL AUTO_INCREMENT, organization_name NCHAR(30) NOT NULL, PRIMARY KEY(id));";
	mysql_query(&mysql, query.c_str());
	mysql_close(con);
}

void f_sql_create_table(std::string table_name)
{
	MYSQL* con{ 0 }, mysql{ 0 };
	MYSQL_RES* res = nullptr;
	mysql_init(&mysql);

	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");
	mysql_real_connect(&mysql, host, user, psw, db, port, NULL, 0);

	std::string query = "CREATE TABLE IF NOT EXISTS " + table_name + "(id MEDIUMINT NOT NULL AUTO_INCREMENT, organization_name NCHAR(30) NOT NULL, PRIMARY KEY(id));";
	mysql_query(&mysql, query.c_str());
	mysql_close(con);
}

void f_sql_insert_into_table2Utf8(HWND hLoginWnd)
{
	MYSQL* con{ 0 }, mysql{ 0 };
	MYSQL_RES* res = nullptr;
	mysql_init(&mysql);

	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");
	mysql_real_connect(&mysql, host, user, psw, db, port, NULL, 0);

	WCHAR name[512]{ 0 };
	int lenUser = SendMessage(hLoginWnd, WM_GETTEXT, 512, (LPARAM)name);

	std::string query = "CREATE TABLE IF NOT EXISTS " + f_sql_str2Utf8(&mysql, name, lenUser) + "(id MEDIUMINT NOT NULL AUTO_INCREMENT, name NCHAR(30) NOT NULL, PRIMARY KEY(id));";
	mysql_query(&mysql, query.c_str());
	mysql_close(con);
}