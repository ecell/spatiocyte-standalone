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

#ifndef __FULLID_HPP
#define __FULLID_HPP

#include <vector>
#include <string> // add (2011/02/03)
// #include "Defs.hpp" // comment out (2011/02/03)
// #include "libecs/Defs.hpp" // comment out (2010/12/21)
// #include "libecs/EntityType.hpp" // comment out (2010/12/21)

// namespace libecs // comment out (2010/12/21)
// {

/** 
   SystemPath 
*/
class SystemPath
{
public:
    typedef std::vector< std::string > StringVector; // change code (2011/01/07)
//    typedef std::vector< String > StringVector;
    typedef StringVector::const_iterator const_iterator;
    typedef StringVector::size_type size_type;

public:
    explicit SystemPath( std::string const& systempathstring ) // change code (2011/01/07)
//    explicit SystemPath( String const& systempathstring )
        : isCanonicalized_( true )
    {
        parse( systempathstring );
    }

    SystemPath(): isCanonicalized_( true ) {}

    SystemPath( SystemPath const& that )
        : isCanonicalized_( that.isCanonicalized_  ),
          theComponents( that.theComponents )
    {
        ; // do nothing
    }

    ~SystemPath() {}

    std::string asString() const;// change code (2011/01/07)
//    String asString() const;// change code (2010/12/21)
//    LIBECS_API String asString() const;

    /** @deprecated use asString() instead. */
    std::string getString() const// change code (2011/01/07)
    //String getString() const// change code (2010/12/21)
    //LIBECS_DEPRECATED String getString() const
    {
        return asString();
    }

    bool operator==( SystemPath const& rhs ) const
    {
        return theComponents == rhs.theComponents;
    }

    bool operator!=( SystemPath const& rhs ) const
    {
        return !operator==( rhs );
    }

    bool operator<( SystemPath const& rhs ) const
    {
        return theComponents < rhs.theComponents;
    }

    bool operator>=( SystemPath const& rhs ) const
    {
        return !operator<( rhs );
    }

    bool operator>( SystemPath const& rhs ) const
    {
        return theComponents > rhs.theComponents;
    }

    bool operator<=( SystemPath const& rhs ) const
    {
        return !operator>( rhs );
    }

    SystemPath const& operator=( SystemPath const& rhs )
    {
        theComponents = rhs.theComponents;
        return *this;
    }

    void swap( SystemPath& that )
    {
        theComponents.swap( that.theComponents );
    }

    void push_back( std::string const& aComponent ) // change code (2011/01/07)
//    void push_back( String const& aComponent )
    {
        theComponents.push_back( aComponent );
        if ( aComponent == "." || aComponent == ".." )
        {
            isCanonicalized_ = false;
        }
    }

    void pop_back()
    {
        theComponents.pop_back();
    }

    const_iterator begin() const
    {
        return theComponents.begin();
    }

    const_iterator end() const
    {
        return theComponents.end();
    }

    size_type size() const
    {
        return theComponents.size();
    }

    bool isAbsolute() const
    {
        return ( ! theComponents.empty() &&
                theComponents.front()[0] == DELIMITER )
                || isModel();
    }

    bool isModel() const
    {
        return theComponents.empty();
    }

    void canonicalize();// change code (2010/12/21)
//    LIBECS_API void canonicalize();

    SystemPath toRelative( SystemPath const& aBaseSystemPath ) const;// change code (2010/12/21)
    // LIBECS_API SystemPath toRelative( SystemPath const& aBaseSystemPath ) const;

    bool isValid() const
    {
        // FIXME: check '..'s and '.'s etc..
        return true;
    }

    bool isCanonicalized() const
    {
        return isCanonicalized_;
    }

    operator std::string() const // change code (2011/01/07)
//    operator String() const
    {
        return asString();
    }

private:

    void parse( std::string const& systempathstring );// change code (2011/01/07)
//    void parse( String const& systempathstring );// change code (2010/12/21)
//    LIBECS_API void parse( String const& systempathstring );

private:

