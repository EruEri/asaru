////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                            //
// This file is part of asaru: An IPhone file explorer                                        //
// Copyright (C) 2024 Yves Ndiaye                                                             //
//                                                                                            //
// asaru is free software: you can redistribute it and/or modify it under the terms           //
// of the GNU General Public License as published by the Free Software Foundation,            //
// either version 3 of the License, or (at your option) any later version.                    //
//                                                                                            //
// iexplore is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;      //
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR           //
// PURPOSE.  See the GNU General Public License for more details.                             //
// You should have received a copy of the GNU General Public License along with asaru.        //
// If not, see <http://www.gnu.org/licenses/>.                                                //
//                                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////


#include "../include/asaru_path.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
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