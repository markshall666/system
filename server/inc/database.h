/*
 * database.h
 *
 *  Created on: 23-10-2016
 *      Author: marek
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include "databaseIf.h"
#include <map>
#include <string>
#include <sqlite3.h>

class DataBase: public DataBaseIf
{
  public:
  DataBase();
  virtual ~DataBase();

  bool addMO(std::string, void*);
  bool modifyMO(std::string, void*);
  bool deleteMO(std::string);
  void* getMO(std::string);

  private:
  static int callback(void*, int, char**, char**);
  sqlite3 *db;
  char *zErrMsg;
  int rc;
  char *sql;
  std::map<unsigned int, std::string> moMap;
  unsigned int objectIdCounter;

};



#endif /* DATABASE_H_ */