    bool isCanonicalized_;
    StringVector theComponents;

public:

    static const char DELIMITER = '/';
};


/**
   FullID is an identifier of a unique Entiy in a cell model.
   The FullID consists of a EntityType, a SystemPath and an ID string.

   @see EntityType, SystemPath
*/
class FullID
{
public:
    FullID()
// Delete because no file in spatiocyte use EntityType (2010/12/21)
//        : theEntityType( EntityType::NONE ),
//          theSystemPath(),
        : theEntityType( "" ),// add (2011/02/03)
          theSystemPath(),
          theID() {}

// add for Entity.cpp (2011/01/06)
    FullID( SystemPath const& systempath,
            std::string const& id ) // change code (2011/01/07)
//            String const& id )
        : theSystemPath( systempath ),
          theID( id )
    {
        ; // do nothing
    }

// Delete because no file in spatiocyte use this function (2010/12/21)
//    FullID( EntityType const& type,
//            SystemPath const& systempath,
//            String const& id )
//        : theEntityType( type ),
//          theSystemPath( systempath ),
//          theID( id )
//    {
//        ; // do nothing
//    }

// Delete because no file in spatiocyte use this function (2010/12/21)
//    explicit FullID( EntityType const& type,
//                     String const& systempathstring,
//                     String const& id )
//        : theEntityType( type ),
//          theSystemPath( systempathstring ),
//          theID( id )
//   {
//        ; // do nothing
//    }

    FullID( std::string const& fullidstring ) // change code (2011/01/07)
//    FullID( String const& fullidstring )
    {
        parse( fullidstring );
    }

// Delete because no file in spatiocyte use this function (2010/12/21)
//    FullID( FullID const& fullid )
//        : theEntityType( fullid.getEntityType() ),
//          theSystemPath( fullid.getSystemPath() ),
//          theID( fullid.getID() )
//    {
//        ; // do nothing
//    }


    ~FullID() {}

    // change code (2011/02/03)
    std::string const& getEntityType() const 
//    EntityType const& getEntityType() const 
    { 
        return theEntityType; 
    }


    SystemPath const& getSystemPath() const
    { 
        return theSystemPath; 
    }


    std::string const& getID() const // change code (2011/01/07)
//    String const& getID() const
    { 
        return theID;
    }

    // change code (2011/02/03)
    void setEntityType( std::string const& type )
//    void setEntityType( EntityType const& type )
    {
        theEntityType = type;
    }


    void setSystemPath( SystemPath const& systempath ) 
    {
        theSystemPath = systempath;
    }


    void setID( std::string const& id ) // change code (2011/01/07)
//    void setID( String const& id ) 
    {
        theID = id;
    }


    bool isValid() const;// change code (2010/12/21)
    // LIBECS_API bool isValid() const;

    std::string asString() const;// change code (2011/01/07)
    //String asString() const;// change code (2010/12/21)
    // LIBECS_API String asString() const;

    /** @deprecated use asString() instead. */
    std::string getString() const// change code (2011/01/07)
    //String getString() const// change code (2010/12/21)
    // LIBECS_DEPRECATED String getString() const
    {
        return asString();
    }

    bool operator<( FullID const& rhs ) const
    {
        // first look at the EntityType
// Delete because no file in spatiocyte use EntityType (2010/12/21)
//        if( getEntityType() != rhs.getEntityType() )
//        {
//            return getEntityType() < rhs.getEntityType();
//        }

        // then compare the SystemPaths
        // FIXME: should be faster is there is SystemPath::compare()
        if( getSystemPath() != rhs.getSystemPath() )
        {
            return getSystemPath() < rhs.getSystemPath();
        }

        // finally compare the ID strings
        return getID() < rhs.getID();
    }

