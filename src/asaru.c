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


#include "../include/connection.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char ** argv) {
    connection_error_t e;
    connection_t* connection = connection_create(&e);
    if (connection == NULL) {
        fprintf(stderr, "kind  = %u\nvalue = %d\n", e.kind, e.error_value);
        exit(1);
    }

    connection_free(connection);
}