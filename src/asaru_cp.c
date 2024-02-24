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


#ifdef __GNUC__

#define _POSIX_C_SOURCE 2

#endif

#include <sys/stat.h>
#include "../include/asaru_cp.h"
#include "../include/asaru_stat.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


#define ASARU_BUFFER_SIZE 4096

asaru_fkind_t asaru_kind_of_path(connection_t* connection, const char* spath) {
    afc_error_t e;
    char** dictionary = connection_info_file(connection, spath, &e);
    if (e != AFC_E_SUCCESS) { return AFK_UNKNOWN; }

    asaru_fstat_t stat = asaru_stat_of_dictionary((const char**) dictionary);

    afc_dictionary_free((char **) dictionary);
    return stat.fkind;
}

int asaru_copy_file(connection_t* connection, asaru_path_t* src, asaru_path_t* dst) {
    uint64_t iphone_fd = 0;
    const char* infilename = asaru_path_to_string(src);
    const char* outfilename = asaru_path_to_string(dst);

    if (afc_file_open(connection->client, infilename, AFC_FOPEN_RDONLY, &iphone_fd) != AFC_E_SUCCESS) {
        fprintf(stderr, "Unable to open the iphone file : %s\n", infilename);
        goto error;
    }

    FILE* outfile = fopen(outfilename, "ab+");
    if (!outfile) { 
        afc_file_close(connection->client, iphone_fd);
        goto error; 
    }

    char buffer[ASARU_BUFFER_SIZE];
    unsigned int bytes_read = 0;
    do {
        if (afc_file_read(connection->client, iphone_fd, buffer, ASARU_BUFFER_SIZE, &bytes_read) != AFC_E_SUCCESS) {
            fclose(outfile);
            afc_file_close(connection->client, iphone_fd);
            goto error;
        }
        fwrite(buffer, sizeof(char), bytes_read, outfile);
    } while (bytes_read == ASARU_BUFFER_SIZE);

    afc_file_close(connection->client, iphone_fd);
    fclose(outfile);
    return 0;
error:
    perror("asaru copy");
    free((void*) infilename);
    free((void*) outfilename);
    return -1;
}

afc_error_t asaru_copy_recursive(connection_t* connection, asaru_path_t* source, asaru_path_t* destination) {
    const char* source_str = asaru_path_to_string(source);
    const char* destination_str = asaru_path_to_string(destination);
    asaru_fkind_t stats = asaru_kind_of_path(connection, source_str);
    struct stat outfile_stats;
    int status = stat(destination_str, &outfile_stats);
    if (status != 0) {
        perror(NULL);
        goto end;
    }
    switch (stats) {
        case AFK_DIRECTORY: {
            status = mkdir(destination_str, 755);
            if (status != 0 && errno != EEXIST) {
                printf("mkdir %s :", destination_str);
                perror("mkdir");
                goto end;
            }
            afc_error_t e;
            char** entries = connection_read_directory(connection, source_str, &e);
            if (entries == NULL) { goto end; }
            for (size_t i = 0; entries[i] != NULL; i+= 1) {
                const char* entry = entries[i];
                if (streq(".", entry) || streq("..", entry)) continue;
                asaru_path_push(source, entry);
                asaru_path_push(destination, entry);

                asaru_copy_recursive(connection, source, destination);

                asaru_path_pop(source);
                asaru_path_pop(destination);
            }
            goto end;
        }
        case AFK_FILE: {
            if S_ISDIR(outfile_stats.st_mode) {
                const char* filename = asaru_path_last_component_ref(source);
                asaru_path_push(destination, filename);
                asaru_copy_file(connection, source, destination);
            } else {

            }
            goto end;
        }
        case AFK_UNKNOWN: 
            goto end;
    }
end:
    free((void*) source_str);
    free((void *) destination_str);
    return AFC_E_SUCCESS;
}

afc_error_t asaru_copy(connection_t* connection, asaru_path_t* source, asaru_path_t* destination) {
    const char* source_str = asaru_path_to_string(source);
    const char* destination_str = asaru_path_to_string(destination);
    asaru_fkind_t stats = asaru_kind_of_path(connection, source_str);
    struct stat outfile_stats;
    int status = stat(destination_str, &outfile_stats);

    // ENOENT no such file or directory
    if (status != 0 && errno != ENOENT) {
        perror(NULL);
        goto end;
    }

    puts(source_str);
    switch (stats) {
        case AFK_DIRECTORY: {
            fprintf(stderr, "%s is a directory\n", source_str);
            goto end;
        }
        case AFK_FILE: {
            if S_ISREG(outfile_stats.st_mode) {
                asaru_copy_file(connection, source, destination);
            } else {
                const char* filename = asaru_path_last_component_ref(source);
                asaru_path_push(destination, filename);
                asaru_copy_file(connection, source, destination);
            }
            goto end;
        }
        case AFK_UNKNOWN: {
            fprintf(stderr, "Uknwon file kind\n");
            goto end;
        }
    }

end:
    free((void*) source_str);
    free((void *) destination_str);
    return AFC_E_SUCCESS;
}

afc_error_t asaru_cp(connection_t* connection, asaru_path_t* apth, args_t* args) {
    const char* input = NULL;
    const char* output = NULL;
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

    asaru_path_t* source = asaru_path_clone(apth);
    asaru_path_push(source, input);
    asaru_path_t* destination = asaru_path_create(false);
    asaru_path_push(destination, output);
    if (recursive) {
        asaru_copy_recursive(connection, source, destination);
    } else {
        asaru_copy(connection, source, destination);
    }

    asaru_path_free(&source);
    asaru_path_free(&destination);
    return AFC_E_SUCCESS;
}