/*
 * database.cpp
 *
 *  Created on: 23-10-2016
 *      Author: marek
 */

#include "database.h"
#include "trace.h"
#include <stdio.h>
#include <sstream>
#include <vector>

std::vector<std::string> DataBase::readData;

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

    rc = sqlite3_exec(db, CREATE_MO_TABLE, callback, 0, &zErrMsg);
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

bool DataBase::addMO(std::vector<std::string>& attr)
{
  /* Create SQL statement */
  std::stringstream sql;
  sql << "INSERT INTO MO (ID,NAME,VAL) VALUES (" << objectIdCounter << ", '" << attr[1] << "', ";
  if (attr.size() > 2)
  {
    sql << attr[3] << ");";
  }
  else
  {
    sql << "0);";
  }

  rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &zErrMsg);
  if( rc != SQLITE_OK )
  {
    TRACE_ERROR("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    return false;
  }

  ++objectIdCounter;
  return true;
}

bool DataBase::modifyMO(std::vector<std::string>& attr)
{
  /* Create SQL statement */
  std::stringstream sql;
  sql << "UPDATE MO set " << attr[2] << " = " << attr[3] << " WHERE NAME='" << attr[1] << "';";
  rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &zErrMsg);
  if( rc != SQLITE_OK )
  {
    TRACE_ERROR("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    return false;
  }

  return true;
}

bool DataBase::deleteMO(std::vector<std::string>& attr)
{
  /* Create SQL statement */
  std::stringstream sql;
  sql << "DELETE FROM MO WHERE NAME='" << attr[1] << "';";
  rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &zErrMsg);
  if( rc != SQLITE_OK )
  {
    TRACE_ERROR("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    return false;
  }

  return true;
}

std::vector<std::string> DataBase::getMO(std::vector<std::string>& attr)
{
  readData.clear();
  /* Create SQL statement */
  std::stringstream sql;
  sql << "SELECT * FROM MO WHERE NAME='" << attr[1] << "';";
  rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &zErrMsg);
  if( rc != SQLITE_OK )
  {
    TRACE_ERROR("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }

  return readData;
}

int DataBase::callback(void *NotUsed, int argc, char **argv, char **azColName)
{
  int i;
  for(i=0; i<argc; i++)
  {
    TRACE_DEBUG("%s = %s", azColName[i], argv[i] ? argv[i] : "NULL");
    if (strcmp(azColName[i], "ID") && strcmp(azColName[i], "NAME"))
    {
      readData.push_back(azColName[i]);
      readData.push_back(argv[i]);
    }
  }
  return 0;
}
