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
// String -> std::string (2011/01/07)
// Integer -> long int (2011/01/17)
// Real -> double (2011/01/17)
// delete LIBECS_DEPRECATED (2011/02/03)

#ifndef __POLYMORPH_HPP
#define __POLYMORPH_HPP

#include <stdexcept>
#include <algorithm>
#include <cstdlib>
#include <cstddef>
#include <cstring>
#include <cassert>
#include <vector>

#include <boost/static_assert.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/size.hpp>
#include <boost/range/const_iterator.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/less.hpp>
#include <boost/range/size_type.hpp> // add for boost::range_size (2011/01/12)

//#include "Defs.hpp"// comment out (2011/02/03)
//#include "libecs/Defs.hpp"
//#include "libecs/convertTo.hpp"
//#include "libecs/Util.hpp"

//namespace libecs
//{
//
class Polymorph;

typedef std::vector< Polymorph > PolymorphVector;

class PolymorphValue // change code (2011/01/11)
// class LIBECS_API PolymorphValue
{
    friend void intrusive_ptr_add_ref( PolymorphValue* );
    friend void intrusive_ptr_release( PolymorphValue* );

private:
    template< int Nidx_, typename Tlhs_, typename Trhs_, bool Nok_ = boost::mpl::less< boost::mpl::int_< Nidx_ >, boost::mpl::int_< boost::tuples::length< Trhs_ >::value > >::value >
    struct __assignIfNotNull
    {
        void operator()( Tlhs_& lhs, Trhs_ const& rhs )
        {
            lhs = boost::get< Nidx_ >( rhs );
        }
    };

    template< int Nidx_, typename Tlhs_, typename Trhs_ >
    struct __assignIfNotNull< Nidx_, Tlhs_, Trhs_, false >
    {
        void operator()( Tlhs_& lhs, Trhs_ const& rhs )
        {
        }
    };

    template< int Nidx_, typename Tlhs_, typename Trhs_ >
    void assignIfNotNull( Tlhs_& lhs, Trhs_ const& rhs )
    {
        __assignIfNotNull< Nidx_, Tlhs_, Trhs_ >()( lhs, rhs );
    }

public:
    enum Type
    {
        NONE     = 0,
        REAL     = 1,
        INTEGER  = 2,
        STRING   = 3,
        TUPLE    = 4
    };

    typedef boost::intrusive_ptr< PolymorphValue > Handle;

    class Tuple
    {
        friend class PolymorphValue;

    public:
        typedef std::size_t size_type;
        typedef Polymorph value_type;

    public:
        size_type size() const
        {
            return theNumberOfItems;
        }

        template<typename Trange>
        bool operator==( Trange const& rhs ) const;

        template< typename Trange >
        bool operator!=( Trange const& rhs ) const
        {
            return !operator==( rhs );
        }

        template< typename Trange >
        bool operator<( Trange const& rhs ) const;

        template< typename Trange >
        bool operator>=( Trange const& rhs ) const
        {
            return !operator<( rhs );
        }

        template< typename Trange >
        bool operator>( Trange const& rhs ) const;

        template< typename Trange >
        bool operator<=( Trange const& rhs ) const
        {
            return !operator<( rhs );
        }

        operator value_type*();

        operator value_type const*() const;

    private:
        size_type theNumberOfItems;
        char ptr[1];
    };

    class RawString
    {
        friend class PolymorphValue;
    public:
        typedef std::size_t size_type;
        typedef std::string::value_type value_type;

        size_type size() const
        {
            return theLength;
        }

        bool operator==( RawString const& rhs ) const
        {
            return theLength == rhs.theLength &&
                0 == memcmp( ptr, rhs.ptr, sizeof( value_type ) * theLength );
        }

        bool operator==( const char* rhs ) const
        {
            return theLength == std::strlen( rhs ) &&
                0 == memcmp( ptr, rhs, sizeof( value_type ) * theLength );
        }

        bool operator==( std::string const& rhs ) const
        {
            return 0 == rhs.compare( 0, theLength, ptr );
        }

        template< typename T_ >
        bool operator!=( T_ const& rhs ) const
        {
            return !operator==( rhs );
        }

        bool operator<( RawString const& rhs ) const
        {
            size_type const l( std::min( theLength, rhs.theLength ) );
            std::pair< value_type const*, value_type const* > c(
                    std::mismatch( ptr, ptr + l, rhs.ptr ) );
            return ( c.first == ptr + theLength &&
                     c.second != rhs.ptr + rhs.theLength)
                    || ( c.first != ptr + theLength &&
                         c.second != rhs.ptr + rhs.theLength &&
                         *c.first < *c.second );
        }

        bool operator<( const char* rhs ) const
        {
            size_type const rl( std::strlen( rhs ) );
            size_type const l( std::min( theLength, rl ) );
            std::pair< value_type const*, value_type const* > c(
                    std::mismatch( ptr, ptr + l, rhs ) );
            return ( c.first == ptr + theLength && c.second != rhs + rl )
                    || ( c.first != ptr + theLength &&
                         c.second != rhs + rl && *c.first < *c.second );
        }

        bool operator<( std::string const& rhs ) const
        {
            return rhs.compare( 0, theLength, ptr ) > 0;
        }

        template< typename T_ >
        bool operator>=( T_ const& rhs ) const
        {
            return !operator<( rhs );
        }

        bool operator>( RawString const& rhs ) const
        {
            size_type const l( std::min( theLength, rhs.theLength ) );
            std::pair< value_type const*, value_type const* > c(
                    std::mismatch( ptr, ptr + l, rhs.ptr ) );
            return ( c.first != ptr + theLength &&
                     c.second == rhs.ptr + rhs.theLength )
                    || ( c.first != ptr + theLength &&
                         c.second != rhs.ptr + rhs.theLength &&
                         *c.first > *c.second );
        }

        bool operator>( const char* rhs ) const
        {
            size_type const rl( std::strlen( rhs ) );
            size_type const l( std::min( theLength, rl ) );
            std::pair< value_type const*, value_type const* > c(
                    std::mismatch( ptr, ptr + l, rhs ) );
            return ( c.first != ptr + theLength && c.second == rhs + rl )
                    || ( c.first != ptr + theLength &&
                         c.second != rhs + rl && *c.first > *c.second );
        }

        bool operator>( std::string const& rhs ) const
        {
            return rhs.compare( 0, theLength, ptr ) < 0;
        }

        template< typename T_ >
        bool operator<=( T_ const& rhs ) const
        {
            return !operator>( rhs );
        }

        operator value_type const*() const
        {
            return reinterpret_cast< value_type const* >( ptr );
        }

        operator value_type*()
        {
            return reinterpret_cast< value_type* >( ptr );
        }

    private:
        size_type theLength;
        value_type ptr[1];
    };

public:

    ~PolymorphValue() {}

    enum Type getType() const
    {
        return theType;
    }

    PolymorphVector asPolymorphVector() const;

    template< typename T >
    T as() const
    {
// temporary comment out (2010/12/28)
//        class CONVERSION_IS_NOT_FEASIBLE;
//        sizeof( CONVERSION_IS_NOT_FEASIBLE );
    }

    static Handle create()
    {
        return new PolymorphValue();
    }

    static Handle create( long int aValue )
    {
        return new PolymorphValue( aValue );
    }

    static Handle create( double aValue )
    {
        return new PolymorphValue( aValue );
    }

    static Handle create( const char* sptr,
                          std::size_t sz = static_cast< std::size_t >( -1 ) )
    {
        if ( sz == static_cast< std::size_t >( -1 ) )
        {
            sz = std::strlen( sptr );
        }
        assert( sizeof( PolymorphValue ) + sz + 1 >= sizeof( PolymorphValue ) );
        void* ptr( std::malloc( sizeof( PolymorphValue ) + sz + 1) );
        if ( !ptr )
        {
            throw std::bad_alloc();
        }

        return new(ptr) PolymorphValue( sptr, sz );
    }

    static Handle create( std::string const& aValue )
    {
        assert( aValue.size() + 1 >= 1 && sizeof( PolymorphValue ) + aValue.size() + 1 >= sizeof( PolymorphValue ) );
        void* ptr( std::malloc( sizeof( PolymorphValue ) + aValue.size() + 1) );
        if ( !ptr )
        {
            throw std::bad_alloc();
        }

        return new(ptr) PolymorphValue( aValue );
    }

    template< typename T0_, typename T1_, typename T2_, typename T3_,
              typename T4_, typename T5_, typename T6_, typename T7_,
              typename T8_, typename T9_ >
    static Handle create( boost::tuple< T0_, T1_, T2_, T3_, T4_,
                                        T5_, T6_, T7_, T8_, T9_ > const& aTuple );

    template< typename Trange_ >
    static Handle create( Trange_ const& aValue );

    bool operator==( double const& rhs ) const;

    bool operator==( long int const& rhs ) const;

    bool operator==( RawString const& rhs ) const;

    bool operator==( Tuple const& rhs ) const;

    bool operator==( const char* rhs ) const;

    bool operator==( std::string const& rhs ) const;

    bool operator==( PolymorphVector const& rhs ) const;

    bool operator==( PolymorphValue const& rhs ) const;

    template< typename T_ >
    bool operator!=( T_ const& rhs ) const
    {
        return !operator==( rhs );
    }

    bool operator<( double const& rhs ) const;

    bool operator<( long int const& rhs ) const;

    bool operator<( RawString const& rhs ) const;

    bool operator<( Tuple const& rhs ) const;

    bool operator<( const char* rhs ) const;

    bool operator<( std::string const& rhs ) const;

    bool operator<( PolymorphVector const& rhs ) const;

    bool operator<( PolymorphValue const& rhs ) const;

    template< typename T_ >
    bool operator>=( T_ const& rhs ) const
    {
        return !operator<( rhs );
    }

    bool operator>( double const& rhs ) const;

    bool operator>( long int const& rhs ) const;

    bool operator>( RawString const& rhs ) const;

    bool operator>( Tuple const& rhs ) const;

    bool operator>( const char* rhs ) const;

    bool operator>( std::string const& rhs ) const;

    bool operator>( PolymorphVector const& rhs ) const;

    bool operator>( PolymorphValue const& rhs ) const;

    template< typename T_ >
    bool operator<=( T_ const& rhs ) const
    {
        return !operator>( rhs );
    }

    operator std::string() const;

    operator PolymorphVector() const;

    operator PolymorphValue::RawString const& () const;

    operator PolymorphValue::Tuple const& () const;

protected:
    explicit PolymorphValue()
        : theType( NONE ), theRefCount( 0 ) {}

    explicit PolymorphValue( long int aValue )
        : theType( INTEGER ), theRefCount( 0 ), theIntegerValue( aValue ) {}

    explicit PolymorphValue( double aValue )
        : theType( REAL ), theRefCount( 0 ), theRealValue( aValue ) {}

    explicit PolymorphValue( const char* sptr, std::size_t sz )
        : theType( STRING ), theRefCount( 0 )
    {
        theStringValue.theLength = sz;
        std::memcpy( theStringValue.ptr, sptr, sz );
        theStringValue.ptr[ sz ] = '\0';
    }

    explicit PolymorphValue( std::string const& aValue )
        : theType( STRING ), theRefCount( 0 )
    {
        theStringValue.theLength = aValue.size();
        std::memcpy( theStringValue.ptr, aValue.data(), aValue.size() );
        theStringValue.ptr[ aValue.size() ] = '\0';
    }

    template< typename T0_, typename T1_, typename T2_, typename T3_,
              typename T4_, typename T5_, typename T6_, typename T7_,
              typename T8_, typename T9_ >
    explicit PolymorphValue( boost::tuple< T0_, T1_, T2_, T3_, T4_, T5_, T6_, T7_, T8_, T9_ > const& aTuple );

    template< typename Trange_ >
    explicit PolymorphValue( Trange_ const& r );

private:
    PolymorphValue( const PolymorphValue& that ) {}
    PolymorphValue const& operator= ( PolymorphValue const& )
    {
        return *this;
    }

private:
    Type theType;
    int theRefCount;

    // must be the last element of the class.
    union {
        long int theIntegerValue;
        double theRealValue;
        RawString theStringValue;
        Tuple theTupleValue;
    };
};

