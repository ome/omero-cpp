/*
 * Copyright (C) Copyright 2014 Glencoe Software, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#include <stdio.h>
#include <time.h>
#include <iostream>


namespace simple_omero
{
#ifndef _utilities_logger_included_
#define _utilities_logger_included_
    class logger {
        public:
            const std::string date_time_now()
            {
                time_t time_now;
                struct tm * time_local;
                char time_buffer[80];
                
                time(&time_now);
                time_local = localtime(&time_now);
                
                strftime(
                    time_buffer, sizeof(time_buffer),
                    "(%Y-%m-%d.%X)", time_local
                );
                return time_buffer;
            };
    };
#endif // _utilities_logger_included_
}