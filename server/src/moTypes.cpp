/*
 * moTypes.cpp
 *
 *  Created on: 08-11-2016
 *      Author: marek
 */

#include <string.h>
#include "moTypes.h"

static const char* KOTEK_NAME = "kotek";
static const char* OWCA_NAME = "owca";
static const char* KROLIK_NAME = "krolik";

typedef struct
{
  unsigned type;
  const char* name;
} moTypeName;

static const moTypeName table[] =
{
  {
    KOTEK_TYPE,
    KOTEK_NAME
  },
  {
    OWCA_TYPE,
    OWCA_NAME
  },
  {
    KROLIK_TYPE,
    KROLIK_NAME
  }
};

int getMoTypeByName(const char* name)
{
  for (int i = 0; i < NUMBER_OF_MO; ++i)
  {
    if (strcmp(table[i].name, name) == 0)
    {
      return table[i].type;
    }
  }
  return -1;
}
