#include "CLSqlite.h"

using namespace std;

static CLSqlite::sqlite = 0;
CLSqlite::CLSqlite()
{
	int ret ;
	Result = 0;
	ret = sqlite3_open("test.db",&db);
	if(ret != SQLITE3_OK)
	{
		throw "In CLSqite::CLSqlite(),sqlite3 open error";
	}
	
}

CLSqlite::CLSqlite(const uint8_t* filename)
{
	int ret;
	Result = 0;
	ret = sqlite3_open(filename,&db);
	if(ret != SQLITE3_OK)
	{
		throw "In CLSqite::CLSqlite(),sqlite3 open error";
	}
}

CLSqlite::~CLSqlite()
{

	int ret = sqlite3_close(db);
	if( ret != SQLITE3_OK)
		throw "In CLSqlite::~CLSqlite(),close db error";
}

int CLSqlite::createPkTable()
{
	string sql = "create table " + tableName + "(ID int itentity(1,1),name varchar(20),pktype int,publickey varchar(128));alter table "+tableName+" add PrimaryKey_key primary key(name,pktype);";
	return sqlexec(sql.c_str());
}

int CLSqlite::sqlexec(const char* sql)
{
	int ret = sqlite3_exec(db,sql,0,0,0);
	if(ret != SQLITE3_OK)
		return -1;
	return 0;
}

int CLSqlite::sqlqueryexec(const char* sql)
{
	int row,col;
	char* errmsg;
	int ret = sqlite3_get_table(db,sql,&row,&col,&Result,&errmsg,0);
	nRow = row;
	nCol = col;
	if(ret != SQLITE3_OK)
		return -1;
	return 0;
}

int CLSqlite::sqlPKqueryexec(const uint8_t* name,const uint32_t pktype)
{
	if( sqlParametersLegalityExamine(name) != 0 )
		return -1;
	string sql = "";
	sql = "select publickey from "+ tableName +
		" where name = '"+ name + "' and pktype = "+ pktype;
	return sqlqueryexec(sql.c_str());
}

int CLSqlite::sqlPKinsertexec(const uint8_t* name,const uint32_t pktype,const uint8_t* pk)
{
	if( sqlParametersLegalityExamine(name) != 0 || sqlParametersLegalityExamine(pk) != 0)
		return -1;
	string sql = "";
	sql = "insert into "+ tableName +" values(ID = 1,name = '"
		+ name + "',pktype = "+ pktype + ",publickey = '" + pk + 
		"')";
	return sqlexec(sql.c_str());
}

int CLSqlite::sqlPKupdateexec(const uint8_t* name,const uint32_t pktype,const uint8_t* pk)
{
	if( sqlParametersLegalityExamine(name) != 0 || sqlParametersLegalityExamine(pk) != 0)
		return -1;
	string sql = "";
	sql = "update "+ tableName +" publickey = '"+ pk + "' where name = '"
		+ name + "' and pktype = "+ pktype ;
	return sqlexec(sql.c_str());
}

int CLSqlite::sqlParametersLegalityExamine(const uint8_t* str)
{
	string s = str;
	if(s.find("update") != -1 || s.find("delete") != -1 || s.find("insert")  != -1
		|| s.find("select") != -1 || s.find("drop") != -1 || s.find("add") != -1 
		|| s.find("create") != -1 || s.find("alter") != -1)
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
		memcpy(ret,Result[index],strlen(Result[index]));
		sqlite3_free_table(Result);
		Result = 0;
	}
	return ret;
}

CLSqlite* CLSqlite::getInstance()
{
	if(sqlite == 0)
		sqlite = new CLSqlite;
	return sqlite;
}