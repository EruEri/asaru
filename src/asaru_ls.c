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


#include "../include/asaru_ls.h"
#include "../include/asaru_util.h"
#include <stdio.h>
#include <stdlib.h>

afc_error_t asaru_ls(connection_t* connection, asaru_path_t* path, const char *args) {
    char* spath = NULL;
    char** dictinary  = NULL;
    afc_error_t e;
    spath = asaru_path_to_string_cat(path, args);
    printf("path = %s\n", spath);
    dictinary = connection_info_file(connection, spath, &e);
    if (e != AFC_E_SUCCESS) {
        goto clean;
    }

    print_array(dictinary);

clean:
    afc_dictionary_free(dictinary);
    free(spath);
    return e;
}