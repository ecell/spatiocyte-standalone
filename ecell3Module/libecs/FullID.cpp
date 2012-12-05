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
// modifying for stand-alone (2011/02/03)
// String -> std::string (2011/01/11)

#ifdef HAVE_CONFIG_H
#include "ecell_config.h"
#endif /* HAVE_CONFIG_H */

#include <string>

// #include "Util.hpp"
// #include "Exceptions.hpp"

#include "FullID.hpp"

#include "Exceptions.hpp" // add (2011/01/11)

// namespace libecs // comment out (2011/01/07)
// {

///////////////////////    SystemPath

// add from Util.cpp (2011/01/07)
void eraseWhiteSpaces( std::string& str )
{
    static const std::string aSpaceCharacters( " \t\n" );

    std::string::size_type p( str.find_first_of( aSpaceCharacters ) );

    while( p != std::string::npos )
    {
        str.erase( p, 1 );
        p = str.find_first_of( aSpaceCharacters, p );
    }
}

void SystemPath::parse( std::string const& systempathstring )
{
    if( systempathstring.empty() )
    {
        return;
    }

    std::string aString( systempathstring );
    eraseWhiteSpaces( aString );
    
    std::string::size_type aCompStart( 0 );
    
    // absolute path ( start with '/' )
    if( aString[ 0 ] == DELIMITER )
    {
        push_back( std::string( 1, DELIMITER ) );
        ++aCompStart;
    }

    for ( std::string::size_type aCompEnd; aCompStart < aString.size();
            aCompStart = aCompEnd + 1 )
    {
        aCompEnd = aString.find_first_of( DELIMITER, aCompStart );
        if ( aCompEnd == std::string::npos )
        {
            aCompEnd = aString.size();
        }

        std::string aComponent( aString.substr( aCompStart, aCompEnd - aCompStart ) );
        if ( aComponent == ".." )
        {
            if ( theComponents.size() == 1 &&
                    theComponents.front()[ 0 ] == DELIMITER )
            {
                THROW_EXCEPTION( BadSystemPath, 
                                 "Too many levels of retraction with .." );
            }
            else if ( theComponents.empty() || theComponents.back() == ".." )
            {
                theComponents.push_back( aComponent );
            }
            else 
            {
                theComponents.pop_back();
            }
        }
        else if ( !aComponent.empty() && aComponent != "." )
        {
            theComponents.push_back( aComponent );
        }
    }

    if ( !aString.empty() && theComponents.empty() )
    {
        theComponents.push_back( "." );
    }
}

std::string SystemPath::asString() const
{
    StringVector::const_iterator i = theComponents.begin();
    std::string aString;

    if( isAbsolute() )
    {
        if( theComponents.size() == 1 )
        {
            return "/";
        }
        else
        {
            ; // do nothing
        }
    }
    else
    {
        // isAbsolute() == false implies that this can be empty
        if( theComponents.empty() )
        {
            return aString;
        }
        else
        {
            aString = *i;
        }
    }

    if( i == theComponents.end() )
    {
        return aString;
    }

    ++i;

    while( i != theComponents.end() )
    {
        aString += '/';
        aString += *i;
        ++i;
    }

    return aString;
}

void SystemPath::canonicalize()
{
    StringVector aNewPathComponents;

    for ( StringVector::const_iterator i( theComponents.begin() );
           i != theComponents.end(); ++i )
    {
        if ( *i == "." )
        {
            continue;
        }
        else if ( *i == ".." )
        {
            if ( aNewPathComponents.empty() )
            {
                break;
            }
            aNewPathComponents.pop_back();
        }
        else
        {
            aNewPathComponents.push_back( *i );
        }
    }

    theComponents.swap( aNewPathComponents );
}

