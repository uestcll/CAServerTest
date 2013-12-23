#ifndef CLSQLITE_H
#define CLSQLITE_H

#include <stdint.h>
#include <sqlite3.h>
#include <iostream>

const std::string tableName = "TB_ServerPublicKey";


class CLSqlite
{
public:
	
	static int Create();
	static int Destroy();

	static CLSqlite* getInstance();
	int sqlexec(const char* sql);
	int sqlqueryexec(const char* sql);
	int sqlPKqueryexec(const uint8_t* name,const uint32_t pktype,uint8_t** result);
	int sqlPKinsertexec(const uint8_t* name,const uint32_t pktype,const uint8_t* pk);
	int sqlPKupdateexec(const uint8_t* name,const uint32_t pktype,const uint8_t* pk);
	int sqlParametersLegalityExamine(const uint8_t* str);
	char** getQueryResult();
	uint8_t* getPKQueryResult();
	int sqlDeleteAllRecords();

private:
	int createPkTable();

	CLSqlite();
	CLSqlite(const char* filename);
	virtual ~CLSqlite();

public:
	static CLSqlite* sqlite;

private:
	sqlite3* db;
	char** Result;
	
	int nRow;
	int nCol;
};
#endif