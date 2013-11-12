#ifndef CLSQLITE_H
#define CLSQLITE_H

#include <stdint.h>
#include <sqlite3.h>
#include <iostream>

const std::string tableName = "TB_ServerPublicKey";
class CLSqlite
{
public:
	CLSqlite();
	CLSqlite(const uint8_t* filename);
	virtual ~CLSqlite();

	static CLSqlite* getInstance();
	int sqlexec(const char* sql);
	int sqlqueryexec(const char* sql);
	int sqlPKqueryexec(const uint8_t* name,const uint32_t pktype);
	int sqlPKinsertexec(const uint8_t* name,const uint32_t pktype,const uint8_t* pk);
	int sqlPKupdateexec(const uint8_t* name,const uint32_t pktype,const uint8_t* pk);
	int sqlParametersLegalityExamine(const uint8_t* str);
	char** getQueryResult();
	uint8_t* getPKQueryResult();

private:
	int createPkTable();
private:
	sqlite3* db;
	uint8_t** Result;
	static CLSqlite* sqlite;
	int nRow;
	int nCol;
};
#endif