SystemPath SystemPath::toRelative( SystemPath const& aBaseSystemPath ) const
{
    // 1. "" (empty) means Model itself, which is invalid for this method.
    // 2. Not absolute is invalid (not absolute implies not empty).
    if( ! isAbsolute() || isModel() )
    {
        return *this;
    }

    if( ! aBaseSystemPath.isAbsolute() || aBaseSystemPath.isModel() )
    {
        THROW_EXCEPTION( BadSystemPath, 
                         "[" + aBaseSystemPath.asString() +
                         "] is not an absolute SystemPath" );
    }

    SystemPath aThisPathCopy;
    SystemPath const* thisPath;

    if ( !isCanonicalized() )
    {
        aThisPathCopy = *this;
        aThisPathCopy.canonicalize();
        thisPath = &aThisPathCopy;
    }
    else
    {
        thisPath = this;
    }

    SystemPath aBaseSystemPathCopy;
    SystemPath const* aCanonicalizedBaseSystemPath;
    if ( !aBaseSystemPath.isCanonicalized() )
    {
        aCanonicalizedBaseSystemPath = &aBaseSystemPath;
    }
    else
    {
        aBaseSystemPathCopy = aBaseSystemPath;
        aBaseSystemPathCopy.canonicalize();
        aCanonicalizedBaseSystemPath = &aBaseSystemPathCopy;
    }

    SystemPath aRetval;
    StringVector::const_iterator j( thisPath->theComponents.begin() ),
                               je( thisPath->theComponents.end() );
    StringVector::const_iterator
        i( aCanonicalizedBaseSystemPath->theComponents.begin() ),
        ie( aCanonicalizedBaseSystemPath->theComponents.end() );

    while ( i != ie && j != je )
    {
        std::string const& aComp( *i );
        if ( aComp != *j )
        {
            break;
        }
        ++i, ++j;
    }
    if ( i != ie )
    {
        while ( i != ie )
        {
            aRetval.theComponents.push_back( ".." );
            ++i;
        }
    }
    std::copy( j, je, std::back_inserter( aRetval.theComponents ) );

    if ( aRetval.theComponents.empty() )
    {
        aRetval.theComponents.push_back( "." );
    }

    return aRetval; 
}


///////////////// FullID

void FullID::parse( std::string const& fullidstring )
{
    // empty FullID string is invalid
    if( fullidstring == "" )
    {
        THROW_EXCEPTION( BadID, "empty FullID string" );
    }

    std::string aString( fullidstring );
    eraseWhiteSpaces( aString );

    // ignore leading white spaces
    std::string::size_type aFieldStart( 0 );
    std::string::size_type aFieldEnd( aString.find_first_of( DELIMITER, aFieldStart ) );
    if( aFieldEnd == std::string::npos )
    {
        THROW_EXCEPTION( BadID, 
                         "no ':' in the FullID string [" + aString + "]" );
    }

    theEntityType = aString.substr( aFieldStart, aFieldEnd - aFieldStart ); // add (2010/02/03)
//    std::string aTypeString( aString.substr( aFieldStart, aFieldEnd - aFieldStart ) );
//    theEntityType = EntityType( aTypeString ); // comment out (2010/01/11)
    
    aFieldStart = aFieldEnd + 1;
    aFieldEnd = aString.find_first_of( DELIMITER, aFieldStart );
    if( aFieldEnd == std::string::npos )
    {
        THROW_EXCEPTION( BadID, "only one ':' in the FullID string [" 
                                + aString + "]" );
    }

    
    theSystemPath = SystemPath( aString.substr( aFieldStart, 
                                aFieldEnd - aFieldStart ) );
    
    aFieldStart = aFieldEnd + 1;

    // drop trailing string after extra ':'(if this is    FullPN),
    // or go to the end
    aFieldEnd = aString.find_first_of( DELIMITER, aFieldStart );

    theID = aString.substr( aFieldStart, aFieldEnd - aFieldStart );
}

std::string FullID::asString() const
{
    if ( theID.empty() )
    {
        return std::string( "(invalid)" );
    }

    // change code (2011/01/11)
    return FullID::DELIMITER 
//    return theEntityType.asString() + FullID::DELIMITER 
        + theSystemPath.asString() + FullID::DELIMITER + theID;
}

bool FullID::isValid() const
{
    bool aFlag( theSystemPath.isValid() );
    aFlag &= ! theID.empty();

    return aFlag;
}


///////////////// FullPN

// comment out (2011/01/07)
//FullPN::FullPN( String const& fullpropertynamestring )
//    : theFullID( fullpropertynamestring )
//{
//
//    String::size_type aPosition( 0 );
//
//    for( int i( 0 ) ; i < 3 ; ++i )
//    {
//        aPosition = fullpropertynamestring.
//            find_first_of( FullID::DELIMITER, aPosition );
//        if( aPosition == String::npos ) 
//        {
//            THROW_EXCEPTION( BadID, "not enough fields in FullPN string [" +
//                                    fullpropertynamestring + "]" );
//        }
//        ++aPosition;
//    }
//
//    thePropertyName = fullpropertynamestring.substr( aPosition, String::npos );
//    eraseWhiteSpaces( thePropertyName );
//}
//
//String FullPN::asString() const
//{
//    return theFullID.asString() + FullID::DELIMITER + thePropertyName;
//}
//
//bool FullPN::isValid() const
//{
//    return theFullID.isValid() & ! thePropertyName.empty();
//}

// } // namespace libecs // comment out (2011/01/07)
