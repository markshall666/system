/*
 * marekObject.cpp
 *
 *  Created on: 23-10-2016
 *      Author: marek
 */

#include "marekObject.h"

MarekObject::MarekObject(std::string nameIn, unsigned int objectIdIn)
{
  name = nameIn;
  objectId = objectIdIn;
}


MarekObject::~MarekObject()
{}

std::string MarekObject::getMoName()
{
  return name;
}

unsigned int MarekObject::getObjectId()
{
  return objectId;
}