inline void intrusive_ptr_add_ref( PolymorphValue* p )
{
    ++p->theRefCount;
}

inline void intrusive_ptr_release( PolymorphValue* p )
{
    if ( --p->theRefCount <= 0 )
    {
        switch ( p->theType )
        {
        case PolymorphValue::NONE:
        case PolymorphValue::INTEGER:
        case PolymorphValue::REAL:
            delete p;
            break;
        case PolymorphValue::STRING:
        case PolymorphValue::TUPLE:
            std::free(p);
            break;
        }
    }
}

//} // namespace libecs
//
//namespace boost
//{
//    using libecs::intrusive_ptr_add_ref;
//    using libecs::intrusive_ptr_release;
//}
//
//namespace libecs
//{

class Polymorph
// class LIBECS_API Polymorph
{
    friend class PolymorphValue;

public:
    typedef PolymorphValue::Type Type;

public:

    Polymorph():
        theValue( PolymorphValue::create() ) { }

    Polymorph( Polymorph const& aValue )
        : theValue( aValue.theValue ) { } 

    explicit Polymorph( PolymorphValue::Handle aHandle )
        : theValue( aHandle ) { }

    explicit Polymorph( std::string const& aValue ) 
        : theValue( PolymorphValue::create( aValue ) ) { }

    explicit Polymorph( const char* ptr,
               std::size_t sz = static_cast< std::size_t >( -1 ) ) 
        : theValue( PolymorphValue::create( ptr, sz ) ) { }

    explicit Polymorph( double aValue )            
        : theValue( PolymorphValue::create( aValue ) ) { }

    explicit Polymorph( long int aValue )            
        : theValue( PolymorphValue::create( aValue ) ) { }

    template< typename Trange_ >
    explicit Polymorph( Trange_ const& aValue )
        : theValue( PolymorphValue::create( aValue ) ) { }

    template< typename T0_, typename T1_, typename T2_, typename T3_,
              typename T4_, typename T5_, typename T6_, typename T7_,
              typename T8_, typename T9_ >
    Polymorph( boost::tuple< T0_, T1_, T2_, T3_, T4_, T5_, T6_, T7_, T8_, T9_> const& that ): theValue( PolymorphValue::create( that ) ) { }

    ~Polymorph() { }

    template< typename T >
    T as() const
    {
        return theValue->as<T>();
    }

    Type getType() const
    {
        return theValue->getType();
    }

    /** @deprecated */
    std::string asString() const;
//    LIBECS_DEPRECATED std::string asString() const;

    /** @deprecated */
    double   asReal() const;
//    LIBECS_DEPRECATED double   asReal() const;
  
    /** @deprecated */
    long int asInteger() const;
//    LIBECS_DEPRECATED long int asInteger() const;

    /** @deprecated */
    PolymorphVector asPolymorphVector() const;
//    LIBECS_DEPRECATED PolymorphVector asPolymorphVector() const;

    Polymorph const& operator=( Polymorph const& rhs )
    {
        if( this != &rhs )
        {
            theValue = rhs.theValue;
        }
    
        return *this;
    }

    template< typename T_ >
    Polymorph const& operator=( T_ const& rhs )
    {
        theValue = PolymorphValue::create( rhs );
        return *this;
    }

    operator PolymorphValue&()
    {
        return *theValue;
    }

    operator PolymorphValue const&() const
    {
        return *theValue;
    }

protected:

    PolymorphValue::Handle theValue;

};

