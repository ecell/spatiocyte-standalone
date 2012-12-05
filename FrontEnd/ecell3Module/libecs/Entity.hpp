//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
//       This file is part of the E-Cell System
//
//       Copyright (C) 1996-2010 Keio University
//       Copyright (C) 2005-2009 The Molecular Sciences Institute
//
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
//
// E-Cell System is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
// 
// E-Cell System is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public
// License along with E-Cell System -- see the file COPYING.
// If not, write to the Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
// 
//END_HEADER
//
// written by Koichi Takahashi <shafi@e-cell.org>,
// E-Cell Project.
//
// modified for stand-alone (2011/02/03)

#ifndef __ENTITY_HPP
#define __ENTITY_HPP

#include "FullID.hpp"// add (2010/12/24)

// comment out (2011/02/03)
// #include "Defs.hpp"// change code (2010/12/24)
// #include "libecs/Defs.hpp"

// comment out (2010/12/24)
// #include "libecs/EntityType.hpp"
// #include "libecs/EcsObject.hpp"

// delete (2011/02/01)
// #include "PropertyInterface.hpp"// change code (2010/12/24)
// #include "libecs/PropertyInterface.hpp"

// comment out (2010/12/24)
// #include "libecs/LoggerBroker.hpp"

// namespace libecs
// {
    class System;

    /**
       Entity class is a base class for all components in the cell model.
    */
    class Entity
    {
//    LIBECS_DM_CLASS( Entity, EcsObject )
//    {
//        friend class LoggerBroker;
    public:

//         LIBECS_DM_OBJECT_ABSTRACT( Entity ) 
//         {
//             INHERIT_PROPERTIES( EcsObject );
//             PROPERTYSLOT_SET_GET( String, Name );
//         }

        Entity(); 
        virtual ~Entity();

        /**
           Get a System to which this Entity belongs.

           @return a borrowed pointer to the super system.
        */
        System* getSuperSystem() const 
        {
            return theSuperSystem;
        }

        /**
           Get a FullID of this Entity.

           @return a FullID of this Entity.
        */
        FullID getFullID() const;

        /**
           Get EntityType of this Entity.

           This method is overridden in Variable, Process and System classes.

           @return EntityType of this Entity object.
           @see EntityType
        */
// Delete because no file in spatiocyte use EntityType (2010/12/24)
//        virtual EntityType getEntityType() const
//        {
//            return EntityType( EntityType::ENTITY );
//        }

        /**
           Get a SystemPath of this Entity.

           @note The SystemPath doesn't include ID of this Entity even if 
           this Entity is a System.

           @return a SystemPath of this Entity.
        */
        virtual SystemPath getSystemPath() const;

        // delete (2011/02/01)
        // add from EcsObject(2010/12/24)
        // virtual PropertyInterfaceBase const& getPropertyInterface() const; 

        /**
           @name Properties
           @{
         */

        /**
           Set an identifier of this Entity.

           @param anID an id of this Entry.
        */
        // change code (2011/01/07)
        void setID( const std::string &value )
//        void setID( const String &value )
        { 
            theID = value;
        }
//         SET_METHOD( String, ID )
//         {
//             theID = value;
//         }


        /**
           Get an id string of this Entity.

           @return an id of this Entity.
        */
        // change code (2011/01/07)
        const std::string getID() const
//        const String getID() const
        { 
            return theID;
        }
//         GET_METHOD( String, ID )
//         {
//             return theID;
//         }


        /**
           Set name of this Entity.

           @param aName a name of this Entity.
        */
        // change code (2011/01/07)
        void setName( const std::string &value )
//        void setName( const String &value )
        { 
            theName = value;
        }
//         SET_METHOD( String, Name )
//         {
//             theName = value;
//         }


        /**
           Get a name of this Entity.

           @return a name of this Entity.
        */
        // change code (2011/01/07)
        const std::string getName() const
//        const String getName() const
        { 
            return theName;
        }
//         GET_METHOD( String, Name )
//         { 
//             return theName; 
//         }


        // delete (2011/02/01)
        /**
           Get a string representation of this Entity as String.

           @return a description string of this Entity.
        */
//        virtual std::string asString() const; // change code (2011/01/07)
//        virtual String asString() const;
        /** @} */


        /**
           Get loggers associated to this Entity
         */
// Delete because no file in spatiocyte use Logger (2010/12/24)
//        LoggerBroker::LoggersPerFullID getLoggers() const;

        /**
           @internal

           Set a supersystem of this Entity.    

           Usually no need to set this manually because a System object does
           this when an Entity is added to the System.

           @param supersystem a pointer to a System to which this object belongs.
        */
        void setSuperSystem( System* supersystem ) 
        { 
            theSuperSystem = supersystem; 
        }

// Delete because no file in spatiocyte use Entity::detach (2011/01/06)
        /**
          Detach this entity from Model
        */
//        virtual void detach();

    protected:
    
// Delete because no file in spatiocyte use Logger (2010/12/24)
//         void setLoggerMap( LoggerBroker::PerFullIDMap* anLoggerMap )
//         {
//             theLoggerMap = anLoggerMap;
//         }

    private:

        // hide them
        Entity( Entity& );
        Entity& operator=( Entity& );

    private:
        System*                        theSuperSystem;
//         LoggerBroker::PerFullIDMap*    theLoggerMap; // comment out (2010/12/24)
        std::string                         theID; // change code (2011/01/07)
        std::string                         theName; // change code (2011/01/07)
//        String                         theID;
//        String                         theName;

        // delete (2011/02/01)
        // add from EcsObject(2011/01/18)
        // static char              thePropertyInterface[ sizeof( PropertyInterface< Entity > ) ];
    };

// } // namespace libecs // change code (2010/12/24)

#endif /* __ENTITY_HPP */
