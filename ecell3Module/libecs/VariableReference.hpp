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

#ifndef __VARIABLEREFERENCE_HPP
#define __VARIABLEREFERENCE_HPP

// change code (2010/12/24)
// #include "Defs.hpp" // comment out (2011/02/03)
#include "Variable.hpp"
// #include "libecs/Defs.hpp"
// #include "libecs/Variable.hpp"
#include <cctype>

// namespace libecs // comment out (2010/12/24)
// {

class Process;

class VariableReference // change code (2010/12/24)
{
// class LIBECS_API VariableReference
// {
    friend class Process;
public:
    class CoefficientLess
    {
    public:
        CoefficientLess()
        {
            ; // do nothing
        }

        bool operator()( VariableReference const& aLhs, 
                         VariableReference const& aRhs ) const
        {
            return compare( aLhs.getCoefficient(), aRhs.getCoefficient() );
        }

        bool operator()( long int aLhs, 
                         VariableReference const& aRhs ) const
        {
            return compare( aLhs, aRhs.getCoefficient() );
        }

        bool operator()( VariableReference const& aLhs, 
                         long int aRhs ) const
        {
            return compare( aLhs.getCoefficient(), aRhs );
        }

    private:
        static bool compare( long int aLhs, long int aRhs )
        {
            return std::less<long int>()( aLhs, aRhs );
        }
    };

    class FullIDLess 
    {
    public:
        FullIDLess()
        {
            ; // do nothing
        }

        bool operator()( VariableReference const& aLhs, 
                         VariableReference const& aRhs ) const
        {
            return compare( aLhs.getFullID(), aRhs.getFullID() );
        }

    private:

        static bool compare( FullID const& aLhs, FullID const& aRhs )
        {
            return std::less<std::string>()( aLhs, aRhs );
        }
    };


    // compare coefficients first, and if equal, compare names.
    class Less
    {
    public:
        Less()
        {
            ; // do nothing
        }

        bool operator()( VariableReference const& aLhs, 
                         VariableReference const& aRhs ) const
        {
            CoefficientLess aCoefficientLess;
            if( aCoefficientLess( aLhs, aRhs ) )
            {
                return true;
            }
            else if( aCoefficientLess( aRhs, aLhs ) )
            {
                return false;
            } 
            else // lhs.coeff == rhs.coeff
            {
                return FullIDLess()( aLhs, aRhs );
            }
        }
    };

public:
    VariableReference()
        : theFullID(),
          theVariable( 0 ),
          theCoefficient( 0 ),
          theIsAccessor( true )
    {
        ; // do nothing
    }

    VariableReference( long int aSerial,
                       FullID const& anFullID,
                       long int aCoefficient,
                       const bool anIsAccessor = true )    
        : theSerial( aSerial ),
          theFullID( anFullID ),
          theVariable( 0 ),
          theCoefficient( aCoefficient ),
          theIsAccessor( anIsAccessor )
    {
        ; // do nothing
    }

    VariableReference( long int aSerial,
                       Variable* aVariable,
                       long int aCoefficient,
                       const bool anIsAccessor = true )    
        : theSerial( aSerial ),
          theFullID(),
          theVariable( aVariable ),
          theCoefficient( aCoefficient ),
          theIsAccessor( anIsAccessor )
    {
        ; // do nothing
    }

    ~VariableReference() {}

    void setName( std::string const& aName )
    {
        theName = aName;
    }

    const std::string getName() const 
    { 
        return theName; 
    }

    const long int getSerial() const
    {
        return theSerial;
    }

    FullID const& getFullID() const
    {
        return theFullID;
    }

    Variable* getVariable() const 
    { 
        return theVariable; 
    }

    void setCoefficient( long int aCoefficient )
    {
        theCoefficient = aCoefficient;
    }

    long int getCoefficient() const 
    { 
        return theCoefficient; 
    }

    bool isMutator() const
    {
        return theCoefficient != 0;
    }

    void setIsAccessor( const bool anIsAccessor )
    {
        theIsAccessor = anIsAccessor;
    }

    bool isAccessor() const
    {
        return theIsAccessor;
    }

    bool isEllipsisName() const
    {
        return isEllipsisNameString( theName );
    }

    long int getEllipsisNumber() const;

    bool isDefaultName() const
    {
        return isDefaultNameString( theName );
    }

    bool operator==( VariableReference const& rhs ) const
    {
        if( theName        == rhs.theName && 
            theVariable == rhs.theVariable &&
            theCoefficient == rhs.theCoefficient &&
            theIsAccessor  == rhs.theIsAccessor )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    static bool isEllipsisNameString( std::string const& aName )
    {
        return aName.size() > 3 && aName.compare( 0, 3, ELLIPSIS_PREFIX ) == 0
               && std::isdigit( *reinterpret_cast< const unsigned char* >(
                    &aName[ 4 ] ) );
    }

    static bool isDefaultNameString( std::string const& aName )
    {
        return aName == DEFAULT_NAME;
    }

    // LIBECS_DEPRECATED // comment out (2011/02/03)
    void setValue( double aValue ) const
    {
        theVariable->setValue( aValue );
    }

    // LIBECS_DEPRECATED // comment out (2011/02/03)
    double getValue() const
    {
        return theVariable->getValue();
    }

    /**
       Add a value to the variable according to the coefficient.
       
       Set a new value to the variable.    
       The new value is: old_value + ( aValue * theCoeffiencnt ).

       @param aValue a Real value to be added.
       @deprecated
    */
    // LIBECS_DEPRECATED // comment out (2011/02/03)
    void addValue( double aValue ) const
    {
        theVariable->addValue( aValue * theCoefficient );
    }

//    LIBECS_DEPRECATED // comment out (2010/12/21)
// Delete because no file in spatiocyte use MolarConc (2010/12/21)
//    Real getMolarConc() const
//    {
//        return theVariable->getMolarConc();
//    }

//    LIBECS_DEPRECATED // comment out (2010/12/21)
// Delete because no file in spatiocyte use NumberConc (2010/12/21)
//    Real getNumberConc() const
//    {
//        return theVariable->getNumberConc();
//    }

//    LIBECS_DEPRECATED // comment out (2010/12/21)
// Delete because no file in spatiocyte use Velocity (2010/12/21)
//    Real getVelocity() const
//    {
//        return theVariable->getVelocity();
//    }

    // LIBECS_DEPRECATED // comment out (2011/02/03)
    bool isFixed() const
    {
        return theVariable->isFixed();
    }

    // LIBECS_DEPRECATED // comment out (2011/02/03)
    void setFixed( const bool aValue ) const
    {
        theVariable->setFixed( aValue );
    }

    // LIBECS_DEPRECATED // comment out (2011/02/03)
    System* getSuperSystem() const
    {
        return theVariable->getSuperSystem();
    }

protected:

    void setSerial( long int anID )
    {
        theSerial = anID;
    }

    void setFullID( FullID const& aFullID )
    {
        theFullID = aFullID;
    }

    void setVariable( Variable* aVariable )
    {
        theVariable = aVariable;
    }


public:
    static const std::string ELLIPSIS_PREFIX;
    static const std::string DEFAULT_NAME;

private:
    long int           theSerial;
    std::string            theName;
    FullID            theFullID;
    Variable*         theVariable;
    long int           theCoefficient;
    bool              theIsAccessor;
};

// } // namespace libecs  // comment out (2010/12/24)

#endif /* __VARIABLEREFERENCE_HPP */