template<>
inline Polymorph Polymorph::as() const
{
    return *this;
}

inline bool operator==( double lhs, PolymorphValue const& rhs )
{
    return rhs.operator==( lhs );
}

inline bool operator==( long int lhs, PolymorphValue const& rhs )
{
    return rhs.operator==( lhs );
}

inline bool operator==( PolymorphValue::RawString const& lhs, PolymorphValue const& rhs )
{
    return rhs.operator==( lhs );
}

inline bool operator==( PolymorphValue::Tuple& lhs, PolymorphValue const& rhs )
{
    return rhs.operator==( lhs );
}

inline bool operator==( const char* lhs, PolymorphValue const& rhs )
{
    return rhs.operator==( lhs );
}

inline bool operator==( std::string const& lhs, PolymorphValue const& rhs )
{
    return rhs.operator==( lhs );
}

inline bool operator==( PolymorphVector const& lhs, PolymorphValue const& rhs )
{
    return rhs.operator==( lhs );
}

inline bool operator==( Polymorph const& lhs, PolymorphValue const& rhs )
{
    return static_cast< PolymorphValue const& >( lhs ).operator==( rhs );
}

inline bool operator!=( double lhs, PolymorphValue const& rhs )
{
    return rhs.operator!=( lhs );
}

inline bool operator!=( long int lhs, PolymorphValue const& rhs )
{
    return rhs.operator!=( lhs );
}

inline bool operator!=( PolymorphValue::RawString const& lhs, PolymorphValue const& rhs )
{
    return rhs.operator!=( lhs );
}

inline bool operator!=( PolymorphValue::Tuple& lhs, PolymorphValue const& rhs )
{
    return rhs.operator!=( lhs );
}

inline bool operator!=( const char* lhs, PolymorphValue const& rhs )
{
    return rhs.operator!=( lhs );
}

inline bool operator!=( std::string const& lhs, PolymorphValue const& rhs )
{
    return rhs.operator!=( lhs );
}

inline bool operator!=( PolymorphVector const& lhs, PolymorphValue const& rhs )
{
    return rhs.operator!=( lhs );
}

inline bool operator!=( Polymorph const& lhs, PolymorphValue const& rhs )
{
    return static_cast< PolymorphValue const& >( lhs ).operator!=( rhs );
}

inline bool operator<( double lhs, PolymorphValue const& rhs )
{
    return rhs.operator>( lhs );
}

inline bool operator<( long int lhs, PolymorphValue const& rhs )
{
    return rhs.operator>( lhs );
}

inline bool operator<( PolymorphValue::RawString const& lhs, PolymorphValue const& rhs )
{
    return rhs.operator>( lhs );
}

inline bool operator<( PolymorphValue::Tuple& lhs, PolymorphValue const& rhs )
{
    return rhs.operator>( lhs );
}

inline bool operator<( const char* lhs, PolymorphValue const& rhs )
{
    return rhs.operator>( lhs );
}

inline bool operator<( std::string const& lhs, PolymorphValue const& rhs )
{
    return rhs.operator>( lhs );
}

inline bool operator<( PolymorphVector const& lhs, PolymorphValue const& rhs )
{
    return rhs.operator>( lhs );
}

inline bool operator<( Polymorph const& lhs, PolymorphValue const& rhs )
{
    return static_cast< PolymorphValue const& >( lhs ).operator<( rhs );
}

inline bool operator>=( double lhs, PolymorphValue const& rhs )
{
    return rhs.operator<=( lhs );
}

inline bool operator>=( long int lhs, PolymorphValue const& rhs )
{
    return rhs.operator<=( lhs );
}

inline bool operator>=( PolymorphValue::RawString const& lhs, PolymorphValue const& rhs )
{
    return rhs.operator<=( lhs );
}

inline bool operator>=( PolymorphValue::Tuple& lhs, PolymorphValue const& rhs )
{
    return rhs.operator<=( lhs );
}

inline bool operator>=( const char* lhs, PolymorphValue const& rhs )
{
    return rhs.operator<=( lhs );
}

inline bool operator>=( std::string const& lhs, PolymorphValue const& rhs )
{
    return rhs.operator<=( lhs );
}

inline bool operator>=( PolymorphVector const& lhs, PolymorphValue const& rhs )
{
    return rhs.operator<=( lhs );
}

inline bool operator>=( Polymorph const& lhs, PolymorphValue const& rhs )
{
    return static_cast< PolymorphValue const & >( lhs ).operator>=( rhs );
}

inline bool operator>( double lhs, PolymorphValue const& rhs )
{
    return rhs.operator<( lhs );
}

inline bool operator>( long int lhs, PolymorphValue const& rhs )
{
    return rhs.operator<( lhs );
}

inline bool operator>( PolymorphValue::RawString const& lhs, PolymorphValue const& rhs )
{
    return rhs.operator<( lhs );
}

inline bool operator>( PolymorphValue::Tuple& lhs, PolymorphValue const& rhs )
{
    return rhs.operator<( lhs );
}

inline bool operator>( const char* lhs, PolymorphValue const& rhs )
{
    return rhs.operator<( lhs );
}

inline bool operator>( std::string const& lhs, PolymorphValue const& rhs )
{
    return rhs.operator<( lhs );
}

inline bool operator>( PolymorphVector const& lhs, PolymorphValue const& rhs )
{
    return rhs.operator<( lhs );
}

inline bool operator>( Polymorph const& lhs, PolymorphValue const& rhs )
{
    return static_cast< PolymorphValue const& >( lhs ).operator>( rhs );
}

inline bool operator<=( double lhs, PolymorphValue const& rhs )
{
    return rhs.operator>=( lhs );
}

inline bool operator<=( long int lhs, PolymorphValue const& rhs )
{
    return rhs.operator>=( lhs );
}

