omero-cpp
=========

C++ tools for interfacing OMERO with various C++ libraries


SimpleOMERO example use:
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
