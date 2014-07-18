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


#include "SimpleOMERO.h"


int simple_omero::connector::connect(
    const std::string &host, const std::string &port,
    const std::string &user, const std::string &pass)
{
    this->host = host;
    this->port = port;
    this->user = user;
    this->pass = pass;
    int error_code = set_client();
    if (error_code == -1)
        return -1;
    error_code = set_session();
    if (error_code == -1)
        return -1;
    error_code = set_admin();
    if (error_code == -1)
        return -1;
    this->printSessionDetails();
    this->get_session();
    return 0;
}


int simple_omero::connector::set_client()
{
    try {
        omero::client_ptr initialize_client =
            new omero::client(initialize_connection_data());
        omero::api::ServiceFactoryPrx session_new =
            initialize_client->createSession();
        client = initialize_client->createClient(false);
    }
    catch (...) {
        std::cout << "\n" << this->log->date_time_now()
                  << "Problem connecting to the server.\n\n";
        return -1;
    }
    return 0;
}


int simple_omero::connector::set_session()
{
    try {
        //session = client->createSession();
        session = client->getSession();
        session->closeOnDestroy();
    } catch (...) {
        std::cout << "\n" << this->log->date_time_now()
                  << "Problem connecting to the server.\n\n";
        return -1;
    }
    return 0;
}


int simple_omero::connector::set_admin()
{
    try {
        admin = session->getAdminService();
    } catch (...) {
        std::cout << "\n" << this->log->date_time_now()
                  << "Problem connecting to the server.\n\n";
        return -1;
    }
    return 0;
}


Ice::InitializationData simple_omero::connector::initialize_connection_data()
{
    Ice::InitializationData data;
    data.properties = Ice::createProperties();
    data.properties->setProperty( "omero.host", this->host );
    data.properties->setProperty( "omero.port", this->port );
    data.properties->setProperty( "omero.user", this->user );
    data.properties->setProperty( "omero.pass", this->pass );
    return data;
}


void simple_omero::connector::list_images_in_datasets()
{
    omero::api::IContainerPrx container_service =
        this->session->getContainerService();
    int userID = admin->getEventContext()->userId;
    omero::sys::ParametersIPtr param = new omero::sys::ParametersI();
    param->exp(userID);
    param->leaves();
    omero::sys::LongList datasetList;
    omero::api::IObjectList datasets =
        container_service->loadContainerHierarchy(
            "Dataset", datasetList, param
    );
    std::vector<omero::model::DatasetPtr> dataset =
        omero::cast<omero::model::DatasetPtr>(datasets);
    omero::model::DatasetLinkedImageSeq imageList;
    std::cout << "\n" << this->log->date_time_now() << "Dataset List\n";
    for (int i = 0; i < dataset.size(); i++) {
        std::cout << "----\n Dataset: "
                  << dataset.at(i)->getName()->getValue()
                  << "\n";
        std::cout << " Images: "
                  << dataset.at(i)->linkedImageList().size()
                  << "\n";
        imageList = dataset.at(i)->linkedImageList();
        for (int j = 0; j < imageList.size(); j++) {
            std::cout << "\tId: " << imageList.at(j)->getId()->getValue()
                      << ", Name: " << imageList.at(j)->getName()->getValue()
                      << "\n";
        }
    }
    std::cout << "----\n";
}


void simple_omero::connector::list_pixel_types()
{
    omero::api::IPixelsPrx pixel_service = this->session->getPixelsService();
    omero::api::IObjectList pixelTypes =
        pixel_service->getAllEnumerations("PixelsType");
    std::vector<omero::model::PixelsTypePtr> pixels =
        omero::cast<omero::model::PixelsTypePtr>(pixelTypes);
    for (int i = 0; i < pixels.size(); i ++)
    {
        std::cout << pixels.at(i)->getValue()->getValue() << " "
                  << pixels.at(i)->getBitSize()->getValue() << "\n";
    }
}


