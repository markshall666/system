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

#define DB_NAME "mo.db"
#define MO_TYPES_TABLE "mo_types"
#define MO_LIST_TABLE "mo_list"

class DataBase: public DataBaseIf
{
  public:
  DataBase();
  virtual ~DataBase();

  bool addMO(unsigned, unsigned, std::string&, std::vector<std::string>&, std::vector<std::string>&);
  bool modifyMO(unsigned, unsigned, std::string&, std::vector<std::string>&, std::vector<std::string>&);
  bool deleteMO(unsigned, unsigned, std::string&);
  bool getMO(std::string&, std::vector<std::string>&);
  bool printMO(std::string&, std::vector<std::string>&);
  unsigned getMaxId();
  unsigned getObjectId(unsigned, std::string&);

  private:
  static int callback(void*, int, char**, char**);
  bool getTypeTable(unsigned, std::string&);
  sqlite3 *db;
  char *zErrMsg;
  int rc;
  static std::vector<std::string> readData;

};

#endif /* DATABASE_H_ */
