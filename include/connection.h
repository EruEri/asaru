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


#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include "libimobiledevice/afc.h"

typedef enum {
    CEK_DEVICE,
    CEK_LOCKDOWN,
    CEK_LOCKDOWN_SERVICE,
    CEK_CLIENT
} connection_error_kind_t;

typedef struct {
    connection_error_kind_t kind;
    int error_value;
} connection_error_t;

typedef struct{
    idevice_t device;
    lockdownd_client_t lockdown_client;
    lockdownd_service_descriptor_t service_descriptor;
    afc_client_t client;
} connection_t;

connection_t* connection_create(connection_error_t* e);
void connection_free(connection_t**);

/**
The caller is responsible to free with [afc_dictionary_free]
*/
char** connection_info_file(connection_t*, const char*, afc_error_t* e);

#endif