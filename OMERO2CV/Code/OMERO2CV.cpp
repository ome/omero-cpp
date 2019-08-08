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


#include "OMERO2CV.h"


omero2cv::type_converter::type_converter(
    const omero::api::ServiceFactoryPrx &session)
{
    omero::api::IPixelsPrx pixel_service = session->getPixelsService();
    omero::api::IObjectList pixel_types =
        pixel_service->getAllEnumerations("PixelsType");
    std::vector<omero::model::PixelsTypePtr> pixels =
        omero::cast<omero::model::PixelsTypePtr>(pixel_types);
    
    omero::model::PixelsTypePtr pixel;
    for (int i = 0; i < pixels.size(); i++) {
        this->omero_pixels.push_back(pixel);
    }
    std::string pixel_name;
    for (int i = 0; i < pixels.size(); i++) {
        pixel_name = pixels.at(i)->getValue()->getValue();
        if (pixel_name == "bit") {
            this->omero_pixels.at(o2cv_bit) = pixels.at(i);
        }
        else if (pixel_name == "int8") {
            this->omero_pixels.at(o2cv_int8) = pixels.at(i);
        }
        else if (pixel_name == "uint8") {
            this->omero_pixels.at(o2cv_uint8) = pixels.at(i);
        }
        else if (pixel_name == "int16") {
            this->omero_pixels.at(o2cv_int16) = pixels.at(i);
        }
        else if (pixel_name == "int32") {
            this->omero_pixels.at(o2cv_int32) = pixels.at(i);
        }
        else if (pixel_name == "uint16") {
            this->omero_pixels.at(o2cv_uint16) = pixels.at(i);
        }
        else if (pixel_name == "uint32") {
            this->omero_pixels.at(o2cv_uint32) = pixels.at(i);
        }
        else if (pixel_name == "float") {
            this->omero_pixels.at(o2cv_flt) = pixels.at(i);
        }
        else if (pixel_name == "double") {
            this->omero_pixels.at(o2cv_dbl) = pixels.at(i);
        }
        else if(pixel_name == "complex") {
            this->omero_pixels.at(o2cv_complex) = pixels.at(i);
        }
        else if(pixel_name == "double-complex") {
            this->omero_pixels.at(o2cv_double_complex) = pixels.at(i);
        }
        else {
            std::cout << "\n\t*" << pixel_name << "* Type not found.\n";
        }
    }
}


int omero2cv::type_converter::omero_2_cv(
        omero::model::PixelsTypePtr omero_type)
{
    std::string type = omero_type->getValue()->getValue();
    if (type == "bit") {
        std::cout << "Type not supported !!!!!!!\n";
        return -1;
    }
    else if(type == "int8") {
        return CV_8S;
    }
    else if(type == "uint8") {
        return CV_8U;
    }
    else if(type == "int16") {
        return CV_16S;
    }
    else if (type == "uint16") {
        return CV_16U;
    }
    else if (type == "int32") {
        return CV_32S;
    }
    else if (type == "uint32") {
        std::cout << "Type not supported !!!!!!!!!!\n";
        return -1;
    }
    else if (type == "float") {
        return CV_32F;
    }
    else if (type == "double") {
        return CV_64F;
    }
    else if (type == "complex") {
        std::cout << "Type not supported !!!!!!!!!!\n";
        return -1;
    }
    else if (type == "double-complex") {
        std::cout << "Type not supported !!!!!!!!!!\n";
        return -1;
    }
    else {
        std::cout << "Type not listed!!!!!!!!\n";
        return -1;
    }
}


int omero2cv::type_converter::cv_2_omero(
    int cv_type, omero::model::PixelsTypePtr *omero_type)
{
    if (cv_type == CV_8S) {
        *omero_type = this->omero_pixels.at(o2cv_int8);
        return 0;
    }
    else if (cv_type == CV_8U) {
        *omero_type = this->omero_pixels.at(o2cv_uint8);
        return 0;
    }
    else if (cv_type == CV_16S) {
        *omero_type = this->omero_pixels.at(o2cv_int16);
        return 0;
    }
    else if (cv_type == CV_16U) {
        *omero_type = this->omero_pixels.at(o2cv_uint16);
        return 0;
    }
    else if (cv_type == CV_32S) {
        *omero_type = this->omero_pixels.at(o2cv_int32);
        return 0;
    }
    else if (cv_type == CV_32F) {
        *omero_type = this->omero_pixels.at(o2cv_flt);
        return 0;
    }
    else if (cv_type == CV_64F) {
        *omero_type = this->omero_pixels.at(o2cv_dbl);
        return 0;
    }
    else {
        std::cout << "\n\t*" << cv_type << "* Type not supported.\n";
        return -1;
    }
}


