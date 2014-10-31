
/*
 * Copyright (C) 2014 University of Dundee & Open Microscopy Environment
 * All rights reserved.
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

#ifndef OMERO_MODEL_FREQUENCYI_H
#define OMERO_MODEL_FREQUENCYI_H

#include <omero/model/Frequency.h>
#include <omero/model/UnitsFrequency.h>

#ifndef OMERO_CLIENT
#   ifdef OMERO_CLIENT_EXPORTS
#       define OMERO_CLIENT ICE_DECLSPEC_EXPORT
#   else
#       define OMERO_CLIENT ICE_DECLSPEC_IMPORT
#   endif
#endif

namespace omero {
  namespace model {
    class FrequencyI;
  }
}

namespace IceInternal {
  OMERO_CLIENT ::Ice::Object* upCast(::omero::model::FrequencyI*);
}

namespace omero {
  namespace model {

    typedef IceInternal::Handle<FrequencyI> FrequencyIPtr;

    class OMERO_CLIENT FrequencyI : virtual public Frequency {

    protected:
        virtual ~FrequencyI(); // protected as outlined in Ice docs.

    public:
        FrequencyI();

        virtual Ice::Double getValue(const Ice::Current& current = Ice::Current());

        virtual void setValue(Ice::Double value, const Ice::Current& current = Ice::Current());

        virtual UnitsFrequencyPtr getUnit(const Ice::Current& current = Ice::Current());

        virtual void setUnit(const UnitsFrequencyPtr& time, const Ice::Current& current = Ice::Current());

        virtual FrequencyPtr copy(const Ice::Current& = Ice::Current());

    };
  }
}
#endif // OMERO_MODEL_FREQUENCYI_H
