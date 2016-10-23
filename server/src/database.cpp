/*
 * database.cpp
 *
 *  Created on: 23-10-2016
 *      Author: marek
 */

#include "database.h"

DataBase::DataBase()
{}

DataBase::~DataBase()
{}

bool DataBase::addMO(MarekObject mo)
{
  moMap.insert(std::pair<unsigned int, MarekObject>(mo.getObjectId(), mo));
  return true;
}

bool DataBase::modifyMO(MarekObject mo)
{
  std::map<unsigned int, MarekObject>::iterator it = moMap.find(mo.getObjectId());
  if (it != moMap.end())
  {
    moMap.erase(it);
    moMap.insert(std::pair<unsigned int, MarekObject>(mo.getObjectId(), mo));
    return true;
  }
  else
  {
    return false;
  }
}

bool DataBase::deleteMO(MarekObject mo)
{
  std::map<unsigned int, MarekObject>::iterator it = moMap.find(mo.getObjectId());
  if (it != moMap.end())
  {
    moMap.erase(it);
    return true;
  }
  else
  {
    return false;
  }
}

MarekObject DataBase::getMO(std::string name)
{
  for (std::map<unsigned int, MarekObject>::iterator it = moMap.begin(); it != moMap.end(); ++it)
  {
    if (it->second.getMoName() == name)
    {
      return it->second;
    }
  }
  return MarekObject("", 0);
}
