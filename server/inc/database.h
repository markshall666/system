/*
 * database.h
 *
 *  Created on: 23-10-2016
 *      Author: marek
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include "databaseIf.h"
#include "marekObject.h"
#include <map>

class DataBase: public DataBaseIf
{
  public:
  DataBase();
  virtual ~DataBase();

  bool addMO(MarekObject mo);
  bool modifyMO(MarekObject mo);
  bool deleteMO(MarekObject mo);
  MarekObject getMO(std::string name);
  unsigned int getNextObjectId();

  private:
  std::map<unsigned int, MarekObject> moMap;
  unsigned int objectIdCounter;

};



#endif /* DATABASE_H_ */
