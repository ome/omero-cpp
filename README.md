omero-cpp
=========

C++ tools for interfacing OMERO with various C++ libraries


#### SimpleOMERO example use:

Connector Class:

    simple_omero::connector *Omero = new simple_omero::connector();
    Omero->connect("server", "port", "user", "password");

Image Class:

    int image_id = 1;
    simple_omero::connector *Omero = new simple_omero::connector();
    Omero->connect("server", "port", "user", "password");
    simple_omero::image *image = new simple_omero::image(
      Omero->get_session(), image_id
    );

#### OMERO2CV example use:
Initialise an image object and read the data from the server to the memory.
    
    //Connect to an OMERO server to Read and Write Images.
    simple_omero::connector *Omero =    
        new simple_omero::connector("localhost", "4064", "user", "pass");
    int image_id = 1;
    /// Intitialise a new omero2cv::image object to read the data.
    omero2cv::image *image =
        new omero2cv::image(Omero->get_session(), image_id);
    // Allocate the Pixel Store to store the data in the memory.
    image->allocate_pixel_store();
    // Read the image from the server to the memory.
    image->read_image();
    // Do some operations.
    ... 
    // Delete object.
    delete image;
    delete Omero;
    
Initialise an image object and read selective data from the server to the memory and save it as a new OMERO image.    
    
    // Connect to an OMERO server to Read and Write Images.
    simple_omero::connector *Omero = 
        new simple_omero::connector("localhost", "4064", "user", "pass");
    int image_id = 1;
    // Intitialise a new omero2cv::image object to read the data.
    omero2cv::image *image =
        new omero2cv::image(Omero->get_session(), image_id);
    // Allocate Pixel Store to read only single 3D stack.
    int timepoint = 4; // Choose a time point to read from the time laps data.
    int channel = 2; // Choose a channel to read from the multi-channel data.
    std::vector<int> timepoint_list;
    timepoint_list.push_back(timepoint);
    std::vector<int> channel_list;
    channel_list.push_back(channel);
    std::vector<int> plane_list;
    for (int z = 0; z < image->size_z; z++) { // Read the whole stack or a subset.
        plane_list.push_back(z);
    }
    image->allocate_pixel_store(timepoint_list, channel_list, plane_list);
    // Read the image from the server to the memory.
    image->read_image();
    // Initialise an omero2cv::image object to write the data.
    int dataset_id = 1; // Write a new image to the dataset.
    omero2cv::image *save_image = new omero2cv::image::image(
        Omero->get_session(), dataset_id, image->pixel_type_omero,
        image->size_x, image->size_y, image->pixel_store.t(0).c(0).size(),
        image->pixel_store.t(0).size(), image->pixel_store.size(),
        "New image name", "New image description",
        image->pixel_size_x, image->pixel_size_y, image->pixel_size_z
    );
    save_image->write_image(image->pixel_store);
    // Delet objects.
    delete image;
    delete save_image;
    delete Omero;
    
Display the planes using OpenCV   
    
    // Connect to an OMERO server to Read and Write Images.
    simple_omero::connector *Omero =
        new simple_omero::connector("localhost", "4064", "user", "pass");
    int image_id = 1;
    /// Intitialise a new omero2cv::image object to read the data.
    omero2cv::image *image =
    new omero2cv::image(Omero->get_session(), image_id);
    // Allocate the Pixel Store to store the data in the memory.
    image->allocate_pixel_store();
    // Read the image from the server to the memory.
    image->read_image();
    // Display the image using the OpenCV HighGui class.
    int plane_index_to_show = 0;
    cv::namedWindow( "Display window", CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);
    cv::imshow("Display window", image->pixel_store.t(0).c(0).z(plane_index_to_show));
    cv::waitKey(0);
    delete image;
    delete Omero;