inline bool operator<=( PolymorphValue::RawString const& lhs, PolymorphValue const& rhs )
{
    return rhs.operator>=( lhs );
}

inline bool operator<=( PolymorphValue::Tuple& lhs, PolymorphValue const& rhs )
{
    return rhs.operator>=( lhs );
}

inline bool operator<=( const char* lhs, PolymorphValue const& rhs )
{
    return rhs.operator>=( lhs );
}

inline bool operator<=( std::string const& lhs, PolymorphValue const& rhs )
{
    return rhs.operator>=( lhs );
}

inline bool operator<=( PolymorphVector const& lhs, PolymorphValue const& rhs )
{
    return rhs.operator>=( lhs );
}

inline bool operator<=( Polymorph const& lhs, PolymorphValue const& rhs )
{
    return static_cast< PolymorphValue const & >( lhs ).operator<=( rhs );
}

// ---------------------------------------------------------------------------
inline bool operator==( double lhs, Polymorph const& rhs )
{
    return lhs == static_cast< PolymorphValue const& >( rhs );
}

inline bool operator==( long int lhs, Polymorph const& rhs )
{
    return lhs == static_cast< PolymorphValue const& >( rhs );
}

inline bool operator==( PolymorphValue::RawString const& lhs, Polymorph const& rhs )
{
    return lhs == static_cast< PolymorphValue const& >( rhs );
}

inline bool operator==( PolymorphValue::Tuple& lhs, Polymorph const& rhs )
{
    return lhs == static_cast< PolymorphValue const& >( rhs );
}

inline bool operator==( const char* lhs, Polymorph const& rhs )
{
    return lhs == static_cast< PolymorphValue const& >( rhs );
}

inline bool operator==( std::string const& lhs, Polymorph const& rhs )
{
    return lhs == static_cast< PolymorphValue const& >( rhs );
}

inline bool operator==( PolymorphVector const& lhs, Polymorph const& rhs )
{
    return lhs == static_cast< PolymorphValue const& >( rhs );
}

inline bool operator==( Polymorph const& lhs, Polymorph const& rhs )
{
    return lhs == static_cast< PolymorphValue const& >( rhs );
}

inline bool operator!=( double lhs, Polymorph const& rhs )
{
    return lhs != static_cast< PolymorphValue const& >( rhs );
}

inline bool operator!=( long int lhs, Polymorph const& rhs )
{
    return lhs != static_cast< PolymorphValue const& >( rhs );
}

inline bool operator!=( PolymorphValue::RawString const& lhs, Polymorph const& rhs )
{
    return lhs != static_cast< PolymorphValue const& >( rhs );
}

inline bool operator!=( PolymorphValue::Tuple& lhs, Polymorph const& rhs )
{
    return lhs != static_cast< PolymorphValue const& >( rhs );
}

inline bool operator!=( const char* lhs, Polymorph const& rhs )
{
    return lhs != static_cast< PolymorphValue const& >( rhs );
}

inline bool operator!=( std::string const& lhs, Polymorph const& rhs )
{
    return lhs != static_cast< PolymorphValue const& >( rhs );
}

inline bool operator!=( PolymorphVector const& lhs, Polymorph const& rhs )
{
    return lhs != static_cast< PolymorphValue const& >( rhs );
}

inline bool operator!=( Polymorph const& lhs, Polymorph const& rhs )
{
    return lhs != static_cast< PolymorphValue const& >( rhs );
}

inline bool operator<( double lhs, Polymorph const& rhs )
{
    return lhs < static_cast< PolymorphValue const& >( rhs );
}

inline bool operator<( long int lhs, Polymorph const& rhs )
{
    return lhs < static_cast< PolymorphValue const& >( rhs );
}

inline bool operator<( PolymorphValue::RawString const& lhs, Polymorph const& rhs )
{
    return lhs < static_cast< PolymorphValue const& >( rhs );
}

inline bool operator<( PolymorphValue::Tuple& lhs, Polymorph const& rhs )
{
    return lhs < static_cast< PolymorphValue const& >( rhs );
}

inline bool operator<( const char* lhs, Polymorph const& rhs )
{
    return lhs < static_cast< PolymorphValue const& >( rhs );
}

inline bool operator<( std::string const& lhs, Polymorph const& rhs )
{
    return lhs < static_cast< PolymorphValue const& >( rhs );
}

inline bool operator<( PolymorphVector const& lhs, Polymorph const& rhs )
{
    return lhs < static_cast< PolymorphValue const& >( rhs );
}

inline bool operator<( Polymorph const& lhs, Polymorph const& rhs )
{
    return lhs < static_cast< PolymorphValue const& >( rhs );
}

inline bool operator>=( double lhs, Polymorph const& rhs )
{
    return lhs >= static_cast< PolymorphValue const& >( rhs );
}

inline bool operator>=( long int lhs, Polymorph const& rhs )
{
    return lhs >= static_cast< PolymorphValue const& >( rhs );
}

inline bool operator>=( PolymorphValue::RawString const& lhs, Polymorph const& rhs )
{
    return lhs >= static_cast< PolymorphValue const& >( rhs );
}

inline bool operator>=( PolymorphValue::Tuple& lhs, Polymorph const& rhs )
{
    return lhs >= static_cast< PolymorphValue const& >( rhs );
}

inline bool operator>=( const char* lhs, Polymorph const& rhs )
{
    return lhs >= static_cast< PolymorphValue const& >( rhs );
}

inline bool operator>=( std::string const& lhs, Polymorph const& rhs )
{
    return lhs >= static_cast< PolymorphValue const& >( rhs );
}

inline bool operator>=( PolymorphVector const& lhs, Polymorph const& rhs )
{
    return lhs >= static_cast< PolymorphValue const& >( rhs );
}

inline bool operator>=( Polymorph const& lhs, Polymorph const& rhs )
{
    return lhs >= static_cast< PolymorphValue const& >( rhs );
}

inline bool operator>( double lhs, Polymorph const& rhs )
{
    return lhs > static_cast< PolymorphValue const& >( rhs );
}

inline bool operator>( long int lhs, Polymorph const& rhs )
{
    return lhs > static_cast< PolymorphValue const& >( rhs );
}

inline bool operator>( PolymorphValue::RawString const& lhs, Polymorph const& rhs )
{
    return lhs > static_cast< PolymorphValue const& >( rhs );
}

inline bool operator>( PolymorphValue::Tuple& lhs, Polymorph const& rhs )
{
    return lhs > static_cast< PolymorphValue const& >( rhs );
}

inline bool operator>( const char* lhs, Polymorph const& rhs )
{
    return lhs > static_cast< PolymorphValue const& >( rhs );
}

inline bool operator>( std::string const& lhs, Polymorph const& rhs )
{
    return lhs > static_cast< PolymorphValue const& >( rhs );
}

inline bool operator>( PolymorphVector const& lhs, Polymorph const& rhs )
{
    return lhs > static_cast< PolymorphValue const& >( rhs );
}

inline bool operator>( Polymorph const& lhs, Polymorph const& rhs )
{
    return lhs > static_cast< PolymorphValue const& >( rhs );
}

