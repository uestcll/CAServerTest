#include "CLSqlite.h"
#include "CLLogger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

#define DB_PATH "/tmp/"

CLSqlite* CLSqlite::sqlite = 0;

CLSqlite::CLSqlite()
{
	int ret ;
	Result = 0;
	
	string s = DB_PATH ;
	s += "test.db";
	s = ":memory:";
	
	ret = sqlite3_open(s.c_str(),&db);

	
	if(ret != SQLITE_OK)
	{
		sqlite3_close(db);
		throw "In CLSqite::CLSqlite(),sqlite3 open error";
	}

	createPkTable();
	
}

CLSqlite::CLSqlite(const char* filename)
{
	int ret;
	Result = 0;

	string s = DB_PATH ;
	s += filename;
	ret = sqlite3_open(s.c_str(),&db);
	
	if(ret != SQLITE_OK)
	{
		sqlite3_close(db);
		throw "In CLSqite::CLSqlite(),sqlite3 open error";
	}

	createPkTable();
}

CLSqlite::~CLSqlite()
{

	int ret = sqlite3_close(db);
	
	if( ret != SQLITE_OK)
		throw "In CLSqlite::~CLSqlite(),close db error";
}

int CLSqlite::createPkTable()
{
	string sql = "create table " + tableName + "(ID int itentity(1,1),name varchar(20),pktype int,publickey varchar(128),primary key(name,pktype));";
	return sqlexec(sql.c_str());
}

int CLSqlite::sqlexec(const char* sql)
{
	int ret = sqlite3_exec(db,sql,0,0,0);
	
	if(ret != SQLITE_OK)
		return -1;
	return 0;
}

int CLSqlite::sqlqueryexec(const char* sql)
{
	int row,col;
	char* errmsg;
	
	int ret = sqlite3_get_table(db,sql,&Result,&row,&col,&errmsg);
	nRow = row;
	nCol = col;
	
	if(ret != SQLITE_OK)
		return -1;

	return 0;
}

int CLSqlite::sqlPKqueryexec(const uint8_t* name,const uint32_t pktype,uint8_t** result)
{
	*result = 0;

	if( sqlParametersLegalityExamine(name) != 0 )
		return -1;

	string sql = "";
	
	char type[20];
	memset(type,0,20);
	sprintf(type,"%d",pktype);

	sql = "select publickey from "+ tableName +
		" where name = '"+ (char*)name + "' and pktype = "+ type;
	
	int ret = sqlqueryexec(sql.c_str());

	if(ret == -1)
		return -1;

	*result = getPKQueryResult();

	return 0;

}

int CLSqlite::sqlPKinsertexec(const uint8_t* name,const uint32_t pktype,const uint8_t* pk)
{
	if( sqlParametersLegalityExamine(name) != 0 || sqlParametersLegalityExamine(pk) != 0)
		return -1;
	
	char type[20];
	memset(type,0,20);
	sprintf(type,"%d",pktype);

	string sql = "";
	sql = "insert into "+ tableName +"(name,pktype,publickey) values('"
		+ (char*)name + "',"+ type + ",'" + (char*)pk + 
		"')";
	
	return sqlexec(sql.c_str());
}

int CLSqlite::sqlPKupdateexec(const uint8_t* name,const uint32_t pktype,const uint8_t* pk)
{
	if( sqlParametersLegalityExamine(name) != 0 || sqlParametersLegalityExamine(pk) != 0)
		return -1;
	
	char type[20];
	memset(type,0,20);
	sprintf(type,"%d",pktype);

	string sql = "";
	sql = "update "+ tableName +" set publickey = '"+ (char*)pk + "' where name = '"
		+ (char*)name + "' and pktype = "+ type ;

	return sqlexec(sql.c_str());
}

int CLSqlite::sqlParametersLegalityExamine(const uint8_t* str)
{
	string s = (char*)str;
/*	if(s.find("update") != -1 || s.find("delete") != -1 || s.find("insert")  != -1
		|| s.find("select") != -1 || s.find("drop") != -1 || s.find("add") != -1 
		|| s.find("create") != -1 || s.find("alter") != -1)
		return -1;
*/
	if(s.find(";") != -1)
		return -1;

	return 0;
}

char** CLSqlite::getQueryResult()
{

	if(Result != 0)
	{
		sqlite3_free_table(Result);
		Result = 0;
	}

	return Result;
}

uint8_t* CLSqlite::getPKQueryResult()
{
	int index = nCol;
	uint8_t* ret = 0;
	
	if(Result != 0)
	{

		if(nCol == 0|| nRow == 0)
		{
			sqlite3_free_table(Result);
			return 0;
		}
		ret = new uint8_t[strlen(Result[index])+1];
		memset(ret,0,strlen(Result[index])+1);

		memcpy(ret,Result[index],strlen(Result[index]));
		sqlite3_free_table(Result);
		Result = 0;
	}

	return ret;
}

CLSqlite* CLSqlite::getInstance()
{
	return sqlite;
}

int CLSqlite::Create()
{
	if(sqlite != 0)
		return 0;

	try
	{
		sqlite = new CLSqlite;
	}
	catch(const char* str)
	{
		CLLogger::WriteLogMsg(str,0);
		return -1;
	}

	return 0;
}

int CLSqlite::Destroy()
{
	if(sqlite == 0)
		return 0;

	try
	{
		delete sqlite;
	}
	catch (const char* str)
	{
		CLLogger::WriteLogMsg(str,0);
		return -1;
	}

	return 0;
}

int CLSqlite::sqlDeleteAllRecords()
{
	string sql = "delete from " + tableName +  ";";

	return sqlexec(sql.c_str());
}