void simple_omero::connector::printSessionDetails()
{
    std::cout << "\n"
              << this->log->date_time_now()
              << " Connected to server: " << host
              << " as " << user
              << "\n";
    std::cout << "--------\n";
    std::cout << "User: " << admin->getEventContext()->userName << "\n"
              << "Id: "   << admin->getEventContext()->userId << "\n"
              << "Current group: " << admin->getEventContext()->groupName
              << "\n";
    std::cout << "---------\n";
}


simple_omero::image::image(
    const omero::api::ServiceFactoryPrx &session, const int &image_id)
{
    omero::api::IContainerPrx container_service =
        session->getContainerService();
    omero::sys::LongList id_list;
    id_list.push_back(image_id);
    omero::api::ImageList image = container_service->getImages(
        "Image", id_list, new omero::sys::ParametersI());
    
    this->Pointer = image.at(0);
    this->id = this->Pointer->getId()->getValue();
    this->name = this->Pointer->getName()->getValue();
    try {
        this->description = this->Pointer->getDescription()->getValue();
    } catch (...) {
        this->description = "no description";
    }
    this->pixel_type =
        this->Pointer->getPrimaryPixels()->getPixelsType();
    this->number_of_channels =
        this->Pointer->getPrimaryPixels()->getSizeC()->getValue();
    this->number_of_timepoints =
        this->Pointer->getPrimaryPixels()->getSizeT()->getValue();
    this->size_x = this->Pointer->getPrimaryPixels()->getSizeX()->getValue();
    this->size_y = this->Pointer->getPrimaryPixels()->getSizeY()->getValue();
    this->size_z = this->Pointer->getPrimaryPixels()->getSizeZ()->getValue();
    this->pixel_size_x =
        this->Pointer->getPrimaryPixels()->getPhysicalSizeX()->getValue();
    this->pixel_size_y =
        this->Pointer->getPrimaryPixels()->getPhysicalSizeY()->getValue();
    if (this->size_z > 1) {
        this->pixel_size_z =
            this->Pointer->getPrimaryPixels()->getPhysicalSizeZ()->getValue();
    } else {
        this->pixel_size_z = 0.0;
    }
    this->print_details();
}


simple_omero::image::image(
    const omero::api::ServiceFactoryPrx &session, const int &dataset_id,
    const omero::model::PixelsTypePtr &pixel_type, const int &width,
    const int &height, const int &depth, const int &number_of_channels,
    const int &number_of_timepoints, const std::string &name,
    const std::string &description, const double &pixel_size_x,
    const double &pixel_size_y, const double &pixel_size_z)
{
    omero::api::IPixelsPrx pixel_service = session->getPixelsService();
    omero::sys::IntList list;
    for (int i = 0; i < number_of_channels; i++) {
        list.push_back(i);
    }
    omero::RLongPtr newID =
        pixel_service->createImage(
            width, height, depth, number_of_timepoints,
            list, pixel_type, name, description
    );
    omero::api::IContainerPrx container_service =
        session->getContainerService();
    omero::sys::LongList dataset_list;
    dataset_list.push_back(dataset_id);
    omero::api::IObjectList datasets =
        container_service->loadContainerHierarchy(
            "Dataset", dataset_list, new omero::sys::ParametersI()
    );
    std::vector<omero::model::DatasetPtr> dataset =
        omero::cast<omero::model::DatasetPtr>(datasets);
    omero::sys::LongList image_list;
    image_list.push_back(newID->getValue());
    omero::api::ImageList new_image =
        container_service->getImages(
            "Image", image_list, new omero::sys::ParametersI()
    );
    omero::model::DatasetImageLinkIPtr link =
    new omero::model::DatasetImageLinkI();
    this->Pointer = new_image.at(0);
    link->setParent(dataset.at(0));
    link->setChild(this->Pointer);
    session->getUpdateService()->saveAndReturnObject(link);

    this->Pointer->getPrimaryPixels()->setPhysicalSizeX(
        omero::rtypes::rdouble(pixel_size_x)
    );
    this->Pointer->getPrimaryPixels()->setPhysicalSizeY(
        omero::rtypes::rdouble(pixel_size_y)
    );
    this->Pointer->getPrimaryPixels()->setPhysicalSizeZ(
        omero::rtypes::rdouble(pixel_size_z)
    );
    this->Pointer =
        omero::model::ImageIPtr::dynamicCast(
            session->getUpdateService()->saveAndReturnObject(this->Pointer)
    );
    //
    this->id = this->Pointer->getId()->getValue();
    this->name = this->Pointer->getName()->getValue();
    this->description = this->Pointer->getDescription()->getValue();
    this->pixel_type =
        this->Pointer->getPrimaryPixels()->getPixelsType();
    this->number_of_channels =
        this->Pointer->getPrimaryPixels()->getSizeC()->getValue();
    this->number_of_timepoints =
        this->Pointer->getPrimaryPixels()->getSizeT()->getValue();
    this->size_x = this->Pointer->getPrimaryPixels()->getSizeX()->getValue();
    this->size_y = this->Pointer->getPrimaryPixels()->getSizeY()->getValue();
    this->size_z = this->Pointer->getPrimaryPixels()->getSizeZ()->getValue();
    this->pixel_size_x =
        this->Pointer->getPrimaryPixels()->getPhysicalSizeX()->getValue();
    this->pixel_size_y =
        this->Pointer->getPrimaryPixels()->getPhysicalSizeY()->getValue();
    if (this->size_z > 1) {
        this->pixel_size_z =
            this->Pointer->getPrimaryPixels()->getPhysicalSizeZ()->getValue();
    } else {
        this->pixel_size_z = 0.0;
    }
    std::cout << "Created New Image\n";
    this->print_details();
}