int omero2cv::type_converter::get_bpp(int cv_type)
{
    if (cv_type == CV_8S) {
        return 1;
    }
    else if (cv_type == CV_8U) {
        return 1;
    }
    else if (cv_type == CV_16S) {
        return 2;
    }
    else if (cv_type == CV_16U) {
        return 2;
    }
    else if (cv_type == CV_32S) {
        return 4;
    }
    else if (cv_type == CV_32F) {
        return 4;
    }
    else if (cv_type == CV_64F) {
        return 8;
    }
    else {
        std::cout << "\n\t*" << cv_type << "* Type not supported.\n";
        return(-1);
    }
}


int omero2cv::type_converter::get_bpp(
    omero::model::PixelsTypePtr omero_type)
{
    std::string type = omero_type->getValue()->getValue();
    if (type == "bit") {
        return 1;
    }
    else if(type == "int8") {
        return 1;
    }
    else if(type == "uint8") {
        return 1;
    }
    else if(type == "int16") {
        return 2;
    }
    else if (type == "uint16") {
        return 2;
    }
    else if (type == "int32") {
        return 4;
    }
    else if (type == "uint32") {
        return 4;
    }
    else if (type == "float") {
        return 4;
    }
    else if (type == "double") {
        return 8;
    }
    else if (type == "complex") {
        return 8;
    }
    else if (type == "double-complex") {
        return 16;
    }
    else {
        std::cout << "Type not listed!!!!!!!!\n";
        return -1;
    }
}


omero2cv::plane_store::plane_store(
    double pixel_size_x, double pixel_size_y, double pixel_size_z)
{
    this->pixel_size_x = pixel_size_x;
    this->pixel_size_y = pixel_size_y;
    this->pixel_size_z = pixel_size_z;
    this->z_scaling = this->pixel_size_z / this->pixel_size_x;
}


void omero2cv::plane_store::allocate_zero_mat(
   const int &width, const int &height, const int &depth, const int &cv_type)
{
    cv::Mat temp_buffer = cv::Mat::zeros(height, width, cv_type);
    for (int z = 0; z < depth; z++) {
        //this->at(z) = temp_buffer.clone();
        this->push_back(temp_buffer.clone());
    }
}   


void omero2cv::image::allocate_zero_mat()
{
    cv::Mat temp_buffer =
        cv::Mat::zeros(this->size_y, this->size_x, this->pixel_type_cv);
    for (int t = 0; t < this->pixel_store_timepoints; t++) {
        for (int c = 0; c < this->pixel_store->number_of_channels; c++) {
            for (int z = 0; z < this->pixel_store->size_z; z++) {
                this->pixel_store->t(t)->c(c)->z(z) = temp_buffer.clone();
            }
        }
    }
}


omero2cv::image::~image()
{
    std::cout << "closing stuff\n";
    this->clear_pixel_store();
    this->omero_image->close_pixel_store();
    delete this->omero_image;
    delete this->converter;
}


