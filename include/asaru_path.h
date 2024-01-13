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


#ifndef __ASARU_PATH_H__
#define __ASARU_PATH_H__

#include <stddef.h>

// asaru_path_t is the owner of each string in components
typedef struct {
    const char** components;
    size_t count;
    size_t capacity;
} asaru_path_t;

asaru_path_t* asaru_path_create();

void asaru_path_clear(asaru_path_t*);

void asaru_path_free(asaru_path_t**);

void asaru_path_push(asaru_path_t*, const char*);

void asaru_path_pop(asaru_path_t*);

char* asaru_path_to_string(asaru_path_t*);

char* asaru_path_to_string_cat(asaru_path_t*, const char*);


#endif