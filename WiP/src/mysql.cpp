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

void f_sql_create_table2Utf8(HWND hWnd)
{
	MYSQL mysql{ 0 };
	MYSQL_RES* res = nullptr;
	mysql_init(&mysql);

	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");
	mysql_real_connect(&mysql, host, user, psw, db, port, NULL, 0);

	WCHAR name[512]{ 0 };
	int lenUser = SendMessage(hWnd, WM_GETTEXT, 512, (LPARAM)name);

	std::string query = "CREATE TABLE IF NOT EXISTS " + f_sql_str2Utf8(&mysql, name, lenUser) + "(id MEDIUMINT NOT NULL AUTO_INCREMENT, organization_name NCHAR(30) NOT NULL, PRIMARY KEY(id));";
	mysql_query(&mysql, query.c_str());
	mysql_close(&mysql);
}

void f_sql_create_organizations_table()
{
	MYSQL  mysql{ 0 };
	MYSQL_RES* res = nullptr;
	mysql_init(&mysql);

	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");
	mysql_real_connect(&mysql, host, user, psw, db, port, NULL, 0);

	std::string query = "CREATE TABLE IF NOT EXISTS organizations(id MEDIUMINT NOT NULL AUTO_INCREMENT, organization_name NCHAR(30) NOT NULL, PRIMARY KEY(id));";
	mysql_query(&mysql, query.c_str());
	mysql_close(&mysql);

}

void f_sql_insert_organization(HWND hWnd)
{
	std::cout << "Dodajemy organizacje " << std::endl;

	MYSQL mysql{ 0 };
	MYSQL_RES* res = nullptr;
	mysql_init(&mysql);

	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");
	mysql_real_connect(&mysql, host, user, psw, db, port, NULL, 0);

	WCHAR name[512]{ 0 };
	int lenUser = SendMessage(hWnd, WM_GETTEXT, 512, (LPARAM)name);

	std::string query = "INSERT INTO organizations (organization_name) VALUES ('" + f_sql_str2Utf8(&mysql, name, lenUser) + "')";
	mysql_query(&mysql, query.c_str());
	mysql_close(&mysql);

}

void f_sql_check_organization(HWND hWnd)
{
	std::cout << "Sprawdzamy: " << std::endl;
	int qstate;
	MYSQL mysql{ 0 };
	MYSQL_ROW row;
	MYSQL_RES* res = nullptr;
	mysql_init(&mysql);

	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");
	mysql_real_connect(&mysql, host, user, psw, db, port, NULL, 0);

	WCHAR name[512]{ 0 };
	int lenUser = SendMessage(hWnd, WM_GETTEXT, 512, (LPARAM)name);

	//std::string query = "SELECT organization_name FROM organizations WHERE organization_name='" + f_sql_str2Utf8(&mysql, name, lenUser) +"'";
	std::string query = "SELECT organization_name FROM organizations WHERE organization_name='nowa'";

	std::cout << query << std::endl;
	qstate = mysql_query(&mysql, query.c_str());
	std::cout << "qstate: " << qstate <<std::endl;

	//std::string query = "SELECT * FROM organizations";
	if (!qstate)
	{
		std::cout << "!qstate" << std::endl;
		res = mysql_store_result(&mysql);
		std::cout << "res: " << res << std::endl;

		if (row = mysql_fetch_row(res))
		{
			std::cout << "if nowe " << std::endl;

			std::cout << "row: " << row << std::endl;
			//printf("ID: %s, Name: %s, Value: %s\n", row[0], row[1], row[2]);
			printf("Value: %s\n", row[0]);
		}
		else
		{
			std::cout << "else nowe" << std::endl;

		}

	}
	else
	{

		std::cout << "Query failed: " << mysql_error(&mysql) << std::endl;
	}

	mysql_close(&mysql);


}