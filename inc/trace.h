/*
 * trace.h
 *
 *  Created on: 24-09-2016
 *      Author: marek
 */

#ifndef TRACE_H_
#define TRACE_H_

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

int trace_enable;

#define TRACE_INIT(argc, argv) {                  \
  if (argc > 1 && strcmp(argv[1], "DEBUG") == 0){ \
	  trace_enable = 1;                           \
  }                                               \
}                                                 \

#define TRACE_DEBUG(...){           \
  if (trace_enable){                \
	  printf(__VA_ARGS__);          \
	  printf("\n");                 \
  }                                 \
}                                   \

#endif /* TRACE_H_ */
