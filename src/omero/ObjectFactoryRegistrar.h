/*
 *   $Id$
 *
 *   Copyright 2007 Glencoe Software, Inc. All rights reserved.
 *   Use is subject to license terms supplied in LICENSE.txt
 *
 */

#ifndef OMERO_OBJECTFACTORYREGISTRAR_H
#define OMERO_OBJECTFACTORYREGISTRAR_H

#include <string>
#include <IceNoWarnPush.h>
#include <Ice/Ice.h>
#include <IceNoWarnPop.h>
#include <IceUtil/IceUtil.h>

namespace omero {

    void conditionalAdd(const std::string& name, const Ice::CommunicatorPtr& ic, const Ice::ObjectFactoryPtr& of);

}

#endif // OMERO_OBJECTFACTORYREGISTRAR_H
