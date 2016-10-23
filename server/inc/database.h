/*
 * database.h
 *
 *  Created on: 23-10-2016
 *      Author: marek
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include "marekObject.h"
#include <map>

class DataBase
{
  public:
  DataBase();
  virtual ~DataBase();

  bool addMO(MarekObject mo);
  bool modifyMO(MarekObject mo);
  bool deleteMO(MarekObject mo);
  MarekObject getMO(std::string name);

  private:
  std::map<unsigned int, MarekObject> moMap;

};



#endif /* DATABASE_H_ */
