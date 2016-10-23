/*
 * database.cpp
 *
 *  Created on: 23-10-2016
 *      Author: marek
 */

#include "database.h"
#include "trace.h"

DataBase::DataBase()
:objectIdCounter(0)
{}

DataBase::~DataBase()
{}

bool DataBase::addMO(MarekObject mo)
{
  moMap.insert(std::pair<unsigned int, MarekObject>(mo.getObjectId(), mo));
  objectIdCounter = mo.getObjectId() + 1;
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
  TRACE_DEBUG("Enter DataBase::getMO with %s", name.c_str());
  for (std::map<unsigned int, MarekObject>::iterator it = moMap.begin(); it != moMap.end(); ++it)
  {
    TRACE_DEBUG("iterating %s", it->second.getMoName().c_str());
    if (it->second.getMoName() == name)
    {
      return it->second;
    }
  }
  return MarekObject("", 0);
}

unsigned int DataBase::getNextObjectId()
{
  return objectIdCounter;
}
