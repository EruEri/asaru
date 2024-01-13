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


#include "../include/asaru_stat.h"
#include "../include/asaru_util.h"
#include <stdio.h>
#include <stdlib.h>


afc_error_t asaru_stat(connection_t* connection, asaru_path_t* path, args_t* args) {
    char* spath = NULL;
    char** dictionary  = NULL;
    afc_error_t e;
    const char* s = args->argc == 2 ? args->argv[1]->ptr : NULL;
    spath = asaru_path_to_string_cat(path, s);
    printf("path = %s\n", spath);
    dictionary = connection_info_file(connection, spath, &e);
    if (e != AFC_E_SUCCESS) {
        goto clean;
    }

    asaru_fstat_t stat = asaru_stat_of_dictionary((const char**) dictionary);
    asaru_fstat_print(stat);

clean:
    afc_dictionary_free(dictionary);
    free(spath);
    return e;
}

asaru_fkind_t asaru_file_kind_of_string(const char* s) {
    if (streq("S_IFDIR", s)) return AFK_DIRECTORY;
    else return AFK_UNKNOWN;
}

asaru_fstat_t asaru_stat_of_dictionary(const char** dictionary) {
    int i = 0;
    const char* entry;
    asaru_fstat_t stats = {};
    while ( (entry = dictionary[i]) ) {
        switch (i) {
        case 1: {
            stats.st_size = atol(entry);
            break;
        }
        case 3: {
            stats.st_blocks = atol(entry);
            break;
        }
        case 5: {
            stats.st_nlink = atol(entry);
            break;
        }
        case 7: {
            stats.fkind = asaru_file_kind_of_string(entry);
            break;
        }
        case 9: {
            stats.st_mtimes = atol(entry);
            break;
        }
        case 11: {
            stats.st_birthtime = atol(entry);
            break;
        }
        }
        i+=1;
    }

    return stats;
}


static const char* asaru_kind_to_stringl(asaru_fkind_t k) {
    switch (k) {
    case AFK_FILE:
        return "Regular file";
    case AFK_DIRECTORY:
        return "Directory";
    case AFK_UNKNOWN:
        return "Unknown";
    }
    return "Not matched";
}


void asaru_fstat_print(asaru_fstat_t stat) {
    printf("kind file    = %s\n", asaru_kind_to_stringl(stat.fkind));
    printf("st_size      = %lu\n", (unsigned long) stat.st_size);
    printf("st_blocks    = %lu\n", (unsigned long) stat.st_blocks);
    printf("st_nlink     = %lu\n", (unsigned long) stat.st_nlink);
    printf("st_mtime     = %lu\n", (unsigned long) stat.st_mtimes);
    printf("st_birthtime = %lu\n", (unsigned long) stat.st_birthtime);
}