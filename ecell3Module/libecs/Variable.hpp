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
// Integer -> long int (2011/01/17)
// Real -> double (2011/01/17)

#ifndef __VARIABLE_HPP
#define __VARIABLE_HPP

#include <utility>
#include <iostream>
#include <boost/call_traits.hpp>// add (2011/02/03)

// change code (2010/12/24)
// #include "Defs.hpp"// comment out (2011/02/03)
#include "Entity.hpp"
// #include "libecs/Defs.hpp"
// #include "libecs/Entity.hpp"
// #include "libecs/Interpolant.hpp" // comment out (2010/12/24)

// change code (2010/12/24)
#include "System.hpp"
// #include "libecs/System.hpp"

// namespace libecs
// {
/**
   Variable class represents state variables in the simulation model, such as
   amounts of molecular species in a compartment.
*/
class Variable : public Entity // change code (2010/12/24)
{
// LIBECS_DM_CLASS( Variable, Entity )
// {
// protected: // comment out (2010/12/24)
//     typedef std::vector< Interpolant* > InterpolantVector;

public:

// comment out (2010/12/24)
//     LIBECS_DM_BASECLASS( Variable );
// 
//     LIBECS_DM_OBJECT( Variable, Variable )
//     {
//         INHERIT_PROPERTIES( Entity );
//         
//         PROPERTYSLOT_LOAD_SAVE( Real, Value,
//                                 &Variable::setValue,
//                                 &Variable::getValue,
//                                 &Variable::loadValue,
//                                 &Variable::saveValue );
// 
// 
//         PROPERTYSLOT_SET_GET( Real,    DiffusionCoeff );
// 
//         PROPERTYSLOT_SET_GET( Integer,    Fixed );
// 
//         PROPERTYSLOT_NO_LOAD_SAVE( Real, Velocity,
//                                    NOMETHOD,
//                                    &Variable::getVelocity );
// 
//         PROPERTYSLOT_LOAD_SAVE( Real, MolarConc,
//                                 &Variable::setMolarConc,
//                                 &Variable::getMolarConc,
//                                 &Variable::loadMolarConc,
//                                 NOMETHOD );
// 
//         PROPERTYSLOT_NO_LOAD_SAVE( Real, NumberConc,
//                                    &Variable::setNumberConc,
//                                    &Variable::getNumberConc );
//     }

    Variable();
    virtual ~Variable();

// Delete because no file in spatiocyte use this function. (2011/01/12)
//    virtual void dispose();

// comment out (2010/12/24)
//     virtual EntityType getEntityType() const
//     {
//         return EntityType( EntityType::VARIABLE );
//     }

    /**
       Pre-initializes this variable. 
    */
    virtual void preinitialize();

    /**
       Initializes this variable. 
    */
// Delete because no file in spatiocyte use this function. (2011/01/12)
//    virtual void initialize();

    /**
       Clear theVelocity by zero.
    */
// comment out (2010/12/24)
//     virtual const bool isIntegrationNeeded() const
//     {
//         return ! theInterpolantVector.empty();
//     }

    /** 
       Integrate.
    */
// Delete because no file in spatiocyte use this function. (2010/12/21)
//    void integrate( Real aTime )
//    {
//        if( isFixed() == false ) 
//        {
//            updateValue( aTime );
//        }
//        else 
//        {
//            theLastTime = aTime;
//        }
//    }

    /**
       This method is used internally by DifferentialStepper.
       @internal
    */

// Delete because no file in spatiocyte use this function. (2010/12/21)
//    void interIntegrate( Real aCurrentTime )
//    {
//        const Real anInterval( aCurrentTime - theLastTime );
//        
//        if ( anInterval > 0.0 )
//        {
//            Real aVelocitySum( calculateDifferenceSum( aCurrentTime,
//                                                       anInterval ) );
//            setValue( getValue() + aVelocitySum );
//        }
//    }


    /**
       This simply sets the value of this Variable if getFixed() is false.

       @see getFixed()
    */

    // change code (2010/12/24)
    virtual void setValue( const double &value )
    { 
        if ( !isFixed() ) 
        {
            loadValue( value ); 
        }
    }
//     virtual SET_METHOD( Real, Value )
//     { 
//         if ( !isFixed() ) 
//         {
//             loadValue( value ); 
//         }
//     }


    // Currently this is non-virtual, but will be changed to a 
    // virtual function, perhaps in version 3.3.
    // virtual
    // change code (2010/12/24)
    const double getValue() const
    { 
        return saveValue();
    }
//     GET_METHOD( Real, Value )
//     { 
//         return saveValue();
//     }


    void addValue( double aValue )
    {
        setValue( getValue() + aValue );
    }

    // change code (2011/02/03)
    void loadValue( boost::call_traits<double>::param_type aValue )
//    void loadValue( Param<double>::type aValue )
    {
        theValue = aValue;
    }


    double saveValue() const
    {
        return theValue;
    }

    /**
       @return current velocity value in (number of molecules)/(step)
    */
// Delete because no file in spatiocyte use Velocity. (2010/12/21)
//    GET_METHOD( Real, Velocity )
//    {
//        Real aVelocitySum( 0.0 );
//        FOR_ALL( InterpolantVector, theInterpolantVector )
//        {
//            Interpolant const* anInterpolantPtr( *i );
//            aVelocitySum += anInterpolantPtr->getVelocity( theLastTime );
//        }
//
//        return aVelocitySum;
//    }

    /**
       A wrapper to set Fixed property by a bool value.
    */
    void setFixed( const bool aValue )
    {
        theFixed = aValue;
    }