omero2cv::image::image(
    const omero::api::ServiceFactoryPrx &session, const int &image_id)
{
/// Still Needs Error Handling!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    this->timepoint_list.clear();
    this->channel_list.clear();
    this->plane_list.clear();
    
    this->id = image_id;
    this->omero_image = new simple_omero::image(session, image_id);
    this->omero_image->open_pixel_store(session);
    this->name = this->omero_image->name;
    this->description = this->omero_image->description;
    this->size_x = this->omero_image->size_x;
    this->size_y = this->omero_image->size_y;
    this->size_z = this->omero_image->size_z;
    this->number_of_channels =
        this->omero_image->number_of_channels;
    this->number_of_timepoints =
        this->omero_image->number_of_timepoints;
    this->pixel_size_x = this->omero_image->pixel_size_x;
    this->pixel_size_y = this->omero_image->pixel_size_y;
    this->pixel_size_z = this->omero_image->pixel_size_z;
    this->z_scaling = this->pixel_size_z / this->pixel_size_x;
    this->max = 0.0;
    this->min = 0.0;

    this->converter = new type_converter(session);
    this->pixel_type_omero = this->omero_image->pixel_type;
    this->pixel_type_cv = converter->omero_2_cv(this->pixel_type_omero);
    this->pixel_type_bpp = converter->get_bpp(this->pixel_type_cv);

    for (int t = 0; t < this->number_of_timepoints; t++) {
        this->timepoint_list.push_back(t);
    }
    for (int c = 0; c < this->number_of_channels; c++) {
        this->channel_list.push_back(c);
    }
    for (int z = 0; z < this->size_z; z++) {
        this->plane_list.push_back(z);
    }
    
    this->pixel_store = new image_store();
    this->pixel_store_timepoints = this->number_of_timepoints;
    this->pixel_store->number_of_channels = this->number_of_channels;
    this->pixel_store->size_x = this->size_x;
    this->pixel_store->size_y = this->size_y;
    this->pixel_store->size_z = this->size_z;
    this->pixel_store->pixel_size_x = this->pixel_size_x;
    this->pixel_store->pixel_size_y = this->pixel_size_y;
    this->pixel_store->pixel_size_z = this->pixel_size_z;
    this->pixel_store->z_scaling = this->z_scaling;
}


omero2cv::image::image(
    const omero::api::ServiceFactoryPrx &session,
    const int &image_id,
    std::vector<int> timepoint_list,
    std::vector<int> channel_list,
    std::vector<int> plane_list)
{
    this->timepoint_list.clear();
    this->channel_list.clear();
    this->plane_list.clear();
    this->timepoint_list = timepoint_list;
    this->channel_list = channel_list;
    this->plane_list = plane_list;
    
    this->id = image_id;
    this->omero_image = new simple_omero::image(session, image_id);
    this->omero_image->open_pixel_store(session);
    this->name = this->omero_image->name;
    this->description = this->omero_image->description;
    this->size_x = this->omero_image->size_x;
    this->size_y = this->omero_image->size_y;
    this->size_z = this->omero_image->size_z;
    this->pixel_size_x = this->omero_image->pixel_size_x;
    this->pixel_size_y = this->omero_image->pixel_size_y;
    this->pixel_size_z = 0.0;
    this->number_of_channels = this->omero_image->number_of_channels;
    this->number_of_timepoints = this->omero_image->number_of_timepoints;
    this->z_scaling = this->pixel_size_z / this->pixel_size_x;
    this->max = 0.0;
    this->min = 0.0;
    
    this->converter = new type_converter(session);
    this->pixel_type_omero = this->omero_image->pixel_type;
    this->pixel_type_cv = converter->omero_2_cv(this->pixel_type_omero);
    this->pixel_type_bpp = converter->get_bpp(this->pixel_type_cv);
    
    this->pixel_store = new image_store();
    this->pixel_store_timepoints = timepoint_list.size();
    this->pixel_store->number_of_channels = channel_list.size();;
    this->pixel_store->size_x = this->size_x;
    this->pixel_store->size_y = this->size_y;
    this->pixel_store->size_z = plane_list.size();
    this->pixel_store->pixel_size_x = this->pixel_size_x;
    this->pixel_store->pixel_size_y = this->pixel_size_y;
    this->pixel_store->pixel_size_z = this->pixel_size_z;
    this->pixel_store->z_scaling = this->z_scaling;
}


