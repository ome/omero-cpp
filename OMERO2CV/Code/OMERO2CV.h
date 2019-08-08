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


#include <time.h>
#include <vector>
#include <SimpleOMERO.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


#define o2cv_bit             0
#define o2cv_int8            1
#define o2cv_uint8           2
#define o2cv_int16           3
#define o2cv_int32           4
#define o2cv_uint16          5
#define o2cv_uint32          6
#define o2cv_flt             7
#define o2cv_dbl             8
#define o2cv_complex         9
#define o2cv_double_complex  10


namespace omero2cv
{
#ifndef _omero2cv_stack_included_
#define _omero2cv_stack_included_
    /// \brief OMERO to OpenCV Image type converter.
    class type_converter
    {
    public:
        /// Constructor
        type_converter(const omero::api::ServiceFactoryPrx &session);
        /// Convert OMERO pixel type to OpenCV pixel type.
        int omero_2_cv(omero::model::PixelsTypePtr omero_type);
        /// COnvert OpenCV pixel type to OMERO pixel type.
        int cv_2_omero(int cv_type, omero::model::PixelsTypePtr *omero_type);
        /// Get bytes per pixel for OpenCV pixel type.
        int get_bpp(int cv_type);
        /// Get bytes per pixel for OMERO pixel type.
        int get_bpp(omero::model::PixelsTypePtr omero_type);
    private:
        /// Vector holding OMERO pixel types fro conversion.
        /// Populated by the constructor.
        std::vector<omero::model::PixelsTypePtr> omero_pixels;
        
    };
#endif //_omero2cv_stack_included_


#ifndef _omero2cv_plane_store_included_
#define _omero2cv_plane_store_included_
    /// \brief Basic building block for data storage in OpenCV format.
    /// Stores 3D stack as cv::Mat vector.
    class plane_store : public std::vector<cv::Mat> {
    public:
        /// Returns a reference to the plane at position z.
        /*!
         * \param plane plane to access.
         */
        reference z(int plane) {return this->at(plane);};
        ///
        plane_store(){};
        ///
        plane_store(
            double pixel_size_x, double pixel_size_y, double pixel_size_z
        );
        ///
        void allocate_zero_mat(
            const int &width, const int &height,
            const int &depth, const int &cv_type
        );
        /// Physical pixel size in X dimension.
        double pixel_size_x;
        /// Physical pixel size in Y dimension.
        double pixel_size_y;
        /// Physical pixel size in Z dimension.
        double pixel_size_z;
        /// z / x scaling factor.
        double z_scaling;
        /// Maximum pixel value
        double max;
        /// Minimum pixel value
        double min;
    };
#endif //_omero2cv_plane_store_included_


#ifndef _omerocv_channel_store_included_
#define _omerocv_channel_store_included_
    class channel_store : public std::vector<plane_store*> {
    public:
        /// Returns a reference to the channel at position c.
        /*!
         * \param channel channel to access.
         */
        reference c(int channel) {return this->at(channel);};
        /// Image Width.
        int size_x;
        /// Image Height.
        int size_y;
        /// Number of planes
        int size_z;
        /// Physical pixel size in X dimension.
        double pixel_size_x;
        /// Physical pixel size in Y dimension.
        double pixel_size_y;
        /// Physical pixel size in Z dimension.
        double pixel_size_z;
        /// z / x scaling factor.
        double z_scaling;
    };
#endif //_omerocv_channel_store_included_


#ifndef _omerocv_image_store_included_
#define _omerocv_image_store_included_
    class image_store : public std::vector<channel_store*> {
    public:
        /// Returns a reference to the plane at position t.
        /*!
         * \param timepoint timepoint to access.
        */
        reference t(int timepoint) {return this->at(timepoint);};
        /// Number of channels.
        int number_of_channels;
        /// Image Width.
        int size_x;
        /// Image Height.
        int size_y;
        /// Number of planes
        int size_z;
        /// Physical pixel size in X dimension.
        double pixel_size_x;
        /// Physical pixel size in Y dimension.
        double pixel_size_y;
        /// Physical pixel size in Z dimension.
        double pixel_size_z;
        /// z / x scaling factor.
        double z_scaling;
    };
#endif //_omerocv_image_store_included_

    
#ifndef _omero2cv_image_included_
#define _omero2cv_image_included_
    /// \brief Read/Write images to and from OMERO in OpenCV format. 
    class image
    {
        ///
        simple_omero::image *omero_image;
        ///
        type_converter *converter;
    public:
        /// Destructor
        ~image();
        /// Constructor for reading.
        /*!
         * \param sessuib pointer to curent session (Service Factory).
         * \param image_id Image ID to retrieve.
         */
        image(
            const omero::api::ServiceFactoryPrx &session,
            const int &image_id
        );
        /// Constructor for reading.
        /*!
         * \param session pointer to curent session (Service Factory).
         * \param image_id Image ID to retrieve.
         * \param timepoint_list List of timepoints to read.
         * \param channel_list List of channels to read.
         * \param plane_list List of planes to read.
         */
        image(
            const omero::api::ServiceFactoryPrx &session,
            const int &image_id,
            std::vector<int> timepoint_list,
            std::vector<int> channel_list,
            std::vector<int> plane_list
        );
        /// Constructor for writing.
        /*!
         * \param session pointer to curent session (Service Factory).
         * \param dataset_id dataset Id where image will be created.
         * \param pixel_type new image's pixel type.
         * \param width image width.
         * \param height image height.
         * \param number_of_planes number of z planes.
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
            const int &width, const int &height, const int &number_of_planes,
            const int &number_of_channels, const int &number_of_timepoints,
            const std::string &name, const std::string &description,
            const double &pixel_size_x, const double &pixel_size_y,
            const double &pixel_size_z
        );
        /// Allocates omero2cv::image_store object to store the data in memory.
        void allocate_pixel_store();
        /// Allocates omero2cv::image_store object to store the data in memory.
        /// This method is for single plane retrival.
        /*!
         * \param timepoint timepoint to read.
         */
        void allocate_pixel_store(
            const int timepoint
        );
        /// Allocates omero2cv::image_store object to store the data in memory.
        /// This method is for single plane retrival.
        /*!
         * \param timepoint timepoint to read.
         * \param channel channel to read.
         */
        void allocate_pixel_store(
            const int timepoint, const int channel
        );
        /// Allocates omero2cv::image_store object to store the data in memory.
        /// This method is for single plane retrival.
        /*!
         * \param timepoint timepoint to read.
         * \param channel channel to read.
         * \param plane plane to read.
         */
        void allocate_pixel_store(
            const int timepoint, const int channel, const int plane
        );
        /// Allocates omero2cv::image_store object to store the data in memory.
        /// This method is for selective data retrival.
        /*!
         * \param timepoint_list list of timepoints to read.
         * \param channel_list list of channels to read.
         * \param plane_list list of planes to read.
         */
        void allocate_pixel_store(
            std::vector<int> timepoint_list,
            std::vector<int> channel_list,
            std::vector<int> plane_list
        );
        /// Write data to server.
        /*!
         * \param image image_store object containing data to write.
         */
        int write_image(omero2cv::image_store *image);
        /// To be implemented.
        int write_channel(
            omero2cv::plane_store *stack,
            const int &timepoint,
            const int &channel
        );
        /// To be implemented.
        int write_plane(
            const cv::Mat &data, 
            const int &timepoint,
            const int &channel,
            const int &plane
        );
        ///
        //void upload_and_link_file(
        //    const omero::api::ServiceFactoryPrx &session,
        //    const std::string &path, const std::string &type)
        //{this->omero_image->upload_and_link_file(session, path, type);};
        /// Deallocate memory.
        void clear_pixel_store();
        ///
        void allocate_zero_mat();
        /// Read the data from the server. Before using this method allocate
        /// buffer using allocate_pixel_store.
        void read_image();
        /// Buffer to store the data in memory.
        image_store *pixel_store;
        /// OMERO pixel type.
        omero::model::PixelsTypePtr pixel_type_omero;
        /// OpenCV pixel type.
        int pixel_type_cv;
        /// Bytes per pixel.
        int pixel_type_bpp;
        /// List of timepoint to read.
        std::vector<int> timepoint_list;
        /// List of channels to read.
        std::vector<int> channel_list;
        /// List of planes to read.
        std::vector<int> plane_list;
        /// Image name.
        std::string name;
        /// Image description.
        std::string description;
        /// Number of channels.
        int number_of_channels;
        /// Number of time points.
        int number_of_timepoints;
        /// Number of timepoints to be read/stored in the memory.
        int pixel_store_timepoints;
        /// OMERO image Id.
        int id;
        /// Image Width.
        int size_x;
        /// Image Height.
        int size_y;
        /// Number of planes
        int size_z;
        /// Physical pixel size in X dimension.
        double pixel_size_x;
        /// Physical pixel size in Y dimension.
        double pixel_size_y;
        /// Physical pixel size in Z dimension.
        double pixel_size_z;
        /// z / x scaling factor.
        double z_scaling;
        /// Maximum pixel value
        double max;
        /// Minimum pixel value
        double min;
    };
#endif //_omero2cv_image_included_
}
