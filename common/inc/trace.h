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

#ifdef __cplusplus
extern int trace_enable;
#else
int trace_enable;
#endif


#define TRACE_INIT(argc, argv) {                  \
  if (argc > 1 && strcmp(argv[1], "debug") == 0){ \
    trace_enable = 1;                             \
  }                                               \
}                                                 \

#define TRACE_DEBUG(...){           \
  if (trace_enable){                \
    printf(__VA_ARGS__);            \
    printf("\n");                   \
  }                                 \
}                                   \

#define TRACE_ERROR(...){           \
  fprintf(stderr, __VA_ARGS__);     \
  fprintf(stderr, "\n");            \
}

#define TRACE_PERROR(...){          \
  perror(__VA_ARGS__);              \
}

#define TRACE_ENTER {               \
  if (trace_enable){                \
    printf("Enter ");               \
    printf(__func__);               \
    printf("\n");                   \
  }                                 \
}                                   \



#define TRACE_MSG(msg){                                                 \
  if (trace_enable){                                                    \
    const unsigned char* const bytes = (const unsigned char* const)msg; \
    uint32_t* msgPtr = (uint32_t*)msg;                                  \
    printf("[");                                                        \
    for(size_t i = 0; i < *(msgPtr-2); i++)                             \
    {                                                                   \
      printf("%02x ", bytes[i]);                                        \
    }                                                                   \
    printf("]\n");                                                      \
  }                                                                     \
}                                                                       \

#endif /* TRACE_H_ */