    /**
       @return true if the Variable is fixed or false if not.
    */
    const bool isFixed() const
    {
        return theFixed;
    }


    // wrappers to expose is/setFixed as PropertySlots 
    // change code (2010/12/21)
    void setFixed( const long int &value )
    { 
        theFixed = value != 0;
    }
//    SET_METHOD( Integer, Fixed )
//    { 
//        theFixed = value != 0;
//    }

    // change code (2010/12/21)
    const long int getFixed() const
    { 
        return theFixed;
    }
//    GET_METHOD( Integer, Fixed )
//    { 
//        return theFixed;
//    }

    // change code (2010/12/21)
    void setDiffusionCoeff( const double &value )
    { 
        theDiffusionCoeff = value;
    }
//    SET_METHOD( Real, DiffusionCoeff )
//    { 
//        theDiffusionCoeff = value;
//    }

    // change code (2010/12/21)
    const double getDiffusionCoeff() const
    { 
        return theDiffusionCoeff;
    }
//    GET_METHOD( Real, DiffusionCoeff )
//    { 
//        return theDiffusionCoeff;
//    }

    void loadNumberConc( const double &value ); // add (2011/01/11)

    /**
       Get a string representation of this Variable as String.

       @return a description string of this Variable.

       add from Entity (2011/02/01)
    */
    virtual std::string asString() const;

    /**
       Set the Variable name (2011/02/01)
       ( usually "Variable" )
    */
    void setVariableName(const std::string &value)
    {
        theVariableName = value;
    }

    /**
       Get the Variable name (2011/02/01)
       ( usually "Variable" )
    */
    const std::string getVariableName() const
    {
        return theVariableName;
    }

    /**
       Returns the molar concentration of this Variable.

       @return Concentration in M [mol/L].
    */
// Delete because no file in spatiocyte use MolarConc and NumberConc. (2010/12/21)
//    GET_METHOD( Real, MolarConc )
//    {
//        // N_A_R = 1.0 / N_A
//        return getNumberConc() * N_A_R;
//    }

    /**
       Set the molar concentration of this Variable.

       @param value Concentration in M [mol/L].
    */
// Delete because no file in spatiocyte use MolarConc and NumberConc. (2010/12/21)
//    SET_METHOD( Real, MolarConc )
//    {
//        setNumberConc( value * N_A );
//    }

    /**
       Load the molar concentration of this Variable.

       This method uses loadNumberConc() instead of setNumberConc().

       @see setNumberConc()
    */
// Delete because no file in spatiocyte use MolarConc and NumberConc. (2010/12/21)
//    LOAD_METHOD( Real, MolarConc )
//    {
//        loadNumberConc( value * N_A );
//    }

    /**
       Returns the number concentration of this Variable.

       Unlike getMolarConc, this method just returns value / size.

       @return Concentration in [number/L].
    */
// Delete because no file in spatiocyte use NumberConc. (2010/12/21)
//    GET_METHOD( Real, NumberConc )
//    {
//        return getValue() / getSizeOfSuperSystem();
//    }

    /**
       Set the number concentration of this Variable.

       @param value concentration in [number/L].
    */

// Delete because no file in spatiocyte use NumberConc. (2010/12/21)
//    SET_METHOD( Real, NumberConc )
//    {
//        setValue( value * getSizeOfSuperSystem() );
//    }


    /**
       Load the number concentration of this Variable.

       This method can be called before the SIZE Variable of 
       the supersystem of this Variable is configured in
       Model::initialize().

       Thus this method gets the value of the SIZE Variable
       without relying on the System::getSizeVariable() method
       of the supersystem.

       @see loadMolarConc()
       @see System::getSizeVariable()
       @see System::configureSizeVariable()
       @see System::findSizeVariable()
    */

// Delete because no file in spatiocyte use NumberConc. (2010/12/21)
//    LOAD_METHOD( Real, NumberConc );

//    void registerInterpolant( Interpolant* anInterpolant );// comment out (2010/12/21)

// Delete because no file in spatiocyte use this function. (2011/01/12)
//    virtual void detach();

protected:
// Delete because no file in spatiocyte use this function. (2010/12/21)
//    const Real calculateDifferenceSum( Real aCurrentTime, 
//                                       Real anInterval ) const
//    {
//        Real aVelocitySum( 0.0 );
//        FOR_ALL( InterpolantVector, theInterpolantVector )
//        {
//            Interpolant const* anInterpolantPtr( *i );
//            aVelocitySum += anInterpolantPtr->getDifference(
//                aCurrentTime, anInterval );
//        }
//        return aVelocitySum;
//    }

// Delete because no file in spatiocyte use this function. (2010/12/21)
//    void updateValue( Real aCurrentTime )
//    {
//        const Real anInterval( aCurrentTime - theLastTime );
//
//        if( anInterval == 0.0 )
//        {
//            return;
//        }
//
//        const Real aVelocitySum( calculateDifferenceSum(
//            aCurrentTime, anInterval ) );
//        setValue( getValue() + aVelocitySum );
//
//        theLastTime = aCurrentTime;
//    }


    void clearInterpolantVector();

// Delete because no file in spatiocyte use this function. (2010/12/21)
// private:
//    const Real getSizeOfSuperSystem() const
//    {
//        return getSuperSystem()->getSizeVariable()->getValue();
//    }

protected:
    double theValue;

    double theLastTime;

    double theDiffusionCoeff;

//     InterpolantVector theInterpolantVector;// comment out (2010/12/21)

    bool theFixed;

    std::string     theVariableName;// add (2011/02/01)
};

// } // namespace libecs // comment out (2010/12/24)

#endif /* __VARIABLE_HPP */
