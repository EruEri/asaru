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

#include "../include/asaru_cp.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

afc_error_t asaru_cp(connection_t* connection, asaru_path_t* apth, args_t* args) {
    char* input = NULL;
    char* output = NULL;
    bool recursive = false;
    while (true) {
        int i = getopt(args->argc, (char* const*) args->argv, "+ri:o:");
        if (i == -1) {
            break;
        }
        switch (i) {
        case 'r':
            recursive = true;
            break;
        case 'i':
            input = optarg;
            break;
        case 'o':
            output = optarg;
            break;
        default:
            break;
        }
    }

    optind = 1;
    if (input == NULL || output == NULL) {
        fprintf(stderr, "usage : cp [-r] -i <ipath> -o <opath>\n");
        return AFC_E_SUCCESS;
    };

    return AFC_E_SUCCESS;
}