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
// String -> std::string (2011/01/11)

#ifndef __EXCEPTIONS_HPP
#define __EXCEPTIONS_HPP

#include <stdexcept>

// change code (2011/01/11)
// #include "Defs.hpp"// comment out (2011/02/03)
// #include "libecs/Defs.hpp"
// #include "libecs/Handle.hpp" // comment out (2011/01/11)

// comment out (2011/01/11)
//namespace libecs
//{
//class EcsObject;
//class Model;

// delete OBJ (2011/01/11)
/**
   A macro to throw an exception with the method name and the causal
   @param CLASS   the exception class.
   @param MESSAGE the message attached to the exception.
   @param OBJ     the ECSObject instance where the exception occurred.
 */
#define THROW_EXCEPTION_ECSOBJECT( CLASS, MESSAGE )\
    throw CLASS( __PRETTY_FUNCTION__, MESSAGE )

/**
   A macro to throw an exception with the method name. The causal is "this".
   @param CLASS the exception class.
   @param MESSAGE the message attached to the exception.
 */
#define THROW_EXCEPTION_INSIDE( CLASS, MESSAGE )\
    THROW_EXCEPTION_ECSOBJECT( CLASS, MESSAGE )

/**
   A macro to throw an exception, with method name
   @param CLASS the exception class.
   @param MESSAGE the message attached to the exception.
 */
#define THROW_EXCEPTION( CLASS, MESSAGE )\
    THROW_EXCEPTION_ECSOBJECT( CLASS, MESSAGE )

/// Base exception class
class Exception: public std::exception // change code (2011/01/11)
// class LIBECS_API Exception: public std::exception 
{
public:
    /**
       Constructor.
       @param method the name of the method from where the exception is thrown.
       @param message the description of the exception.
       @param entity the entity where the exception occurred.
     */
    // change code (2011/01/11)
    Exception( std::string const& method, std::string const& message = "" );
//    Exception( String const& method, String const& message = "", EcsObject const* object = 0 );

    virtual ~Exception() throw();

    const std::string& message() const throw()
    {
        return theMessage;
    }

//    virtual const char* what() const throw(); // comment out (2011/01/11)

    virtual const char* getClassName() const throw()
    {
        return "Exception";
    }

    virtual std::string const& getMethod() const throw()
    {
        return theMethod;
    }

//    EcsObject const* getEcsObject() const throw(); // comment out (2011/01/11)

protected:
    const std::string theMethod;
    const std::string theMessage;
// comment out (2011/01/11)
//    Handle const theEcsObjectHandle;
//    Model const* const theModel;
    mutable std::string theWhatMsg;
};

/**
    @internal
 */
// delete EcsObject,LIBECS_API,libecs (2011/01/11)
#define DEFINE_EXCEPTION( CLASSNAME, BASECLASS )\
class CLASSNAME : public BASECLASS\
{\
public:\
    CLASSNAME( std::string const& method, std::string const& message = "" )\
        : BASECLASS( method, message ) {}\
    CLASSNAME( Exception const& orig ) \
        : BASECLASS( orig.getMethod(), orig.message() ) {}\
    virtual ~CLASSNAME() throw();\
    virtual char const* getClassName() const throw();\
};

#define DEFINE_EXCEPTION_METHOD_BODIES( CLASSNAME ) \
    CLASSNAME::~CLASSNAME() throw() {}\
    char const* CLASSNAME::getClassName() const throw() { return #CLASSNAME ; }

// system errors
DEFINE_EXCEPTION( UnexpectedError,                Exception );
DEFINE_EXCEPTION( NotFound,                       Exception );
DEFINE_EXCEPTION( IOException,                    Exception );
DEFINE_EXCEPTION( NotImplemented,                 Exception ); 
DEFINE_EXCEPTION( Instantiation,                  Exception );

DEFINE_EXCEPTION( AssertionFailed,                Exception );
DEFINE_EXCEPTION( AlreadyExist,                   Exception );
DEFINE_EXCEPTION( ValueError,                     Exception );
DEFINE_EXCEPTION( TypeError,                      Exception );
DEFINE_EXCEPTION( OutOfRange,                     Exception );
DEFINE_EXCEPTION( IllegalOperation,               Exception );
DEFINE_EXCEPTION( TooManyItems,                   Exception );

// simulation errors
DEFINE_EXCEPTION( SimulationError,                Exception );
DEFINE_EXCEPTION( InitializationFailed,           SimulationError );
DEFINE_EXCEPTION( RangeError,                     SimulationError );

// PropertySlot errors
DEFINE_EXCEPTION( PropertyException,              Exception );
DEFINE_EXCEPTION( NoSlot,                         PropertyException );
DEFINE_EXCEPTION( AttributeError,                 PropertyException );

// Introspection errors
DEFINE_EXCEPTION( NoInfoField,                    Exception );

// FullID errors
DEFINE_EXCEPTION( BadID,                          Exception ); 
DEFINE_EXCEPTION( BadSystemPath,                  BadID );
DEFINE_EXCEPTION( InvalidEntityType,              BadID);


/**
   This macro throws UnexpectedError exception with a method name.

   Use this macro to indicate where must not be reached.
*/

// delete libecs (2011/01/11)

// delete (2011/02/03)
//#define NEVER_GET_HERE\
//    THROW_EXCEPTION( UnexpectedError, \
//                     "never get here (" + std::string( __PRETTY_FUNCTION__ )\
//                     + ")" )

// } // namespace libecs

#endif /* __EXCEPTIONS_HPP */
