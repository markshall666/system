/*
 * dataBaseIf.h
 *
 *  Created on: 23-10-2016
 *      Author: marek
 */

#ifndef DATABASEIF_H_
#define DATABASEIF_H_

#include <string>
#include "marekObject.h"

/*
 * DataBase abstract interface to make possible stub.
 */
class DataBaseIf
{
  public:
  virtual ~DataBaseIf()
  {}

  virtual bool addMO(MarekObject) = 0;
  virtual bool modifyMO(MarekObject) = 0;
  virtual bool deleteMO(MarekObject) = 0;
  virtual MarekObject getMO(std::string) = 0;
  virtual unsigned int getNextObjectId() = 0;
};



#endif /* DATABASEIF_H_ */