omero2cv::image::image(
    const omero::api::ServiceFactoryPrx &session, const int &dataset_id,
    const omero::model::PixelsTypePtr &pixel_type, const int &width,
    const int &height, const int &number_of_planes,
    const int &number_of_channels, const int &number_of_timepoints,
    const std::string &name, const std::string &description,
    const double &pixel_size_x,
    const double &pixel_size_y,
    const double &pixel_size_z)
{
    this->omero_image = new simple_omero::image(
        session, dataset_id, pixel_type, width, height, number_of_planes,
        number_of_channels, number_of_timepoints, name, description,
        pixel_size_x, pixel_size_y, pixel_size_z
    );
    
    this->omero_image->open_pixel_store(session);
    this->name = this->omero_image->name;
    this->description = this->omero_image->description;
    this->size_x = this->omero_image->size_x;
    this->size_y = this->omero_image->size_y;
    this->size_z = this->omero_image->size_z;
    this->pixel_size_x = this->omero_image->pixel_size_x;
    this->pixel_size_y = this->omero_image->pixel_size_y;
    this->pixel_size_z = this->omero_image->pixel_size_z;
    this->number_of_channels = this->omero_image->number_of_channels;
    this->number_of_timepoints = this->omero_image->number_of_timepoints;
    this->z_scaling = this->pixel_size_z / this->pixel_size_x;
    this->max = 0.0;
    this->min = 0.0;
   
    this->converter = new type_converter(session);
    this->pixel_type_omero = this->omero_image->pixel_type;
    this->pixel_type_cv = converter->omero_2_cv(this->pixel_type_omero);
    this->pixel_type_bpp = converter->get_bpp(this->pixel_type_cv);
    
    this->pixel_store = new image_store();
    this->pixel_store_timepoints = this->number_of_timepoints;
    this->pixel_store->number_of_channels = this->number_of_channels;
    this->pixel_store->size_x = this->size_x;
    this->pixel_store->size_y = this->size_y;
    this->pixel_store->size_z = this->size_z;
    this->pixel_store->pixel_size_x = this->pixel_size_x;
    this->pixel_store->pixel_size_y = this->pixel_size_y;
    this->pixel_store->pixel_size_z = this->pixel_size_z;
    this->pixel_store->z_scaling = this->z_scaling;
}


int omero2cv::image::write_image(omero2cv::image_store *image)
{
    simple_omero::logger *log = new simple_omero::logger();
    if (this->number_of_timepoints != image->size()) {
        std::cout << "\tNumber of time points incorrect!!!!\n";
        return -1;
    }
    if (this->number_of_channels != image->t(0)->size()) {
        std::cout << "\tNumber of channels incorrect!!!!\n";
        return -1;
    }
    if (this->size_z != image->t(0)->c(0)->size()) {
        std::cout << this->size_z << " " << image->t(0)->c(0)->size() << "\n";
        std::cout << "\tNumber of planes incorrect!!!!!!\n";
        return -1;
    }
    
    unsigned char *buffer;
    cv::Mat image_temp;
    for (int t = 0; t < this->number_of_timepoints; t++) {
        for (int c = 0; c < this->number_of_channels; c++) {
            std::cout << log->date_time_now()
                      << " Writing Image: " << this->omero_image->id
                      << " time point: " << t
                      << " channel: " << c
                      << "\n";
            for (int z = 0; z < this->size_z; z++) {
                image_temp =
                    image->t(t)->c(c)->z(z).clone();
                if (this->pixel_type_bpp > 1) {
                    cv::flip(image_temp, image_temp, -1);
                }
                buffer = reinterpret_cast<unsigned char *>(image_temp.data);
                this->omero_image->write_plane(
                    buffer, this->pixel_type_bpp, t, c, z);
            }
        }
    }
    return 0;
}


int omero2cv::image::write_channel(
    omero2cv::plane_store *stack,
    const int &timepoint, const int &channel)
{
    simple_omero::logger *log = new simple_omero::logger();
    if (this->pixel_store->size_z != stack->size()) {
        std::cout << "\tNumber of planes incorrect!!!!!!\n";
        return -1;
    }
    unsigned char *buffer;
    cv::Mat image_temp;
    std::cout << log->date_time_now()
              << " Writing Image: " << this->omero_image->id
              << " time point: " << timepoint
              << " channel: " << channel
              << "\n";
    for (int z = 0; z < this->pixel_store->size_z; z++) {
        image_temp = stack->z(z).clone();
        if (this->pixel_type_bpp > 1) {
            cv::flip(image_temp, image_temp, -1);
        }
        buffer = reinterpret_cast<unsigned char *>(image_temp.data);
        this->omero_image->write_plane(
            buffer, this->pixel_type_bpp, timepoint, channel, z);
    }
    return 0;
}


