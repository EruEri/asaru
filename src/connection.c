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
#include "../include/asaru_util.h"

#include <stdio.h>
#include <stdlib.h>

static idevice_t create_device(connection_error_t* e) {
    idevice_t device = NULL;
    e->kind = CEK_DEVICE;
    e->error_value = idevice_new_with_options(&device, NULL, IDEVICE_LOOKUP_USBMUX);
    return device;
}


static lockdownd_client_t create_lockdown(idevice_t device, connection_error_t* e) {
    lockdownd_client_t lockdown_client = NULL;
    e->kind = CEK_LOCKDOWN;
    e->error_value = lockdownd_client_new_with_handshake(device, &lockdown_client, AFC_SERVICE_NAME);
    return lockdown_client;
}

static lockdownd_service_descriptor_t create_lockdown_service(lockdownd_client_t lockdown_client, connection_error_t* e) {
    lockdownd_service_descriptor_t service = NULL;
    e->kind = CEK_LOCKDOWN_SERVICE;
    e->error_value = lockdownd_start_service(lockdown_client, AFC_SERVICE_NAME, &service);
    return service;
}

static afc_client_t create_client(idevice_t device, lockdownd_service_descriptor_t service, connection_error_t* e) {
    afc_client_t client;
    e->kind = CEK_CLIENT;
    e->error_value = afc_client_new(device, service, &client);
    return client;
}
/**
Create a connction usb with an iphone,
Returns a pointer to the connection if succeed or NULL if an error happenned 
set [connection_error_t]
*/
connection_t* connection_create(connection_error_t* e) {
    if (e == NULL) {
        fprintf(stderr, "connection error is null\n");
        return NULL;
    }
    connection_t* connection = alloc(sizeof(connection_t));
    idevice_t device = NULL;
    lockdownd_client_t lockdown_client  = NULL;
    lockdownd_service_descriptor_t service_descriptor = NULL;
    afc_client_t client = NULL;
    device = create_device(e);
    if (e->error_value != IDEVICE_E_SUCCESS) { goto error; }
    lockdown_client = create_lockdown(device, e);
    if (e->error_value != LOCKDOWN_E_SUCCESS) { goto error; }
    service_descriptor = create_lockdown_service(lockdown_client, e);
    if (e->error_value != LOCKDOWN_E_SUCCESS) { goto error; }
    client = create_client(device, service_descriptor, e);
    if (e->error_value != AFC_E_SUCCESS) { goto error; }
    connection->device = device;
    connection->client = client;
    connection->lockdown_client = lockdown_client;
    connection->service_descriptor = service_descriptor;
    return connection;
error:
    idevice_free(device);
    lockdownd_client_free(lockdown_client);
    lockdownd_service_descriptor_free(service_descriptor);
    afc_client_free(client);
    free(connection);
    return NULL;
}


void connection_free(connection_t** rconnection) {
    connection_t* connection = *rconnection;
    idevice_free(connection->device);
    lockdownd_client_free(connection->lockdown_client);
    lockdownd_service_descriptor_free(connection->service_descriptor);
    afc_client_free(connection->client);
    free(connection);
    *rconnection = NULL;
}

char** connection_info_file(connection_t* connection, const char* path, afc_error_t* e) {
    char** info = NULL;
    *e = afc_get_file_info(connection->client, path, &info);
    return info;
}