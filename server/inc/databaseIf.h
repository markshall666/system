/*
 * dataBaseIf.h
 *
 *  Created on: 23-10-2016
 *      Author: marek
 */

#ifndef DATABASEIF_H_
#define DATABASEIF_H_

#include <string>

/*
 * DataBase abstract interface to make possible stub.
 */
class DataBaseIf
{
  public:
  virtual ~DataBaseIf()
  {}

  virtual bool addMO(std::string, void*) = 0;
  virtual bool modifyMO(std::string, void*) = 0;
  virtual bool deleteMO(std::string) = 0;
  virtual void* getMO(std::string) = 0;
};



#endif /* DATABASEIF_H_ */
