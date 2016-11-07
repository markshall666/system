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

  virtual bool addMO(unsigned, unsigned, std::string&, std::vector<std::string>&, std::vector<std::string>&) = 0;
  virtual bool modifyMO(unsigned, unsigned, std::string&, std::vector<std::string>&, std::vector<std::string>&) = 0;
  virtual bool deleteMO(unsigned, unsigned, std::string&) = 0;
  virtual bool getMO(std::string&, std::vector<std::string>&) = 0;
  virtual bool printMO(std::string&, std::vector<std::string>&) = 0;
  virtual unsigned getMaxId() = 0;
  virtual unsigned getObjectId(unsigned, std::string&) = 0;
};



#endif /* DATABASEIF_H_ */
