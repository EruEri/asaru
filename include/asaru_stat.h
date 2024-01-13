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


#ifndef __ASARU_STAT_H__
#define __ASARU_STAT_H__

#include "asaru_path.h"
#include "asaru_util.h"
#include "connection.h"
#include <stddef.h>

typedef enum {
    AFK_FILE,
    AFK_DIRECTORY,
    AFK_UNKNOWN
} asaru_fkind_t;

typedef struct {
    asaru_fkind_t fkind;
    size_t st_size;
    size_t st_blocks;
    size_t st_nlink;
    size_t st_mtimes;
    size_t st_birthtimes;
} asaru_fstat_t;

afc_error_t asaru_stat(connection_t*, asaru_path_t*, args_t*);


asaru_fkind_t asaru_file_kind_of_string(const char*);

asaru_fstat_t asaru_stat_of_dictionary(const char**);

void asaru_fstat_print(asaru_fstat_t);

#endif