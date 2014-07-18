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


#include "SimpleOMERO_Headers.h"
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <fstream>
#include <unistd.h>
#include "logger.h"


namespace simple_omero {
#ifndef _simpleomero_connector_included_
#define _simpleomero_connector_included_ 
    /// Simple connection to OMERO server
    class connector {
        public:
            /// Constructor.
            connector()
            {
                this->log = new logger();
            }
            /// \brief Constructor with connection. No need to call connect().
            /*!
            * \param host Server Address.
            * \param port Port Number.
            * \param user User Name.
            * \param pass User Password.
            * \return  0 sucess; -1 Failed.
            */
            
            connector(
                const std::string &host , const std::string &port,
                const std::string &user, const std::string &pass)
            {
                this->log = new logger();
                this->connect(host, port, user, pass);
            }
            /// \brief Connects to omero server
            /*!
            * \param host Server Address.
            * \param port Port Number.
            * \param user User Name.
            * \param pass User Password.
            * \return  0 sucess; -1 Failed.
            */
            int connect(
                const std::string &host, const std::string &port,
                const std::string &user, const std::string &pass
            );
            /// Omero client getter.
            omero::client_ptr get_client() {return client;};
            /// Omero session getter.
            omero::api::ServiceFactoryPrx get_session() {return session;};
            /// Omero admin getter.
            omero::api::IAdminPrx get_admin() {return admin;};
            /// Prints session details to console.
            void printSessionDetails();
            /// Prints to console list of user's datasets and images. 
            void list_images_in_datasets();
            /// Lists OMERO pixel types.
            void list_pixel_types();
        private:
            /// Omero client instance
            omero::client_ptr client;
            /// Omero session instance
            omero::api::ServiceFactoryPrx session;
            /// Omero admin instace.
            omero::api::IAdminPrx admin;
            /// Omero client setter.
            int set_client();
            /// Omero admin setter.
            int set_admin();
            /// Omero session setter.
            int set_session();
            /// Creates Ice connection data holder from parsed arguments.
            /*!
             * \return Ice connection inistialization data.
             */
            Ice::InitializationData initialize_connection_data();
            std::string host;
            std::string port;
            std::string user;
            std::string pass;
            Ice::InitializationData connection_data;
            logger *log;
    };
#endif //_simpleomero_connector_included_

#ifndef _simpleomero_image_included_
#define _simpleomero_image_included_ 
    /// Simple image access
    class image {
        public:
            /// \brief   SimpleOMERO image contructor for Retriving image from
            ///          OMERO.
            /// \details Method retrives OMERO image using ContainerService
            ///          and sets up all data members.
            ///
            /*!
             * \param session pointer to curent session (Service Factory).
             * \param image_id image Id obtained from Omero client.
             */
            image(
                  const omero::api::ServiceFactoryPrx &session,
                  const int &image_id
            );
            /// \brief   SimpleOMERO image contructor for Creating image in
            ///          OMERO.
            /// \details Method creates NEW image in OMERO and then
            ///          populates SimpleOMERO object data members.
            /*!
             * \param session pointer to curent session (Service Factory).
             * \param dataset_id dataset Id where image will be created.
             * \param pixel_type new image's pixel type.
             * \param width image width.
             * \param height image height.
             * \param depth number of z planes.
             * \param numeber_of_channels number of channels.
             * \param number_of_timepoints number of time points.
             * \param name Image name.
             * \param description Image description.
             * \param pixel_size_x Physical pixel size in X dimension.
             * \param pixel_size_y Physical pixel size in Y dimension.
             * \param pixel_size_z Physical voxel size in Z dimension.
             */
            image(
                const omero::api::ServiceFactoryPrx &session,
                const int &dataset_id,
                const omero::model::PixelsTypePtr &pixel_type,
                const int &width, const int &height, const int &depth,
                const int &number_of_channels, const int &number_of_timepoints,
                const std::string &name, const std::string &description,
                const double &pixel_size_x, const double &pixel_size_y,
                const double &pixel_size_z
            );
            /// \brief Creates OMERO RawPixelStore for Reading/Writing pixels.
            /*! 
             * \param session pointer to curent session (Service Factory).
             */
            void open_pixel_store(
                const omero::api::ServiceFactoryPrx &session
            );
            /// \brief Closes and saves OMERO RawPixelStore for Reading/Writing
            /// pixels.
            /*!
             */
            void close_pixel_store();
            /// \brief Prints out OMERO Image details.
            /*!
             */
            void print_details();
            ///
            /// Do not use it - toooooo slow. Retrives pointer to memory
            /// containing image pixels.
            /** The method uses opened connection (session) to OMERO server and
                retrived Image to request raw pixel data.
            */
            /*!
            * \param session pointer to curent session (Service Factory).
            * \param image pointer to omero.image object
            * \param plane z plane of interest.
            * \param channel channel of interest.
            * \param timepoint time point of interest.
            * \param image_name image name (for logging purposes).
            * \return pointer to memory with raw pixel data.
            */
            unsigned char *get_raw_pixels(
                const omero::api::ServiceFactoryPrx &session,
                const omero::model::ImageIPtr &image, const int &plane,
                const int &channel, const int &time_point,
                const std::string &image_name
            );
            /// Retrives Plane from previously opened image->pixel_store.
            /** Before using this method call image::open_pixel_store(session)
             *  once done retriving data call image::clse_pixel_store().
             */
            /*!
             * \param image_cast pre-allocated unsigned char buffer
             *        to store raw pixel bytes.
             * \param plane z plane to retrive.
             * \param channel channel of interest.
             * \param timepoint time point of interest.
             * \param bpp number of bytes per pixel.
             */
            void get_raw_pixels(
                unsigned char *image_cast, const int &plane,
                const int &channel, const int &time_point, const int &bpp
            );
            /// Retrives Hypercube from previously opened image->pixel_store.
            /** Before using this method call image::open_pixel_store(session)
             *  once done retriving data call image::clse_pixel_store().
             */
            /*!
             * \param image_cast pre-allocated unsigned char buffer to store
             *        raw pixel bytes.
             * \param start_x first pixel.
             * \param end_x last pixel.
             * \param start_y first row.
             * \param end_y last row.
             * \param step_y step in y dimension.
             * \param start_z first plane.
             * \param end_z last plane.
             * \param bpp bytes per pixel.
             */
            void get_raw_pixels_hyper_cube(
                unsigned char *image_cast, const int &start_x,
                const int &end_x, const int &start_y, const int &end_y,
                const int &step_y, const int &start_z, const int &end_z,
                const int &bpp
            );
            /// Retrives Row from previously opened image->pixel_store.
            /** Before using this method call image::open_pixel_store(session)
             *  once done retriving data call image::clse_pixel_store().
             */
            /*!
             * \param image_cast pre-allocated unsigned char buffer
             *        to store raw pixel bytes.
             * \param row row to retrive.
             * \param plane z plane of interest.
             * \param channel channel of interest.
             * \param timepoint time point of interest.
             * \param bpp number of bytes per pixel.
             */
            void get_raw_pixels_row(
                unsigned char *image_cast, const int &row, const int &plane,
                const int &channel, const int &time_point, const int &bpp
            );
            /// Write pixels to OMERO image.
            /** Converts unsigned char buffer to Ice::Byte vector and writes
             *  it to specified time point, channel, plane.
             */
            /*!
             * \param session session pointer to curent session
             *        (Service Factory).
             * \param buffer buffer containing pixels to write.
             * \param channel channel to write to.
             * \param timepoint timepoint to write to.
             * \param plane plane to write to.
             */
            void write_plane(
                unsigned char *buffer, int bpp,
                const int &timepoint, const int &channel, const int &plane
            );
            /// Pad integer to given number of digits.
            /** */
            /*!
             * \param padding Number of digits.
             * \param num Numberto pad.
             * \return Padded number.
             */
            static std::string zero_pad_number(
                const int &padding, const int &num
            );
            /// OMERO image pixel type.
            omero::model::PixelsTypePtr pixel_type;
            /// OMERO image pointer.
            omero::model::ImagePtr Pointer;
            /// OMERO image number of channels.
            int number_of_channels;
            /// OMERO image number of time points.
            int number_of_timepoints;
            /// Physical pixel size in X dimension.
            double pixel_size_x;
            /// Physical pixel size in Y dimension.
            double pixel_size_y;
            /// Physical voxel size in Z dimension.
            double pixel_size_z;
            /// OMERO image width.
            double size_x;
            /// OMERO image height.
            double size_y;
            /// OMERO image depth (number of z planes).
            double size_z;
            /// OMERO image ID
            int id;
            /// Image name
            std::string name;
            /// Image description
            std::string description;
            /// OMERO Raw Pixel Store for retrival of image data.
            omero::api::RawPixelsStorePrx pixel_store;
    };
#endif //_simpleomero_image_included
};