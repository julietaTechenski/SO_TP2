//
// Created by Usuario on 10/06/2024.
//

#ifndef SO1_WRAPPER_H
#define SO1_WRAPPER_H

#include "../../include/library.h"
#include "test_util.h"

int64_t killProcess(uint64_t argc, char * argv[]);
int64_t niceProcess(uint64_t argc, char * argv[]);
int64_t blockProcess(uint64_t argc, char * argv[]);
int64_t unblockProcess(uint64_t argc, char * argv[]);

#endif //SO1_WRAPPER_H
