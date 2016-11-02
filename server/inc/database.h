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

#define CREATE_MO_TABLE "CREATE TABLE MO("\
	                "ID INT PRIMARY KEY NOT NULL," \
	                "NAME           TEXT NOT NULL," \
	                "VAL            INT);"

class DataBase: public DataBaseIf
{
  public:
  DataBase();
  virtual ~DataBase();

  bool addMO(std::vector<std::string>&, int);
  bool modifyMO(std::vector<std::string>&);
  bool deleteMO(std::vector<std::string>&);
  std::vector<std::string> getMO(std::vector<std::string>&);
  std::vector<std::string> printMO(std::vector<std::string>&);
  unsigned getMaxId();

  enum operation{
    GET,
    PRINT,
    ID};

  private:
  static int callback(void*, int, char**, char**);
  sqlite3 *db;
  char *zErrMsg;
  int rc;
  unsigned int objectIdCounter;
  static std::vector<std::string> readData;
  static operation op;

};



#endif /* DATABASE_H_ */
