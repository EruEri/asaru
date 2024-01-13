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


#include "../include/asaru_path.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/asaru_util.h"

static void asaru_path_realloc(asaru_path_t* path) {
    size_t new_capacity = path->capacity * 2;
    const char** components = ralloc(path->components, sizeof(const char*) * new_capacity);
    path->components = components;
    path->capacity = new_capacity;
}

static bool asaru_path_need_realloc(asaru_path_t* path) {
    return path->count >= path->capacity;
}

void asaru_path_clear(asaru_path_t* path) {
    for (size_t i = 0; i < path->count; i += 1) {
        free((void *) path->components[i]);
    }
    path->count = 0;
}

asaru_path_t* asaru_path_create() {
    size_t capacity = 1;
    size_t count = 0;
    const char** components = alloc(sizeof(const char*));
    asaru_path_t* path = alloc(sizeof(asaru_path_t));
    path->components = components;
    path->count = count;
    path->capacity = capacity;
    return path;
}

void asaru_path_free(asaru_path_t** rpath) {
    asaru_path_t* p = *rpath;
    for (size_t i = 0; i < p->count; i++) {
        free((void*) p->components[i]);
    }
    free(p->components);
    free(p);
    *rpath = NULL;
}

void asaru_path_pop(asaru_path_t* path) {
    if (path->count == 0) return;
    free((void*) path->components[path->count]);
    path->count -= 1;
}

void asaru_path_push(asaru_path_t * path, const char* c) {
    if (streq(c, ".")) {
        return;
    } 
    if (streq(c, ".."))  {
        asaru_path_pop(path);
        return;
    }
        
    if (asaru_path_need_realloc(path)){
        asaru_path_realloc(path);
    }
    char* copy = strclone(c); 
    path->components[path->count] = copy;
    path->count += 1;
}

char* asaru_path_concat(size_t index, const char** strs, size_t length) {
    size_t diff = length - index;
    if (diff == 0) {
        char* s = strclone("");
        return s;
    } else if (diff == 1) {
        return strclone(strs[index]);
    } else {
        char* nexts = asaru_path_concat(index + 1, strs, length);
        const char* current = strs[index];
        size_t len_nexts = strlen(nexts);
        size_t len_current = strlen(current);
        size_t len_total = len_current + 1 + len_nexts + 1;
        char* cat = alloc(len_total);
        memcpy(cat, current, len_current);
        cat[len_current] = '/';
        strcpy(cat + len_current + 1, nexts);
        free((void *) nexts);
        return cat;
    }
}

char* asaru_path_to_string_cat(asaru_path_t* path, const char* extra) {
    char* root = asaru_path_concat(0, path->components, path->count);
    size_t len_root = strlen(root);
    char* cat = alloc(len_root + 2);
    *cat = '/';
    strcpy(cat + 1, root);
    free(root);
    if (extra == NULL) return cat;
    size_t len_cat = strlen(cat); 
    size_t len_extra = strlen(extra);
    size_t len_total = len_cat + 1 + len_extra + 1;
    char* catcat = alloc(len_total);
    memcpy(catcat, cat, len_cat);
    catcat[len_cat] = '/';
    strcpy(catcat + len_cat + 1, extra);
    free(cat);
    return catcat;
}

char* asaru_path_to_string(asaru_path_t* path) {
    return asaru_path_to_string_cat(path, NULL);
}