int omero2cv::image::write_plane(
    const cv::Mat &data, const int &timepoint,
    const int &channel, const int &plane)
{
    simple_omero::logger *log = new simple_omero::logger();
    if (this->pixel_store->size_z < plane) {
        std::cout << "\twrite_plane: ";
        std::cout << this->size_z << " " << plane << "\n";
        std::cout << "\tPlane index wrong!!!!!!\n";
        return -1;
    }
    unsigned char *buffer;
    cv::Mat image_temp;
    std::cout << log->date_time_now()
              << " Writing Image: " << this->omero_image->id
              << " time point: " << timepoint
              << " channel: " << channel
              << " plane: " << plane
              << "\n";
    image_temp = data.clone();
    if (this->pixel_type_bpp > 1) {
        cv::flip(image_temp, image_temp, -1);
    }
    buffer = reinterpret_cast<unsigned char *>(image_temp.data);
    this->omero_image->write_plane(
        buffer, this->pixel_type_bpp, timepoint, channel, plane);
    return 0;
}


void omero2cv::image::allocate_pixel_store()
{
    this->pixel_store = new image_store();
    
    this->timepoint_list.clear();
    this->channel_list.clear();
    this->plane_list.clear();
    
    for (int t = 0; t < this->number_of_timepoints; t++) {
        this->timepoint_list.push_back(t);
    }
    for (int c = 0; c < this->number_of_channels; c++) {
        this->channel_list.push_back(c);
    }
    for (int z = 0; z < this->size_z; z++) {
        this->plane_list.push_back(z);
    }
    
    this->pixel_store_timepoints = this->number_of_timepoints;
    this->pixel_store->number_of_channels = this->number_of_channels;
    this->pixel_store->size_x = this->size_x;
    this->pixel_store->size_y = this->size_y;
    this->pixel_store->size_z = this->size_z;
    this->pixel_store->pixel_size_x = this->pixel_size_x;
    this->pixel_store->pixel_size_y = this->pixel_size_y;
    this->pixel_store->pixel_size_z = this->pixel_size_z;
    this->pixel_store->z_scaling = this->pixel_size_z / this->pixel_size_x;
    
    plane_store *planes;
    channel_store *temp = new channel_store();

    for (int t = 0; t < this->pixel_store_timepoints; t++) {
        temp = new channel_store();
        temp->size_x = this->pixel_store->size_x;
        temp->size_y = this->pixel_store->size_y;
        temp->size_z = this->pixel_store->size_z;
        temp->pixel_size_x = this->pixel_store->pixel_size_x;
        temp->pixel_size_y = this->pixel_store->pixel_size_y;
        temp->pixel_size_z = this->pixel_store->pixel_size_z;
        for (int c = 0; c < this->pixel_store->number_of_channels; c++) {
             planes = new plane_store(
                this->pixel_store->pixel_size_x,
                this->pixel_store->pixel_size_y,
                this->pixel_store->pixel_size_z
            );
            planes->resize(this->pixel_store->size_z);
            temp->push_back(planes);
        }
        this->pixel_store->push_back(temp);
    }
}


void omero2cv::image::allocate_pixel_store(const int timepoint)
{
    this->pixel_store = new image_store();
    
    this->timepoint_list.clear();
    this->channel_list.clear();
    this->plane_list.clear();

    this->timepoint_list.push_back(timepoint);
    for (int c = 0; c < this->number_of_channels; c++) {
        this->channel_list.push_back(c);
    }
    for (int z = 0; z < this->size_z; z++) {
        this->plane_list.push_back(z);
    }
    
    this->pixel_store_timepoints = 1;
    this->pixel_store->number_of_channels = this->number_of_channels;
    this->pixel_store->size_x = this->size_x;
    this->pixel_store->size_y = this->size_y;
    this->pixel_store->size_z = this->size_z;
    this->pixel_store->pixel_size_x = this->pixel_size_x;
    this->pixel_store->pixel_size_y = this->pixel_size_y;
    this->pixel_store->pixel_size_z = this->pixel_size_z;
    this->pixel_store->z_scaling = this->pixel_size_z / this->pixel_size_x;
    
    plane_store *planes;
    channel_store *temp = new channel_store();

    for (int t = 0; t < this->pixel_store_timepoints; t++) {
        temp = new channel_store();
        temp->size_x = this->pixel_store->size_x;
        temp->size_y = this->pixel_store->size_y;
        temp->size_z = this->pixel_store->size_z;
        temp->pixel_size_x = this->pixel_store->pixel_size_x;
        temp->pixel_size_y = this->pixel_store->pixel_size_y;
        temp->pixel_size_z = this->pixel_store->pixel_size_z;
        for (int c = 0; c < this->pixel_store->number_of_channels; c++) {
            planes = new plane_store(
                this->pixel_store->pixel_size_x,
                this->pixel_store->pixel_size_y,
                this->pixel_store->pixel_size_z
            );
            planes->resize(this->pixel_store->size_z);
            temp->push_back(planes);
        }
        this->pixel_store->push_back(temp);
    }
}


