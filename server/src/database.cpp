/*
 * database.cpp
 *
 *  Created on: 23-10-2016
 *      Author: marek
 */

#include "database.h"
#include "trace.h"
#include <stdio.h>
#include <vector>
#include <sstream>
#include <stdlib.h>

using namespace std;

vector<string> DataBase::readData;

DataBase::DataBase()
:zErrMsg(0)
{
  rc = sqlite3_open(DB_NAME, &db);
  if(!rc)
  {
    TRACE_DEBUG("Opened database successfully\n");
  }
  else
  {
    TRACE_ERROR("Can't open database: %s\n", sqlite3_errmsg(db));
  }
}

DataBase::~DataBase()
{
  sqlite3_close(db);
}

bool DataBase::addMO(unsigned objectId, unsigned type, string& name, vector<string>& attrNames, vector<string>& attrValues)
{
  /* find mo_type */
  string table;
  if (!getTypeTable(type, table))
  {
    return false;
  }

  /* add mo to proper type table */
  stringstream sql;
  sql << "insert into " << table << "(id,name";
  for (vector<string>::iterator it = attrNames.begin(); it != attrNames.end(); ++it)
  {
    sql << "," << *it;
  }
  sql << ")values(" << objectId << ",'" << name << "'";
  for (vector<string>::iterator it = attrValues.begin(); it != attrValues.end(); ++it)
  {
    sql << ",'" << *it << "'";
  }
  sql << ");";
  TRACE_DEBUG("addMO sql: %s", sql.str().c_str());
  rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &zErrMsg);
  if(rc != SQLITE_OK)
  {
    TRACE_ERROR("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    return false;
  }

  /*add mo to mo_list */
  sql.str("");
  sql << "insert into " << MO_LIST_TABLE << "(id,type,name)values(" << objectId << "," << type << ",'" << name << "');";
  TRACE_DEBUG("addMO sql: %s", sql.str().c_str());
  rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &zErrMsg);
  if(rc != SQLITE_OK)
  {
    TRACE_ERROR("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    return false;
  }

  return true;
}

bool DataBase::modifyMO(unsigned objectId, unsigned type, string& name, vector<string>& attrNames, vector<string>& attrValues)
{
  /* find mo_type */
  string table;
  if (!getTypeTable(type, table))
  {
    return false;
  }

  /* modify mo */
  stringstream sql;
  sql << "update " << table << " set ";
  for (unsigned i = 0; i < attrNames.size(); ++i)
  {
    sql << attrNames[i] << "='" << attrValues[i] << "'";
    if (i + 1 < attrNames.size()) sql << ",";
  }
  sql << "where name='" << name << "';";
  TRACE_DEBUG("modifyMO sql: %s", sql.str().c_str());
  rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &zErrMsg);
  if( rc != SQLITE_OK )
  {
    TRACE_ERROR("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    return false;
  }

  return true;
}

bool DataBase::deleteMO(unsigned objectId, unsigned type, string& name)
{
  /* find mo_type */
  string table;
  if (!getTypeTable(type, table))
  {
    return false;
  }

  /* delete from type table */
  stringstream sql;
  sql << "delete from " << table << " where name='" << name << "';";
  TRACE_DEBUG("deleteMO sql: %s", sql.str().c_str());
  rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &zErrMsg);
  if( rc != SQLITE_OK )
  {
    TRACE_ERROR("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    return false;
  }

  /* delete from mo_list table */
  sql.str("");
  sql << "delete from " << MO_LIST_TABLE << " where name='" << name << "';";
  TRACE_DEBUG("deleteMO sql: %s", sql.str().c_str());
  if( rc != SQLITE_OK )
  {
    TRACE_ERROR("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    return false;
  }

  return true;
}

bool DataBase::getMO(string& name, vector<string>& out)
{
  /* find type in mo_list table */
  stringstream sql;
  sql << "select type from " << MO_LIST_TABLE << " where name='" << name << "';";
  TRACE_DEBUG("getMO sql: %s", sql.str().c_str());
  rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &zErrMsg);
  if( rc != SQLITE_OK )
  {
    TRACE_ERROR("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    return false;
  }

  /* find mo in proper type table */
  sql.str("");
  sql << "select * from " << readData.front() << " where name='" << name << "';";
  TRACE_DEBUG("getMO sql: %s", sql.str().c_str());
  rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &zErrMsg);
  if( rc != SQLITE_OK )
  {
    TRACE_ERROR("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    return false;
  }

  out = readData;
  return true;
}

bool DataBase::printMO(string& name, vector<string>& out)
{
  /* find all matching mo */
  std::stringstream sql;
  sql << "select name from " << MO_LIST_TABLE << " where name like '" << name << "%';";
  TRACE_DEBUG("printMO sql: %s", sql.str().c_str());
  rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &zErrMsg);
  if( rc != SQLITE_OK )
  {
    TRACE_ERROR("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    return false;
  }

  out = readData;
  return true;
}

unsigned DataBase::getMaxId()
{
  /* find max id in mo_list table */
  stringstream sql;
  sql << "select MAX(id) from " << MO_LIST_TABLE << ";";
  TRACE_DEBUG("getMaxId sql: %s", sql.str().c_str());
  rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &zErrMsg);
  if( rc != SQLITE_OK )
  {
    TRACE_ERROR("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }

  return !readData.empty() && readData.front() != "" ? atoi(readData.front().c_str()) : 0;
}

unsigned DataBase::getObjectId(unsigned type, std::string& name)
{
  /* find id in mo_list table */
  stringstream sql;
  sql << "select id from " << MO_LIST_TABLE << " where name='" << name << "';";
  TRACE_DEBUG("getObjectId sql: %s", sql.str().c_str());
  rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &zErrMsg);
  if( rc != SQLITE_OK )
  {
    TRACE_ERROR("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }

  return !readData.empty() && readData.front() != "" ? atoi(readData.front().c_str()) : 0;
}

bool DataBase::getTypeTable(unsigned type, std::string& table)
{
  /* find mo_type */
  stringstream sql;
  sql << "select name from " << MO_TYPES_TABLE << " where type=" << type;
  TRACE_DEBUG("getTypeTable sql: %s", sql.str().c_str());
  rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &zErrMsg);
  if(rc != SQLITE_OK)
  {
    TRACE_ERROR("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    return false;
  }

  table = readData.front();
  return true;
}

int DataBase::callback(void *NotUsed, int argc, char **argv, char **azColName)
{
  readData.clear();
  int i;
  for(i=0; i<argc; i++)
  {
    TRACE_DEBUG("%s = %s", azColName[i], argv[i] ? argv[i] : "NULL");
    readData.push_back(argv[i] ? argv[i] : "");
  }
  return 0;
}