    bool operator==( FullID const& rhs ) const
    {
        // first look at the EntityType
// Delete because no file in spatiocyte use EntityType (2010/12/21)
//        if( getEntityType() != rhs.getEntityType() )
//        {
//            return false;
//        }

        // then compare the SystemPaths
        if( getSystemPath() != rhs.getSystemPath() )
        {
            return false;
        }

        // finally compare the ID strings
        return getID() == rhs.getID();
    }

    bool operator!=( FullID const& rhs ) const
    {
        return ! operator==( rhs );
    }

    operator std::string() const // change code (2011/01/07)
//    operator String() const
    {
        return asString();
    }

protected:
    void parse( std::string const& fullidstring );// change code (2011/01/07)
    //void parse( String const& fullidstring );// change code (2010/12/21)
    // LIBECS_API void parse( String const& fullidstring );

public:
    static const char DELIMITER = ':';

private:
    std::string theEntityType;// add (2011/02/03)
//    EntityType theEntityType;// comment out (2010/12/21)
    SystemPath theSystemPath;
    std::string     theID; // change code (2011/01/07)
//    String     theID;

};

// Delete because no file in spatiocyte use this class (2010/12/21)

//class FullPN
//{
//public:
//    FullPN( const EntityType type, 
//            SystemPath const& systempath,
//            String const& id,
//            String const& propertyname )
//        : theFullID( type, systempath, id ),
//          thePropertyName( propertyname )
//    {
//        ; // do nothing
//    }
//
//    FullPN( FullID const& fullid, String const& propertyname )
//        : theFullID( fullid ),
//          thePropertyName( propertyname )
//    {
//        ; // do nothing
//    }
//
//    FullPN( FullPN const& fullpn )
//        : theFullID( fullpn.getFullID() ),
//          thePropertyName( fullpn.getPropertyName() )
//    {
//        ; // do nothing
//    }
//
//    LIBECS_API FullPN( String const& fullpropertynamestring );
//
//    ~FullPN() 
//    {
//        ; // do nothing
//    }
//
//    FullID const& getFullID() const
//    {
//        return theFullID;
//    }
//
//    const EntityType    getEntityType() const 
//    { 
//        return getFullID().getEntityType(); 
//    }
//
//    SystemPath const& getSystemPath() const
//    { 
//        return getFullID().getSystemPath();
//    }
//
//    String const& getID() const
//    { 
//        return getFullID().getID();
//    }
//
//    String const& getPropertyName() const
//    {
//        return thePropertyName;
//    }
//
//    void setEntityType( const EntityType type )
//    {
//        theFullID.setEntityType( type );
//    }
//
//    void setSystemPath( SystemPath const& systempath ) 
//    {
//        theFullID.setSystemPath( systempath );
//    }
//
//    void setID( String const& id ) 
//    {
//        theFullID.setID( id );
//    }
//
//    void setPropertyName( String const& propertyname )
//    {
//        thePropertyName = propertyname;
//    }
//
//    LIBECS_API String asString() const;
//
//    /** @deprecated use asString() instead. */
//    LIBECS_DEPRECATED String getString() const
//    {
//        return asString();
//    }
//
//    LIBECS_API bool isValid() const;
//
//    bool operator<( FullPN const& rhs ) const
//    {
//        if( getFullID() != rhs.getFullID() )
//        {
//            return getFullID() < rhs.getFullID();
//        }
//
//        return getPropertyName() < rhs.getPropertyName();
//    }
//
//    bool operator==( FullPN const& rhs ) const
//    {
//        if( getFullID() != rhs.getFullID() )
//        {
//            return false;
//        }
//
//        // finally compare the ID strings
//        return getPropertyName() == rhs.getPropertyName();
//    }
//
//    bool operator!=( FullPN const& rhs ) const
//    {
//        return ! operator==( rhs );
//    }
//
//    operator String() const
//    {
//        return asString();
//    }
//
//private:
//    FullID theFullID;
//    String thePropertyName;
//};

// } // namespace libecs // comment out (2010/12/21)

#endif // __FULLID_HPP
