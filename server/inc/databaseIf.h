/*
 * dataBaseIf.h
 *
 *  Created on: 23-10-2016
 *      Author: marek
 */

#ifndef DATABASEIF_H_
#define DATABASEIF_H_

#include <string>
#include <vector>

/*
 * DataBase abstract interface to make possible stub.
 */
class DataBaseIf
{
  public:
  virtual ~DataBaseIf()
  {}

  virtual bool addMO(std::vector<std::string>&, int) = 0;
  virtual bool modifyMO(std::vector<std::string>&) = 0;
  virtual bool deleteMO(std::vector<std::string>&) = 0;
  virtual std::vector<std::string> getMO(std::vector<std::string>&) = 0;
  virtual std::vector<std::string> printMO(std::vector<std::string>&) = 0;
  virtual unsigned getMaxId() = 0;
};



#endif /* DATABASEIF_H_ */
