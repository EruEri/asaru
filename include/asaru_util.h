////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                            //
// This file is part of asaru: An IPhone file explorer                                        //
// Copyright (C) 2024 Yves Ndiaye                                                             //
//                                                                                            //
// asaru is free software: you can redistribute it and/or modify it under the terms           //
// of the GNU General Public License as published by the Free Software Foundation,            //
// either version 3 of the License, or (at your option) any later version.                    //
//                                                                                            //
// asaru is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;         //
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR           //
// PURPOSE.  See the GNU General Public License for more details.                             //
// You should have received a copy of the GNU General Public License along with asaru.        //
// If not, see <http://www.gnu.org/licenses/>.                                                //
//                                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __ASARU_UTIL_H__
#define __ASARU_UTIL_H__

#include "asaru_string.h"
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    int argc;
    string_t** argv;
} args_t;

args_t* parse_string(char*);

void args_free(args_t**);


/**
Same as malloc but exit(2) if the pointer returned by malloc is NULL
*/
void* alloc(size_t);

/**
Same as realloc but exit(2) if the pointer returned by realloc is NULL
*/
void* ralloc(void*, size_t);

bool streq(const char*, const char*);

bool is_absolute_path(const char*);

char* strclone(const char*);

void print_array(char**);


#endif