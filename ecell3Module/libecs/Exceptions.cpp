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
// modified for stand-alone (2011/01/11)
// String -> std::string (2011/01/11)

#ifdef HAVE_CONFIG_H
#include "ecell_config.h"
#endif /* HAVE_CONFIG_H */

#include "Exceptions.hpp"

// comment out (2011/01/11)
// #include "EcsObject.hpp"
// #include "Model.hpp"

// namespace libecs
// {

// change code (2011/01/11)
Exception::Exception( std::string const& method, std::string const& message )
//Exception::Exception( String const& method, String const& message, EcsObject const* object )
    : theMethod( method ), 
      theMessage( message ),
// comment out (2011/01/11)
//      theEcsObjectHandle( object ? object->getHandle(): Handle::INVALID_HANDLE_VALUE ),
//      theModel( object ? object->getModel(): 0 ),
      theWhatMsg()
{
    ; // do nothing
}

Exception::~Exception() throw()
{
    ; // do nothing
}

// comment out (2011/01/11)
//EcsObject const* Exception::getEcsObject() const throw()
//{
//    EcsObject const* object( 0 );
//
//    if ( theModel && theEcsObjectHandle != Handle::INVALID_HANDLE_VALUE )
//    {
//        try { object = theModel->getObject( theEcsObjectHandle ); }
//        catch ( std::exception const& ) {}
//    }
//
//    return object;
//}

 // comment out (2011/01/11)
//const char* Exception::what() const throw()
//{
//    if (theWhatMsg.empty())
//    {
//        std::string whatMsg( getClassName() );
//        whatMsg += ": " + theMessage;
//        EcsObject const* object( getEcsObject() );
//        if ( object )
//        {
//            whatMsg += " (";
//            whatMsg += object->asString();
//            whatMsg += ")";
//        }
//        theWhatMsg = whatMsg;
//    }
//    return theWhatMsg.c_str();
//}

DEFINE_EXCEPTION_METHOD_BODIES( UnexpectedError );
DEFINE_EXCEPTION_METHOD_BODIES( NotFound );
DEFINE_EXCEPTION_METHOD_BODIES( IOException );
DEFINE_EXCEPTION_METHOD_BODIES( NotImplemented ); 
DEFINE_EXCEPTION_METHOD_BODIES( Instantiation );

DEFINE_EXCEPTION_METHOD_BODIES( AssertionFailed );
DEFINE_EXCEPTION_METHOD_BODIES( AlreadyExist );
DEFINE_EXCEPTION_METHOD_BODIES( ValueError );
DEFINE_EXCEPTION_METHOD_BODIES( TypeError );
DEFINE_EXCEPTION_METHOD_BODIES( OutOfRange );
DEFINE_EXCEPTION_METHOD_BODIES( IllegalOperation );
DEFINE_EXCEPTION_METHOD_BODIES( TooManyItems );

// simulation errors
DEFINE_EXCEPTION_METHOD_BODIES( SimulationError );
DEFINE_EXCEPTION_METHOD_BODIES( InitializationFailed );
DEFINE_EXCEPTION_METHOD_BODIES( RangeError );

// PropertySlot errors
DEFINE_EXCEPTION_METHOD_BODIES( PropertyException );
DEFINE_EXCEPTION_METHOD_BODIES( NoSlot );
DEFINE_EXCEPTION_METHOD_BODIES( AttributeError );

// Introspection errors
DEFINE_EXCEPTION_METHOD_BODIES( NoInfoField );

// FullID errors
DEFINE_EXCEPTION_METHOD_BODIES( BadID ); 
DEFINE_EXCEPTION_METHOD_BODIES( BadSystemPath );
DEFINE_EXCEPTION_METHOD_BODIES( InvalidEntityType );

// } // namespace libecs // comment out (2011/01/11)
