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

#include "../include/asaru_util.h"
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


args_t* parse_string(char * input) {
    size_t true_length = strlen(input);
    args_t* args = alloc(sizeof(args_t));
    string_t** argv = alloc(0);
    string_t* current = string_alloc("");
    int argc = 0;
    size_t index = 0;
    bool escaped = false;
    bool quoted = false;
    bool seen_char = false;

    while (index < true_length) {
        char c = input[index];
        if (escaped) {
            string_add_char(current, c);
            escaped = false;
        } else if (c == '\\') {
            escaped = true;
        } else if (quoted) {
            string_add_char(current, c);
        } else if (c == '\"') {
            quoted = ! quoted;
        } else if (isspace(c)) {
            if (seen_char) {
                argv = ralloc(argv, sizeof(char*) * (argc + 1));
                argv[argc] = current;
                argc += 1;
                seen_char = false;
                current = string_alloc("");
            }
            index += 1;
            continue;
        } else {
            string_add_char(current, c);
        }
        index += 1;
        seen_char = true;
    }

    argv = ralloc(argv, sizeof(char*) * (argc + 1));
    argv[argc] = current;
    argc += 1;

    args->argc = argc;
    args->argv = argv;

    return args;
}

void args_free(args_t** rargs) {
    args_t* args = *rargs;
    for (int i = 0; i < args->argc; i += 1) {
        string_free(args->argv[i]);
    }
    free(args->argv);
    free(args);
    *rargs = NULL;
}


void* alloc(size_t size) {
    void* c = malloc(size);
    if (c == NULL) {
        perror("malloc");
        exit(2);
    }

    return c;
}

void* ralloc(void* ptr, size_t size) {
    void* new = realloc(ptr, size);
    if (new == NULL) {
        perror("realloc");
        exit(2);
    }

    return new;
}

bool streq(const char * lhs, const char * rhs) {
    return strcmp(lhs, rhs) == 0;
}

char* strclone(const char* src) {
    size_t length = strlen(src);
    char* cp = alloc(length + 1);
    strncpy(cp, src, length + 1);
    return cp;
}

void print_array(char** array) {
    int i = 0;
    char* folder;
    while( (folder = array[i]) ) printf("%02d : %s\n", ++i, folder);
}