/*
 * database.cpp
 *
 *  Created on: 23-10-2016
 *      Author: marek
 */

#include "database.h"
#include "trace.h"
#include <stdio.h>

DataBase::DataBase()
:zErrMsg(0),objectIdCounter(0)
{
  rc = sqlite3_open("mo.db", &db);
  if( rc )
  {
    TRACE_ERROR("Can't open database: %s\n", sqlite3_errmsg(db));
  }
  else
  {
    TRACE_DEBUG("Opened database successfully\n");

    /* Create SQL statement */
    sql = "CREATE TABLE MO("\
	  "ID INT PRIMARY KEY     NOT NULL," \
	  "NAME           TEXT    NOT NULL," \
	  "ATTR1          INT);";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK )
    {
    TRACE_ERROR("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    }
    else
    {
      TRACE_DEBUG("Table created successfully\n");
    }
  }
}

DataBase::~DataBase()
{
  sqlite3_close(db);
}

bool DataBase::addMO(std::string name, void* attr)
{
  moMap.insert(std::pair<unsigned int, std::string>(objectIdCounter, name));
  ++objectIdCounter;
  /* Create SQL statement */
  sql = "INSERT INTO MO (ID,NAME,ATTR1) "  \
        "VALUES (1, 'kotek', 666);";

  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if( rc != SQLITE_OK )
  {
    TRACE_ERROR("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
  else
  {
    TRACE_DEBUG("Record created successfully\n");
  }

  return true;
}

bool DataBase::modifyMO(std::string name, void* attr)
{
  for (std::map<unsigned int, std::string>::iterator it = moMap.begin(); it != moMap.end(); ++it)
  {
    if (it->second.compare(name) == 0)
    {
      return true;
    }
  }
  return false;
}

bool DataBase::deleteMO(std::string name)
{
  for (std::map<unsigned int, std::string>::iterator it = moMap.begin(); it != moMap.end(); ++it)
  {
    if (it->second.compare(name) == 0)
    {
      moMap.erase(it);
      return true;
    }
  }
  return false;
}

void* DataBase::getMO(std::string name)
{
  for (std::map<unsigned int, std::string>::iterator it = moMap.begin(); it != moMap.end(); ++it)
  {
    TRACE_DEBUG("iterating '%s', '%s'", it->second.c_str(), name.c_str());
    if (it->second.compare(name) == 0)
    {
      return (void*)new std::string(name);
    }
  }
  return NULL;
}

int DataBase::callback(void *NotUsed, int argc, char **argv, char **azColName)
{
  int i;
  for(i=0; i<argc; i++)
  {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}