inline bool operator<=( double lhs, Polymorph const& rhs )
{
    return lhs <= static_cast< PolymorphValue const& >( rhs );
}

inline bool operator<=( long int lhs, Polymorph const& rhs )
{
    return lhs <= static_cast< PolymorphValue const& >( rhs );
}

inline bool operator<=( PolymorphValue::RawString const& lhs, Polymorph const& rhs )
{
    return lhs <= static_cast< PolymorphValue const& >( rhs );
}

inline bool operator<=( PolymorphValue::Tuple& lhs, Polymorph const& rhs )
{
    return lhs <= static_cast< PolymorphValue const& >( rhs );
}

inline bool operator<=( const char* lhs, Polymorph const& rhs )
{
    return lhs <= static_cast< PolymorphValue const& >( rhs );
}

inline bool operator<=( std::string const& lhs, Polymorph const& rhs )
{
    return lhs <= static_cast< PolymorphValue const& >( rhs );
}

inline bool operator<=( PolymorphVector const& lhs, Polymorph const& rhs )
{
    return lhs <= static_cast< PolymorphValue const& >( rhs );
}

template< typename T_>
inline bool operator==( Polymorph const& lhs, T_ rhs )
{
    return static_cast< PolymorphValue const& >( lhs ) == rhs;
}

template< typename T_>
inline bool operator!=( Polymorph const& lhs, T_ rhs )
{
    return static_cast< PolymorphValue const& >( lhs ) != rhs;
}

template< typename T_>
inline bool operator<( Polymorph const& lhs, T_ rhs )
{
    return static_cast< PolymorphValue const& >( lhs ) < rhs;
}

template< typename T_>
inline bool operator>=( Polymorph const& lhs, T_ rhs )
{
    return static_cast< PolymorphValue const& >( lhs ) >= rhs;
}

template< typename T_>
inline bool operator>( Polymorph const& lhs, T_ rhs )
{
    return static_cast< PolymorphValue const& >( lhs ) > rhs;
}

template< typename T_>
inline bool operator<=( Polymorph const& lhs, T_ rhs )
{
    return static_cast< PolymorphValue const& >( lhs ) <= rhs;
}

// comment out (2010/12/28)
//} // namespace libecs
//
namespace boost
{
template<>
struct range_iterator< PolymorphValue::Tuple >
{
    typedef PolymorphValue::Tuple::value_type* type;
};

template<>
struct range_const_iterator< PolymorphValue::Tuple >
{
    typedef PolymorphValue::Tuple::value_type const* type;
};

template<>
inline range_iterator< PolymorphValue::Tuple >::type
begin< PolymorphValue::Tuple >( PolymorphValue::Tuple& r )
{
    return static_cast< Polymorph* >( r );
}

template<>
inline range_const_iterator< PolymorphValue::Tuple >::type
begin< PolymorphValue::Tuple >( PolymorphValue::Tuple const& r )
{
    return static_cast< Polymorph const* >( r );
}

template<>
inline range_iterator< PolymorphValue::Tuple >::type
end< PolymorphValue::Tuple >(
        PolymorphValue::Tuple& r )
{
    return static_cast< Polymorph* >( r ) + r.size();
}

template<>
inline range_const_iterator< PolymorphValue::Tuple >::type
end< PolymorphValue::Tuple >(
        PolymorphValue::Tuple const& r )
{
    return static_cast< Polymorph const* >( r ) + r.size();
}

} // namespace boost
//
//namespace libecs
//{
//
template< typename Trange >
inline bool PolymorphValue::Tuple::operator==( Trange const& rhs ) const
{
    if ( theNumberOfItems != rhs.size() )
        return false;
    return std::equal( boost::begin( *this ), boost::end( *this ),
                       boost::begin( rhs ) );
}

template< typename Trange >
inline bool PolymorphValue::Tuple::operator<( Trange const& rhs ) const
{
    std::pair< Polymorph const*,
               typename boost::range_const_iterator< Trange >::type > c(
        std::mismatch( boost::begin( *this ),
                       boost::begin( *this )
                           + std::min( theNumberOfItems, rhs.size() ),
                       boost::begin( rhs ) ) );
    return ( c.first == boost::end( *this ) && c.second != boost::end( rhs ) )
            || ( c.first != boost::end( *this ) &&
                 c.second != boost::end( rhs ) &&
                 *c.first < *c.second );
}


template< typename Trange >
inline bool PolymorphValue::Tuple::operator>( Trange const& rhs ) const
{
    std::pair< Polymorph const*,
               typename boost::range_const_iterator< Trange >::type > c(
        std::mismatch( boost::begin( *this ),
                       boost::begin( *this )
                           + std::min( theNumberOfItems, rhs.size() ),
                       boost::begin( rhs ) ) );
    return ( c.first != boost::end( *this ) && c.second == boost::end( rhs ) )
            || ( c.first != boost::end( *this ) &&
                 c.second != boost::end( rhs ) &&
                 *c.first > *c.second );
}

inline PolymorphValue::Tuple::operator Polymorph*()
{
    return reinterpret_cast< Polymorph* >( ptr );
}

inline PolymorphValue::Tuple::operator Polymorph const*() const
{
    return reinterpret_cast< Polymorph const* >( ptr );
}


//template <>
//inline String PolymorphValue::as() const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return "";
//    case INTEGER:
//        return convertTo< String >( theIntegerValue );
//    case REAL:
//        return convertTo< String >( theRealValue );
//    case STRING:
//        return String( theStringValue.ptr, theStringValue.theLength );
//    case TUPLE:
//        {
//            checkSequenceSize( theTupleValue, 1 );
//            return theTupleValue[ 0 ].as< String >();
//        }
//    }
//    NEVER_GET_HERE;
//}
//
//
//template <>
//inline Real PolymorphValue::as() const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return 0.0;
//    case INTEGER:
//        return convertTo< Real >( theIntegerValue );
//    case REAL:
//        return theRealValue;
//    case STRING:
//        return convertTo< Real >( as< String >() );
//    case TUPLE:
//        {
//            checkSequenceSize( theTupleValue, 1 );
//            return theTupleValue[ 0 ].as< Real >();
//        }
//    }
//    NEVER_GET_HERE;
//}
//
//
//template <>
//inline Integer PolymorphValue::as() const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return 0;
//    case INTEGER:
//        return theIntegerValue;
//    case REAL:
//        return convertTo< Integer >( theRealValue );
//    case STRING:
//        return convertTo< Integer >( as< String >() );
//    case TUPLE:
//        {
//            checkSequenceSize( theTupleValue, 1 );
//            return theTupleValue[ 0 ].as< Integer >();
//        }
//    }
//    NEVER_GET_HERE;
//}
//
//
//template <>
//inline PolymorphVector PolymorphValue::as() const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return PolymorphVector();
//    case TUPLE:
//        return PolymorphVector( boost::begin( theTupleValue ), boost::end( theTupleValue ) );
//    default:
//        return PolymorphVector( 1, Polymorph( PolymorphValue::Handle( const_cast< PolymorphValue * >( this ) ) ) );
//    }
//}
//
//
//template <>
//inline PolymorphValue::RawString const& PolymorphValue::as() const
//{
//    if ( theType != STRING )
//        THROW_EXCEPTION( TypeError, "conversion not feasible" );
//
//    return theStringValue;
//}
//
//
//template <>
//inline PolymorphValue::Tuple const& PolymorphValue::as() const
//{
//    if ( theType != TUPLE )
//        THROW_EXCEPTION( TypeError, "conversion not feasible" );
//
//    return theTupleValue;
//}
//