void omero2cv::image::allocate_pixel_store(
    const int timepoint, const int channel)
{
    this->pixel_store = new image_store();
    
    this->timepoint_list.clear();
    this->channel_list.clear();
    this->plane_list.clear();

    this->timepoint_list.push_back(timepoint);
    this->channel_list.push_back(channel);
    for (int z = 0; z < this->size_z; z++) {
        this->plane_list.push_back(z);
    }
    
    this->pixel_store_timepoints = 1;
    this->pixel_store->number_of_channels = 1;
    this->pixel_store->size_x = this->size_x;
    this->pixel_store->size_y = this->size_y;
    this->pixel_store->size_z = this->size_z;
    this->pixel_store->pixel_size_x = this->pixel_size_x;
    this->pixel_store->pixel_size_y = this->pixel_size_y;
    this->pixel_store->pixel_size_z = this->pixel_size_z;
    this->pixel_store->z_scaling = this->pixel_size_z / this->pixel_size_x;
    
    plane_store *planes;
    channel_store *temp = new channel_store();

    for (int t = 0; t < this->pixel_store_timepoints; t++) {
        temp = new channel_store();
        temp->size_x = this->pixel_store->size_x;
        temp->size_y = this->pixel_store->size_y;
        temp->size_z = this->pixel_store->size_z;
        temp->pixel_size_x = this->pixel_store->pixel_size_x;
        temp->pixel_size_y = this->pixel_store->pixel_size_y;
        temp->pixel_size_z = this->pixel_store->pixel_size_z;
        for (int c = 0; c < this->pixel_store->number_of_channels; c++) {
            planes = new plane_store(
                this->pixel_store->pixel_size_x,
                this->pixel_store->pixel_size_y,
                this->pixel_store->pixel_size_z
            );
            planes->resize(this->pixel_store->size_z);
            temp->push_back(planes);
        }
        this->pixel_store->push_back(temp);
    }
}


void omero2cv::image::allocate_pixel_store(
    const int timepoint, const int channel, const int plane)
{
    this->pixel_store = new image_store();
    
    this->timepoint_list.clear();
    this->channel_list.clear();
    this->plane_list.clear();

    this->timepoint_list.push_back(timepoint);
    this->channel_list.push_back(channel);
    this->plane_list.push_back(plane);

    this->pixel_store_timepoints = 1;
    this->pixel_store->number_of_channels = 1;
    this->pixel_store->size_x = this->size_x;
    this->pixel_store->size_y = this->size_y;
    this->pixel_store->size_z = 1;
    this->pixel_store->pixel_size_x = this->pixel_size_x;
    this->pixel_store->pixel_size_y = this->pixel_size_y;
    this->pixel_store->pixel_size_z = this->pixel_size_z;
    this->pixel_store->z_scaling = this->pixel_size_z / this->pixel_size_x;
    
    plane_store *planes;
    channel_store *temp = new channel_store();
    
    for (int t = 0; t < this->pixel_store_timepoints; t++) {
        temp = new channel_store();
        temp->size_x = this->pixel_store->size_x;
        temp->size_y = this->pixel_store->size_y;
        temp->size_z = this->pixel_store->size_z;
        temp->pixel_size_x = this->pixel_store->pixel_size_x;
        temp->pixel_size_y = this->pixel_store->pixel_size_y;
        temp->pixel_size_z = this->pixel_store->pixel_size_z;
        for (int c = 0; c < this->pixel_store->number_of_channels; c++) {
            planes = new plane_store(
                this->pixel_store->pixel_size_x,
                this->pixel_store->pixel_size_y,
                this->pixel_store->pixel_size_z
            );
            planes->resize(this->pixel_store->size_z);
            temp->push_back(planes);
        }
        this->pixel_store->push_back(temp);
    }
}


