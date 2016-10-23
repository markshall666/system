/*
 * marekObject.h
 *
 *  Created on: 23-10-2016
 *      Author: marek
 */

#ifndef MAREKOBJECT_H_
#define MAREKOBJECT_H_

#include <string>

class MarekObject
{
  public:
  MarekObject(std::string nameIn, unsigned int objectIdIn);
  virtual ~MarekObject();

  std::string getMoName();
  unsigned int getObjectId();

  private:
  unsigned int objectId;
  std::string name;
};


#endif /* MAREKOBJECT_H_ */
