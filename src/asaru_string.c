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


#include "../include/asaru_string.h"
#include "../include/asaru_util.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


bool string_need_realloc(string_t* string) {
    return string->len >= string->capacity -1;
}

void string_ralloc(string_t* string) {
    size_t new_capacity = string->capacity * 2;
    char* ptr = ralloc(string->ptr, new_capacity);
    string->ptr = ptr;
    string->capacity = new_capacity;
}

string_t* string_alloc(const char* in) {
    string_t* s = alloc(sizeof(string_t));
    if (in) {
        size_t len = strlen(in);
        char* str = strclone(in);
        s->ptr = str;
        s->len = len;
        s->capacity = len + 1;
    } else {
        char* str = alloc(1);
        *str = '\0';
        s->ptr = str;
        s->len = 0;
        s->capacity = 1;
    };
    return s;
}

char* string_ptr(string_t* p) {
    return p->ptr;
}

bool string_is_blank(const string_t* s) {
    if (s->len == 0 || s->ptr == NULL) return true;
    for (size_t i = 0; i < s->len; s += 1) {
        int c = s->ptr[i];
        if (isspace(c) || isblank(c)) return true;
    }

    return false;
}

void string_free_only(string_t* s) {
    free((void*) s);
}

void string_cat(string_t* dst, const string_t* rhs) {
    size_t len = dst->capacity + rhs->len;
    if (dst->capacity - 1 < len) {
        char* ptr = ralloc(dst->ptr, len);
        dst->ptr = ptr;
    }
    strncpy(dst->ptr + dst->len, rhs->ptr, rhs->capacity);
}

void string_add_char(string_t* string, const char c) {
    if (string_need_realloc(string)) {
        string_ralloc(string);
    }
    string->ptr[string->len] = c;
    string->len += 1;
    string->ptr[string->len] = '\0';
}

int string_cmp(const string_t * lhs, const string_t* rhs) {
    return strcmp(lhs->ptr, rhs->ptr);
}

void string_free(string_t* string) {
    free(string->ptr);
    free(string);
}