template< typename T0_, typename T1_, typename T2_, typename T3_, typename T4_,
          typename T5_, typename T6_, typename T7_, typename T8_, typename T9_ >
inline PolymorphValue::PolymorphValue(
        boost::tuple< T0_, T1_, T2_, T3_, T4_, T5_, T6_, T7_, T8_, T9_ > const& aTuple )
    : theType( TUPLE ), theRefCount( 0 )
{
    typedef boost::tuple< T0_, T1_, T2_, T3_, T4_, T5_, T6_, T7_, T8_, T9_ > arg_type;
    const int numberOfItems( boost::tuples::length< arg_type >::value );
    theTupleValue.theNumberOfItems = numberOfItems;

    assignIfNotNull< 0 >( theTupleValue[ 0 ], aTuple );
    assignIfNotNull< 1 >( theTupleValue[ 1 ], aTuple );
    assignIfNotNull< 2 >( theTupleValue[ 2 ], aTuple );
    assignIfNotNull< 3 >( theTupleValue[ 3 ], aTuple );
    assignIfNotNull< 4 >( theTupleValue[ 4 ], aTuple );
    assignIfNotNull< 5 >( theTupleValue[ 5 ], aTuple );
    assignIfNotNull< 6 >( theTupleValue[ 6 ], aTuple );
    assignIfNotNull< 7 >( theTupleValue[ 7 ], aTuple );
    assignIfNotNull< 8 >( theTupleValue[ 8 ], aTuple );
    assignIfNotNull< 9 >( theTupleValue[ 9 ], aTuple );
}


template< typename T0_, typename T1_, typename T2_, typename T3_, typename T4_,
          typename T5_, typename T6_, typename T7_, typename T8_, typename T9_ >
PolymorphValue::Handle PolymorphValue::create( boost::tuple< T0_, T1_, T2_, T3_, T4_, T5_, T6_, T7_, T8_, T9_ > const& aTuple )
{
    typedef boost::tuple< T0_, T1_, T2_, T3_, T4_, T5_, T6_, T7_, T8_, T9_ > arg_type;
    const std::size_t numberOfItems( boost::tuples::length< arg_type >::value );
    std::size_t pbytes( sizeof( Polymorph ) * numberOfItems );
    assert( pbytes / sizeof( Polymorph ) == numberOfItems &&
            sizeof( PolymorphValue ) + pbytes >= sizeof( PolymorphValue ));
    void* ptr( std::malloc( sizeof( PolymorphValue ) + pbytes ) );
    if ( !ptr )
    {
        throw std::bad_alloc();
    }

    // The following is a fast alternative of
    // for ( i = 0; i < boost::size( aValue ); ++i )
    // {
    //   new(static_cast< PolymorphValue* >( ptr )->theTupleValue[ i ]) Polymorph();
    // }
    std::memset( static_cast< PolymorphValue* >( ptr )->theTupleValue,
                 0, pbytes );

    return new(ptr) PolymorphValue( aTuple );
}


template< typename Trange_ >
inline PolymorphValue::PolymorphValue( Trange_ const& r )
    : theType( TUPLE ), theRefCount( 0 )
{
    typedef typename boost::range_const_iterator< Trange_ >::type const_iterator;

    theTupleValue.theNumberOfItems = boost::size( r );

    std::copy( boost::begin( r ), boost::end( r ), boost::begin( theTupleValue ) );
}

