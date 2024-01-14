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


#include "../include/connection.h"
#include "../include/asaru_path.h"
#include "../include/asaru_util.h"
#include "../include/asaru_cmd.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef afc_error_t(*command_t)(connection_t*, asaru_path_t*, args_t*);

command_t command_find(args_t* args) {
    const char* cmd = args->argv[0];
    if (streq(cmd, "cd")) return asaru_cd;
    if (streq(cmd, "cp")) return asaru_cp;
    if (streq(cmd, "ls")) return asaru_ls;
    if (streq(cmd, "pwd")) return asaru_pwd;
    if (streq(cmd, "stat")) return asaru_stat;
    if (streq(cmd, "exit")) return NULL;
    return asaru_unknown;
}

int repl(connection_t* connection) {
    asaru_path_t* path = asaru_path_create();
    char buffer[256];
    while (true) {
        printf(">>> ");
        // We say no to gets
        char* s = fgets(buffer, sizeof(buffer), stdin);
        args_t* args = parse_string(s);
        if (args->argc == 0) {
            args_free(&args);
            continue;
        }
        command_t command = command_find(args);
        if (command == NULL) {
            args_free(&args);
            break;
        }
        afc_error_t e = command(connection, path, args);
        args_free(&args);
    };

    asaru_path_free(&path);
    return 0;
}

int main(int argc, const char ** argv) {
    connection_error_t e;
    connection_t* connection = connection_create(&e);
    if (connection == NULL) {
        fprintf(stderr, "kind  = %u\nvalue = %d\n", e.kind, e.error_value);
        exit(1);
    }

    repl(connection);

    connection_free(&connection);
}