void simple_omero::image::open_pixel_store(
    const omero::api::ServiceFactoryPrx &session)
{
    this->pixel_store = session->createRawPixelsStore();
    this->pixel_store->setPixelsId(
        this->Pointer->getPrimaryPixels()->getId()->getValue(),
        false
    );
}


void simple_omero::image::close_pixel_store()
{
    this->pixel_store->save();
    this->pixel_store->close();
}


// Reading Methods


void simple_omero::image::get_raw_pixels(
    unsigned char *image_cast, const int &plane, const int &channel,
    const int &time_point, const int &bpp)
{
    std::vector<Ice::Byte> image_ice_container;
    image_ice_container =
        this->pixel_store->getPlane(
            plane, channel, time_point
    );
    if (bpp == 1) {
        memcpy(
            image_cast,
            reinterpret_cast<unsigned char *>(&image_ice_container[0]),
            this->pixel_store->getPlaneSize()
        );
    }
    else if (bpp > 1) {
        std::reverse_copy(
            reinterpret_cast<unsigned char *>(&image_ice_container[0]),
            reinterpret_cast<unsigned char *>(&image_ice_container[0])
                + pixel_store->getPlaneSize(),
            image_cast
        );
    }
    image_ice_container.clear();
}


unsigned char *simple_omero::image::get_raw_pixels(
    const omero::api::ServiceFactoryPrx &session,
    const omero::model::ImageIPtr &image, const int &plane, const int &channel,
    const int &time_point, const std::string &image_name)
{
    omero::api::RawPixelsStorePrx pixel_store;
    std::vector<Ice::Byte> image_ice_container;
    pixel_store = session->createRawPixelsStore();
    pixel_store->setPixelsId(
        image->getPrimaryPixels()->getId()->getValue(), false
    );
    image_ice_container = pixel_store->getPlane(
        plane, channel, time_point
    );
    unsigned char *image_cast =
        (unsigned char *) malloc (pixel_store->getPlaneSize());
    int pixel_size =
        image->getPrimaryPixels()->getPixelsType()->getBitSize()->getValue();
    if (pixel_size == 1) {
        memcpy(
            image_cast,
            reinterpret_cast<unsigned char *>(&image_ice_container[0]),
            pixel_store->getPlaneSize()
        );
    }
    else if (pixel_size > 1) {
        std::reverse_copy(
            reinterpret_cast<unsigned char *>(&image_ice_container[0]),
            reinterpret_cast<unsigned char *>(&image_ice_container[0])
                + pixel_store->getPlaneSize(),
            image_cast
        );
    }
    pixel_store->close();
    return image_cast;
}