template< typename Trange_ >
inline PolymorphValue::Handle PolymorphValue::create( Trange_ const& aValue )
{
    typename boost::range_size< Trange_ >::type numberOfItems( boost::size( aValue ) );
    std::size_t pbytes( sizeof( Polymorph ) * numberOfItems );
    assert( pbytes / sizeof( Polymorph ) == numberOfItems &&
            sizeof( PolymorphValue ) + pbytes >= sizeof( PolymorphValue ));
    void* ptr( std::malloc( sizeof( PolymorphValue ) + pbytes ) );
    if ( !ptr )
    {
        throw std::bad_alloc();
    }

    // The following is a fast alternative of
    // for ( i = 0; i < boost::size( aValue ); ++i )
    // {
    //   new(static_cast< PolymorphValue* >( ptr )->theTupleValue[ i ]) Polymorph();
    // }
    std::memset( static_cast< PolymorphValue* >( ptr )->theTupleValue,
                 0, pbytes );

    return new(ptr) PolymorphValue( aValue );
} 
//
//inline bool PolymorphValue::operator==( Real const& rhs ) const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return false;
//    case REAL:
//        return theRealValue == rhs;
//    case INTEGER:
//        return static_cast< Real >( theIntegerValue ) == rhs;
//    case STRING:
//        return false;
//    case TUPLE: 
//        return theTupleValue.size() == 1 &&
//            static_cast< PolymorphValue const& >( theTupleValue[ 0 ] ) == rhs;
//    }
//    NEVER_GET_HERE;
//}
//
//inline bool PolymorphValue::operator==( Integer const& rhs ) const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return false;
//    case REAL:
//        return theRealValue == static_cast< Real>( rhs );
//    case INTEGER:
//        return theIntegerValue == rhs;
//    case STRING:
//        return false;
//    case TUPLE: 
//        return theTupleValue.size() == 1 &&
//            static_cast< PolymorphValue const& >( theTupleValue[ 0 ] ) == rhs;
//    }
//    NEVER_GET_HERE;
//}
//
//inline bool PolymorphValue::operator==( RawString const& rhs ) const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return false;
//    case REAL:
//        return false;
//    case INTEGER:
//        return false;
//    case STRING:
//        return theStringValue == rhs;
//    case TUPLE:
//        // XXX: check if this works without static_cast later...
//        return theTupleValue.size() == 1 &&
//            static_cast< PolymorphValue const& >( theTupleValue[ 0 ] ) == rhs;
//    }
//    NEVER_GET_HERE;
//}
//
//inline bool PolymorphValue::operator==( Tuple const& rhs ) const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return false;
//    case TUPLE:
//        return theTupleValue == rhs;
//    case REAL:
//        return rhs.size() == 1 && theRealValue == rhs[ 0 ];
//    default:
//         return false;
//    }
//    NEVER_GET_HERE;
//}
//
//inline bool PolymorphValue::operator==( const char* rhs ) const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return false;
//    case REAL:
//        return false;
//    case INTEGER:
//        return false;
//    case STRING:
//        return theStringValue == rhs;
//    case TUPLE:
//        // XXX: check if this works without static_cast later...
//        return theTupleValue.size() == 1 &&
//            static_cast< PolymorphValue const& >( theTupleValue[ 0 ] ) == rhs;
//    }
//    NEVER_GET_HERE;
//}
//
//inline bool PolymorphValue::operator==( String const& rhs ) const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return false;
//    case REAL:
//        return false;
//    case INTEGER:
//        return false;
//    case STRING:
//        return rhs.compare(0, theStringValue.size(), theStringValue ) == 0;
//    case TUPLE:
//        return theTupleValue.size() == 1 &&
//            static_cast< PolymorphValue const& >( theTupleValue[ 0 ] ) == rhs;
//    }
//    NEVER_GET_HERE;
//}
//
//inline bool PolymorphValue::operator==( PolymorphValue const& rhs ) const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return rhs.getType() == NONE;
//    case REAL:
//        return rhs == theRealValue;
//    case INTEGER:
//        return rhs == theIntegerValue;
//    case STRING:
//        return rhs == theStringValue;
//    case TUPLE:
//        return rhs == theTupleValue;
//    }
//    NEVER_GET_HERE;
//}
//
//inline bool PolymorphValue::operator==( PolymorphVector const& rhs ) const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return false;
//    case TUPLE:
//        return theTupleValue == rhs;
//    case REAL:
//        return rhs.size() == 1 && theRealValue == rhs[ 0 ];
//    case INTEGER:
//        return rhs.size() == 1 && theIntegerValue == rhs[ 0 ];
//    case STRING:
//        return rhs.size() == 1 && theStringValue == rhs[ 0 ];
//    default:
//         return false;
//    }
//    NEVER_GET_HERE;
//}
//
//inline bool PolymorphValue::operator<( Real const& rhs ) const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return true;
//    case REAL:
//        return theRealValue < rhs;
//    case INTEGER:
//        return static_cast< Real >( theIntegerValue ) < rhs;
//    case STRING:
//        return false;
//    case TUPLE: 
//        return theTupleValue.size() == 0
//                || ( theTupleValue.size() == 1 &&
//                 static_cast< PolymorphValue const& >( theTupleValue[ 0 ] ) < rhs);
//    }
//    NEVER_GET_HERE;
//}
//
//inline bool PolymorphValue::operator<( Integer const& rhs ) const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return true;
//    case REAL:
//        return theRealValue < static_cast< Real >( rhs );
//    case INTEGER:
//        return theIntegerValue < rhs;
//    case STRING:
//        return false;
//    case TUPLE: 
//        return theTupleValue.size() == 0
//                || ( theTupleValue.size() == 1 &&
//                 static_cast< PolymorphValue const& >( theTupleValue[ 0 ] ) < rhs);
//    }
//    NEVER_GET_HERE;
//}
//
//inline bool PolymorphValue::operator<( RawString const& rhs ) const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return true;
//    case REAL:
//        return true;
//    case INTEGER:
//        return true;
//    case STRING:
//        return theStringValue < rhs;
//    case TUPLE:
//        return theTupleValue.size() == 0
//                || ( theTupleValue.size() == 1 &&
//                 static_cast< PolymorphValue const& >( theTupleValue[ 0 ] ) < rhs);
//    }
//    NEVER_GET_HERE;
//}
//
//inline bool PolymorphValue::operator<( Tuple const& rhs ) const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return true;
//    case REAL:
//        return ( rhs.size() == 1 && theRealValue < static_cast< PolymorphValue const& >( rhs[ 0 ] ) )
//                 || ( rhs.size() > 1 && theRealValue <= static_cast< PolymorphValue const& >( rhs[ 0 ] ) );
//    case INTEGER:
//        return ( rhs.size() == 1 && theIntegerValue < static_cast< PolymorphValue const& >( rhs[ 0 ] ) )
//                 || ( rhs.size() > 1 && theIntegerValue <= static_cast< PolymorphValue const& >( rhs[ 0 ] ) );
//    case STRING:
//        return ( rhs.size() == 1 && theStringValue < static_cast< PolymorphValue const& >( rhs[ 0 ] ) )
//                 || ( rhs.size() > 1 && theStringValue <= static_cast< PolymorphValue const& >( rhs[ 0 ] ) );
//    case TUPLE:
//        return theTupleValue < rhs;
//    }
//    NEVER_GET_HERE;
//}
//
//inline bool PolymorphValue::operator<( const char* rhs ) const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return true;
//    case REAL:
//        return true;
//    case INTEGER:
//        return true;
//    case STRING:
//        return theStringValue < rhs;
//    case TUPLE:
//        return theTupleValue.size() == 0
//                || ( theTupleValue.size() == 1 &&
//                 static_cast< PolymorphValue const& >( theTupleValue[ 0 ] ) < rhs);
//    }
//    NEVER_GET_HERE;
//}
//
//inline bool PolymorphValue::operator<( String const& rhs ) const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return true;
//    case REAL:
//        return true;
//    case INTEGER:
//        return true;
//    case STRING:
//        return theStringValue < rhs;
//    case TUPLE:
//        return theTupleValue.size() == 0
//                || ( theTupleValue.size() == 1 &&
//                 static_cast< PolymorphValue const& >( theTupleValue[ 0 ] ) < rhs);
//    }
//    NEVER_GET_HERE;
//}
//
//inline bool PolymorphValue::operator<( PolymorphVector const& rhs ) const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return true;
//    case REAL:
//        return ( rhs.size() == 1 && theRealValue < static_cast< PolymorphValue const& >( rhs[ 0 ] ) )
//                 || ( rhs.size() > 1 && theRealValue <= static_cast< PolymorphValue const& >( rhs[ 0 ] ) );
//    case INTEGER:
//        return ( rhs.size() == 1 && theIntegerValue < static_cast< PolymorphValue const& >( rhs[ 0 ] ) )
//                 || ( rhs.size() > 1 && theIntegerValue <= static_cast< PolymorphValue const& >( rhs[ 0 ] ) );
//    case STRING:
//        return ( rhs.size() == 1 && theStringValue < static_cast< PolymorphValue const& >( rhs[ 0 ] ) )
//                 || ( rhs.size() > 1 && theStringValue <= static_cast< PolymorphValue const& >( rhs[ 0 ] ) );
//    case TUPLE:
//        return theTupleValue < rhs;
//    }
//    NEVER_GET_HERE;
//}
//
//inline bool PolymorphValue::operator<( PolymorphValue const& rhs ) const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return rhs.getType() != NONE;
//    case REAL:
//        return rhs > theRealValue;
//    case INTEGER:
//        return rhs > theIntegerValue;
//    case STRING:
//        return rhs > theStringValue;
//    case TUPLE:
//        return rhs > theTupleValue;
//    }
//    NEVER_GET_HERE;
//}
//
//inline bool PolymorphValue::operator>( Real const& rhs ) const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return false;
//    case REAL:
//        return theRealValue > rhs;
//    case INTEGER:
//        return static_cast< Real >( theIntegerValue ) > rhs;
//    case STRING:
//        return true;
//    case TUPLE: 
//        return ( theTupleValue.size() == 1 &&
//                 static_cast< PolymorphValue const& >( theTupleValue[ 0 ] ) > rhs )
//                || ( theTupleValue.size() > 1 &&
//                 static_cast< PolymorphValue const& >( theTupleValue[ 0 ] ) >= rhs);
//    }
//    NEVER_GET_HERE;
//}
//
//inline bool PolymorphValue::operator>( Integer const& rhs ) const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return false;
//    case REAL:
//        return theRealValue > static_cast< Real >( rhs );
//    case INTEGER:
//        return theIntegerValue > rhs;
//    case STRING:
//        return true;
//    case TUPLE: 
//        return ( theTupleValue.size() == 1 &&
//                 static_cast< PolymorphValue const& >( theTupleValue[ 0 ] ) > rhs )
//                || ( theTupleValue.size() > 1 &&
//                 static_cast< PolymorphValue const& >( theTupleValue[ 0 ] ) >= rhs);
//    }
//    NEVER_GET_HERE;
//}
//
//inline bool PolymorphValue::operator>( RawString const& rhs ) const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return false;
//    case REAL:
//        return false;
//    case INTEGER:
//        return false;
//    case STRING:
//        return theStringValue > rhs;
//    case TUPLE:
//        return ( theTupleValue.size() == 1 &&
//                 static_cast< PolymorphValue const& >( theTupleValue[ 0 ] ) > rhs )
//                || ( theTupleValue.size() > 1 &&
//                 static_cast< PolymorphValue const& >( theTupleValue[ 0 ] ) >= rhs);
//    }
//    NEVER_GET_HERE;
//}
//
//inline bool PolymorphValue::operator>( Tuple const& rhs ) const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return false;
//    case REAL:
//        return rhs.size() == 0
//                || ( rhs.size() == 1 && static_cast< PolymorphValue const& >( rhs[ 0 ] ) <= theRealValue );
//    case INTEGER:
//        return rhs.size() == 0
//                || ( rhs.size() >= 1 && static_cast< PolymorphValue const& >( rhs[ 0 ] ) <= theIntegerValue );
//    case STRING:
//        return rhs.size() == 0
//                || ( rhs.size() >= 1 && static_cast< PolymorphValue const& >( rhs[ 0 ] ) <= theStringValue );
//    case TUPLE:
//        return theTupleValue > rhs;
//    }
//    NEVER_GET_HERE;
//}
//
//inline bool PolymorphValue::operator>( const char* rhs ) const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return false;
//    case REAL:
//        return false;
//    case INTEGER:
//        return false;
//    case STRING:
//        return theStringValue > rhs;
//    case TUPLE:
//        return ( theTupleValue.size() == 1 &&
//                 static_cast< PolymorphValue const& >( theTupleValue[ 0 ] ) > rhs )
//                || ( theTupleValue.size() > 1 &&
//                 static_cast< PolymorphValue const& >( theTupleValue[ 0 ] ) >= rhs);
//    }
//    NEVER_GET_HERE;
//}
//
//inline bool PolymorphValue::operator>( String const& rhs ) const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return false;
//    case REAL:
//        return false;
//    case INTEGER:
//        return false;
//    case STRING:
//        return theStringValue > rhs;
//    case TUPLE:
//        return ( theTupleValue.size() == 1 &&
//                 static_cast< PolymorphValue const& >( theTupleValue[ 0 ] ) > rhs )
//                || ( theTupleValue.size() > 1 &&
//                 static_cast< PolymorphValue const& >( theTupleValue[ 0 ] ) >= rhs);
//    }
//    NEVER_GET_HERE;
//}
//
//inline bool PolymorphValue::operator>( PolymorphVector const& rhs ) const
//{
//    switch ( theType )
//    {
//    case NONE:
//        return false;
//    case REAL:
//        return rhs.size() == 0
//                || ( rhs.size() == 1 && static_cast< PolymorphValue const& >( rhs[ 0 ] ) <= theRealValue );
//    case INTEGER:
//        return rhs.size() == 0
//                || ( rhs.size() >= 1 && static_cast< PolymorphValue const& >( rhs[ 0 ] ) <= theIntegerValue );
//    case STRING:
//        return rhs.size() == 0
//                || ( rhs.size() >= 1 && static_cast< PolymorphValue const& >( rhs[ 0 ] ) <= theStringValue );
//    case TUPLE:
//        return theTupleValue > rhs;
//    }
//    NEVER_GET_HERE;
//}
//
//inline bool PolymorphValue::operator>( PolymorphValue const& rhs ) const
//{
//    switch ( rhs.getType() )
//    {
//    case NONE:
//        return false;
//    case REAL:
//        return operator>( rhs.as< Real >() );
//    case INTEGER:
//        return operator>( rhs.as< Integer >() );
//    case STRING:
//        return operator>( rhs.as< RawString const& >() );
//    case TUPLE:
//        return operator>( rhs.as< Tuple const& >() );
//    }
//    NEVER_GET_HERE;
//}
//
//
//inline PolymorphValue::operator String() const
//{
//    return as<String>();
//}
//
//inline PolymorphValue::operator PolymorphVector() const
//{
//    return as<PolymorphVector>();
//}
//
//inline PolymorphValue::operator PolymorphValue::RawString const& () const
//{
//    return as<PolymorphValue::RawString const& >();
//}
//
//inline PolymorphValue::operator PolymorphValue::Tuple const& () const
//{
//    return as<PolymorphValue::Tuple const& >();
//}
//
//
//template< typename Tnew_ >
//inline Tnew_ convertTo( const Polymorph& aValue )
//{
//    return aValue.as< Tnew_ >();
//}
//
//
////
//// nullValue() specialization for Polymorph. See Util.hpp
////
//
//template<>
//inline Polymorph nullValue()
//{
//    return Polymorph();
//}
//
//
//inline String Polymorph::asString() const
//{ 
//    return as<String>(); 
//}
//
//inline Real   Polymorph::asReal() const
//{ 
//    return as<Real>(); 
//}
//  
//inline Integer Polymorph::asInteger() const
//{ 
//    return as<Integer>();
//}
//
//inline PolymorphVector Polymorph::asPolymorphVector() const
//{ 
//    return as<PolymorphVector>();
//}
//
//} // namespace libecs
//
//#include <ostream>
//
//namespace std {
//
//template< typename T1_, typename T2_ >
//inline std::basic_ostream< T1_, T2_ >&
//operator<<( std::basic_ostream< T1_, T2_ >& s,
//            libecs::Polymorph const& aValue )
//{
//    switch ( aValue.getType() )
//    {
//    case libecs::PolymorphValue::REAL:
//        s << aValue.as< libecs::Real >();
//        break;
//    case libecs::PolymorphValue::INTEGER:
//        s << aValue.as< libecs::Integer >();
//        break;
//    case libecs::PolymorphValue::TUPLE:
//        {
//            s << '[';
//            libecs::PolymorphValue::Tuple const& aTuple( aValue.as< libecs::PolymorphValue::Tuple const& >() );
//
//            for ( boost::range_const_iterator< libecs::PolymorphValue::Tuple >::type i( boost::begin( aTuple ) ); i != boost::end( aTuple ); ++i )
//            {
//                if ( i != boost::begin( aTuple ) )
//                    s << ", ";
//                s << *i;
//            }
//            s << ']';
//        }
//        break;
//    case libecs::PolymorphValue::STRING:
//        s << '"';
//        s << aValue.as< libecs::String >();
//        s << '"';
//    }
//    return s;
//}
//
//} // namespace std

#endif /* __POLYMORPH_HPP */
