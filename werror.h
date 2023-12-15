#ifndef _WERR_H_
#define _WERR_H_

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    SUCCESS = 0,
    FAILURE = 1,
    ALLOC_ERROR,
    NULL_REFERENCE,
    DATA_CORRUPTED,
    FILE_NOT_FOUND,
    INVALID_ARGUMENTS,
    PERMISSION_DENIED,
    INDEX_OUT_OF_RANGE,
    UNDEFINED_BEHAVIOUR,
} exit_codes;

#define warning(fmt, ...) fprintf(stderr, "WARNING: %s:%s:%d: " fmt, __FILE__, __func__, __LINE__ , ##__VA_ARGS__ )
#define error(fmt, ...) fprintf(stderr, "ERROR: %s:%s:%d: " fmt, __FILE__, __func__, __LINE__ , ##__VA_ARGS__)
#define raise(code, fmt, ...) do {error(fmt, ##__VA_ARGS__); exit(code);} while(0);

#endif