void simple_omero::image::get_raw_pixels_hyper_cube(
    unsigned char *image_cast, const int &start_x, const int &end_x,
    const int &start_y, const int &end_y, const int &step_y,
    const int &start_z, const int &end_z, const int &bpp)
{
    omero::sys::IntList offset;
    offset.push_back(start_x);
    offset.push_back(start_y);
    offset.push_back(start_z);
    omero::sys::IntList size;
    size.push_back(end_x - start_x);
    size.push_back(end_y - start_y);
    size.push_back(end_z - start_z);
    omero::sys::IntList step;
    step.push_back(1);
    step.push_back(1);
    step.push_back(1);
    std::vector<Ice::Byte> image_ice_container;
    image_ice_container = this->pixel_store->getHypercube(offset, size, step);
    int hyper_cube_size =
        bpp * (end_x - start_x) * (end_y - start_y) * (end_z - start_z);
    if (bpp == 1) {
        memcpy(image_cast,
            reinterpret_cast<unsigned char *>(&image_ice_container[0]),
            hyper_cube_size
        );
    }
    else if (bpp > 1) {
        std::reverse_copy(
            reinterpret_cast<unsigned char *>(&image_ice_container[0]),
            reinterpret_cast<unsigned char *>(&image_ice_container[0])
            + hyper_cube_size, image_cast
        );
    }
}


void simple_omero::image::get_raw_pixels_row(
    unsigned char *image_cast, const int &row, const int &plane,
    const int &channel, const int &time_point, const int &bpp)
{
    std::vector<Ice::Byte> image_ice_container;
    image_ice_container =
        this->pixel_store->getRow(row, plane, channel, time_point);
    if (bpp == 1) {
        memcpy(
            image_cast,
            reinterpret_cast<unsigned char *>(&image_ice_container[0]),
            this->pixel_store->getRowSize()
        );
    }
    else if (bpp > 1) {
        std::reverse_copy(
            reinterpret_cast<unsigned char *>(&image_ice_container[0]),
            reinterpret_cast<unsigned char *>(&image_ice_container[0])
                + pixel_store->getRowSize(),
            image_cast
        );
    }
}


//Writing methods


void simple_omero::image::write_plane(
    unsigned char *buffer, int bpp,
    const int &timepoint, const int &channel, const int &plane)
{
    int size = bpp * this->size_x * this->size_y;
    std::vector<Ice::Byte> bytes;
    bytes.resize(size);
    if (bpp == 1) {
        std::copy(
            reinterpret_cast<Ice::Byte *>(buffer),
            reinterpret_cast<Ice::Byte *>(buffer) + size,
            bytes.begin()
        );
    }
    else if (bpp > 1) {
        std::reverse_copy(
            reinterpret_cast<Ice::Byte *>(buffer),
            reinterpret_cast<Ice::Byte *>(buffer) + size,
            bytes.begin());
    }
    pixel_store->setPlane(bytes, plane, channel, timepoint);
}


void simple_omero::image::print_details()
{
    // Image Description
    std::cout << "  Title: " << this->name << "\n";
    std::cout << "  Description: "
              << this->description
              << "\n";
    std::cout << "  # of Channels: ";
    std::cout << this->number_of_channels
              << "\n";
    std::cout << "  # of Timepoints: ";
    std::cout << this->number_of_timepoints
              << "\n";
    std::cout << "  Size X: "
              << this->size_x
              << " pixels; "
              << this->pixel_size_x
              << "\n";
    std::cout << "  Size Y: "
              << this->size_y
              << " pixels; "
              << this->pixel_size_y
              << "\n";
    std::cout << "  Size Z: "
              << this->size_z
              << " pixels; "
              << this->pixel_size_z
              << "\n";
}


std::string simple_omero::image::zero_pad_number(
    const int &padding, const int &num)
{
    std::ostringstream ss;
    ss << std::setw( padding ) << std::setfill( '0' ) << num;
    return ss.str();
}
/*
 Possible Types with sizes:
 bit 1
 int8 8
 uint8 8
 int16 16
 int32 32
 uint16 32
 uint32 32
 float 32
 double 64
 complex 64
 double-complex 128
*/