void omero2cv::image::allocate_pixel_store(
    std::vector<int> timepoint_list,
    std::vector<int> channel_list,
    std::vector<int> plane_list)
{
    this->pixel_store = new image_store();
    
    this->timepoint_list.clear();
    this->channel_list.clear();
    this->plane_list.clear();
    this->timepoint_list = timepoint_list;
    this->channel_list = channel_list;
    this->plane_list = plane_list;

    this->pixel_store_timepoints = this->timepoint_list.size();
    this->pixel_store->number_of_channels = this->channel_list.size();
    this->pixel_store->size_x = this->size_x;
    this->pixel_store->size_y = this->size_y;
    this->pixel_store->size_z = this->plane_list.size();
    this->pixel_store->pixel_size_x = this->pixel_size_x;
    this->pixel_store->pixel_size_y = this->pixel_size_y;
    this->pixel_store->pixel_size_z = this->pixel_size_z;
    this->pixel_store->z_scaling = this->pixel_size_z / this->pixel_size_x;
    
    plane_store *planes;
    channel_store *temp = new channel_store();
    
    for (int t = 0; t < this->pixel_store_timepoints; t++) {
        temp = new channel_store();
        temp->size_x = this->pixel_store->size_x;
        temp->size_y = this->pixel_store->size_y;
        temp->size_z = this->pixel_store->size_z;
        temp->pixel_size_x = this->pixel_store->pixel_size_x;
        temp->pixel_size_y = this->pixel_store->pixel_size_y;
        temp->pixel_size_z = this->pixel_store->pixel_size_z;
        for (int c = 0; c < this->pixel_store->number_of_channels; c++) {
            planes = new plane_store(
                this->pixel_store->pixel_size_x,
                this->pixel_store->pixel_size_y,
                this->pixel_store->pixel_size_z
            );
            planes->resize(this->pixel_store->size_z);
            temp->push_back(planes);
        }
        this->pixel_store->push_back(temp);
    }
}


void omero2cv::image::clear_pixel_store()
{
    for (int t = 0; t < this->pixel_store->size(); t++) {
        for (int c = 0; c < this->pixel_store->number_of_channels; c++) {
            for (int z = 0; z < this->pixel_store->size_z; z++) {
                this->pixel_store->t(t)->c(c)->z(z).release();
            }
            this->pixel_store->t(t)->c(c)->clear();
            delete this->pixel_store->t(t)->c(c);
        }
        this->pixel_store->t(t)->clear();
        delete this->pixel_store->t(t);
    }
    this->pixel_store->clear();
    delete this->pixel_store;
    this->timepoint_list.clear();
    this->channel_list.clear();
    this->plane_list.clear();
    this->pixel_store_timepoints = this->timepoint_list.size();
    this->pixel_store->number_of_channels = this->channel_list.size();
    this->pixel_store->size_z = this->plane_list.size();
    this->pixel_store->pixel_size_z = 0;
}


void omero2cv::image::read_image()
{
    simple_omero::logger *log = new simple_omero::logger();
    int timepoint, channel, plane;
    cv::Mat buffer;
    unsigned char *image_cast =
        (unsigned char *) malloc (
            this->omero_image->pixel_store->getPlaneSize()
    );
    for (int t = 0; t < this->pixel_store_timepoints; t++) {
        timepoint = this->timepoint_list.at(t);
        for (int c = 0; c < this->pixel_store->number_of_channels; c++) {
            channel = this->channel_list.at(c);
            std::cout << log->date_time_now()
                      << " Reading Image: " << this->omero_image->id
                      << " time point: " << timepoint
                      << " channel: " << channel
                      << "\n";
            for (int z = 0; z < this->pixel_store->size_z; z++) {
                plane = this->plane_list.at(z);
                this->omero_image->get_raw_pixels(
                    image_cast, plane, channel, timepoint, this->pixel_type_bpp
                );
                buffer = cv::Mat(
                    this->size_y, this->size_x, this->pixel_type_cv, image_cast
                );
                if (this->pixel_type_bpp > 1) {
                    // Second part of conversion from BIG_ENDIAN (OMERO)
                    // to LITTLE_ENDIAN (OpenCV), first part in get_raw_pixels
                    cv::flip(buffer, buffer, -1);
                }
                this->pixel_store->t(t)->c(c)->z(z) = buffer.clone();
            }
        }
    }
    delete image_cast;
    delete log;
}
