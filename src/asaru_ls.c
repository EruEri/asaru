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


#include "../include/asaru_ls.h"
#include "../include/asaru_util.h"
#include <stdio.h>
#include <stdlib.h>

afc_error_t asaru_ls(connection_t* connection, asaru_path_t* path, args_t* args) {
    char* spath = NULL;
    char** dictionary  = NULL;
    afc_error_t e;
    const char* s = args->argc == 2 ? args->argv[1] : NULL;
    spath = is_absolute_path(s) ? strclone(s) : asaru_path_to_string_cat(path, s);
    printf("path = %s\n", spath);
    dictionary = connection_read_directory(connection, spath, &e);
    if (e != AFC_E_SUCCESS) {
       goto clean;
   }

   print_array(dictionary);

clean:
    afc_dictionary_free(dictionary);
    free(spath);
    return e;
}