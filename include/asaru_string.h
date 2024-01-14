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

#ifndef __ASARU_STRING_H__
#define __ASARU_STRING_H__


#include <stddef.h>
#include <stdbool.h>

typedef struct {
    char* ptr;
    // should be at most capacity - 1 to include the \0
    size_t len;
    size_t capacity;
} string_t;

string_t* string_alloc(const char*);
void string_add_char(string_t*, const char);
void string_cat(string_t*, const string_t*);
void string_free(string_t*);

/**
Free [string_t] but keep the underlining pointer alive (ie. pointer returns by string_ptr)
this pointer **must be freed** by free

*/
void string_free_only(string_t*);
char* string_ptr(string_t*);
int string_cmp(const string_t*, const string_t*);
bool string_is_blank(const string_t*);

#endif