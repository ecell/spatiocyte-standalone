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
// modified for stand-alone (2011/02/01)
// String -> std::string (2011/01/11)

#ifdef HAVE_CONFIG_H
#include "ecell_config.h"
#endif /* HAVE_CONFIG_H */

#include "System.hpp"
#include "FullID.hpp"

#include "Entity.hpp"
#include "Model.hpp"

#include "Exceptions.hpp" // add (2011/01/11)

// namespace libecs // comment out (2011/01/06)
// {

// LIBECS_DM_INIT_STATIC( Entity, Entity ); // comment out (2011/01/06)

Entity::Entity()
    : theSuperSystem( 0 ),
      theID( "" ),
      theName( "" ) // change code (2011/01/06)
//      theName( "" ),
//      theLoggerMap( 0 ) // comment out (2011/01/06)
{

}


Entity::~Entity()
{
    ; // do nothing
}

FullID Entity::getFullID() const
{
    // change code (2011/01/06)
    return FullID( getSystemPath(), getID() );
//    return FullID( getEntityType(), getSystemPath(), getID() );
}

SystemPath Entity::getSystemPath() const
{
    System* aSystem( getSuperSystem() );

    if ( !aSystem )
    {
        THROW_EXCEPTION( IllegalOperation, "no system is associated" );
    }

    if ( aSystem == this )
    {
        return SystemPath();
    }

    SystemPath aSystemPath( aSystem->getSystemPath() );
    aSystemPath.push_back( aSystem->getID() );
    return aSystemPath;
}

// delete (2011/02/01)
// add from EcsObject(2011/01/18)
//char Entity::thePropertyInterface[ sizeof( PropertyInterface< Entity > ) ];
//PropertyInterfaceBase const& Entity::getPropertyInterface() const
//{
//    return *reinterpret_cast< PropertyInterface< Entity >* >( thePropertyInterface );
//}

// Delete because no file in spatiocyte use Logger (2011/01/06)
//LoggerBroker::LoggersPerFullID
//Entity::getLoggers() const
//{
//    LoggerBroker::PerFullIDMap* aLoggerMap(
//        theLoggerMap ? theLoggerMap:
//            &getModel()->getLoggerBroker().theEmptyPerFullIDMap );
//    return LoggerBroker::LoggersPerFullID(
//        LoggerBroker::PerFullIDLoggerIterator(
//            aLoggerMap->begin(),
//            SelectSecond< LoggerBroker::PerFullIDMap::value_type >() ),
//        LoggerBroker::PerFullIDLoggerIterator(
//            aLoggerMap->end(),
//            SelectSecond< LoggerBroker::PerFullIDMap::value_type >() )
//    );
//    // return getModel()->getLoggerBroker().getLoggersByFullID( getFullID() );
//}

// delete (2011/02/01)
//std::string Entity::asString() const
//{
//    return getPropertyInterface().getClassName() + "["
//            + ( theSuperSystem ? getFullID().asString():
//                                 "unbound: " + getID() ) + "]";
//}

// Delete because no file in spatiocyte use Entity::detach (2011/01/06)
//void Entity::detach()
//{
//    if ( theSuperSystem )
//    {
//        if ( theModel )
//        {
//            try { theModel->getLoggerBroker().removeLoggersByFullID( getFullID() ); }
//            catch ( NotFound const& ) {}
//        }
//
//        try { theSuperSystem->unregisterEntity( this ); }
//        catch ( NotFound const& ) {}
//    }
//    theSuperSystem = 0;
//    EcsObject::detach();
//}


// } // namespace libecs // comment out (2011/01/06)
