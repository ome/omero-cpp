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

#ifndef OMERO_MODEL_ELECTRICPOTENTIALI_H
#define OMERO_MODEL_ELECTRICPOTENTIALI_H

#include <omero/IcePortPush.h>
#include <omero/IceNoWarnPush.h>
#include <omero/model/ElectricPotential.h>
#include <omero/model/Units.h>
#include <omero/IceNoWarnPop.h>

#include <omero/conversions.h>

#ifndef OMERO_CLIENT
#   ifdef OMERO_CLIENT_EXPORTS
#       define OMERO_CLIENT ICE_DECLSPEC_EXPORT
#   else
#       define OMERO_CLIENT ICE_DECLSPEC_IMPORT
#   endif
#endif

namespace omero {
  namespace model {
    class ElectricPotentialI;
  }
}

namespace IceInternal {
  OMERO_CLIENT ::Ice::Object* upCast(::omero::model::ElectricPotentialI*);
}

namespace omero {
  namespace model {

    typedef IceInternal::Handle<ElectricPotentialI> ElectricPotentialIPtr;

    class OMERO_CLIENT ElectricPotentialI : virtual public ElectricPotential {

    protected:
        virtual ~ElectricPotentialI(); // protected as outlined in Ice docs.
        static std::map<enums::UnitsElectricPotential,
            std::map<enums::UnitsElectricPotential,
                omero::conversions::ConversionPtr> > CONVERSIONS;
        static std::map<enums::UnitsElectricPotential, std::string> SYMBOLS;

    public:

        static std::string lookupSymbol(enums::UnitsElectricPotential unit) {
            return SYMBOLS[unit];
        }

        ElectricPotentialI();

        ElectricPotentialI(const double& value, const enums::UnitsElectricPotential& unit);

        // Conversion constructor
        ElectricPotentialI(const ElectricPotentialPtr& value, const enums::UnitsElectricPotential& target);

        virtual Ice::Double getValue(
                const Ice::Current& current = Ice::Current());

        virtual void setValue(
                Ice::Double value,
                const Ice::Current& current = Ice::Current());

        virtual enums::UnitsElectricPotential getUnit(
                const Ice::Current& current = Ice::Current());

        virtual void setUnit(
                enums::UnitsElectricPotential unit,
                const Ice::Current& current = Ice::Current());

        virtual std::string getSymbol(
                const Ice::Current& current = Ice::Current());

        virtual ElectricPotentialPtr copy(
                const Ice::Current& = Ice::Current());

    };
  }
}

#include <omero/IcePortPop.h>

#endif // OMERO_MODEL_ELECTRICPOTENTIALI_H

