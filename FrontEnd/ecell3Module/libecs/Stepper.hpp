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
// Time -> double (2011/02/03)
// INF -> std::numeric_limits<double>::infinity() (2011/02/03)

#ifndef __STEPPER_HPP
#define __STEPPER_HPP

#ifdef DLL_EXPORT
#undef DLL_EXPORT
#include <gsl/gsl_rng.h>
#define DLL_EXPORT
#else
#include <gsl/gsl_rng.h>
#endif

#include <vector> // add (2010/12/24)
#include <limits> // add (2011/02/03)

// comment out (2011/02/03)
// change code (2010/12/24)
// #include "Defs.hpp"
// #include "libecs/Defs.hpp"

#include "Model.hpp"// add (2010/12/27)

// #include "libecs/Util.hpp" // comment out (2010/12/24)

#include "Polymorph.hpp"// change code (2010/12/28)
// #include "libecs/Polymorph.hpp"

// #include "libecs/Interpolant.hpp" // comment out (2010/12/24)

// delete (2011/02/01)
// #include "PropertyInterface.hpp"// change code (2010/12/27)
// #include "libecs/PropertyInterface.hpp"

// namespace libecs
// {
//
class Model;
// class Logger;
class Process;
class System;
class Variable;

/**
   Stepper class defines and governs a computation unit in a model.

   The computation unit is defined as a set of Process objects.
*/
class Stepper // change code (2010/12/22)
{
// LIBECS_DM_CLASS( Stepper, EcsObject )
// {
public:
    typedef std::vector<double> RealVector;
    typedef std::vector<Variable*> VariableVector;
    typedef std::vector<Process*> ProcessVector;
    typedef std::vector<System*> SystemVector;

    typedef VariableVector::size_type VariableIndex;

// comment out (2010/12/22)
//    LIBECS_DM_BASECLASS( Stepper );
//
//    LIBECS_DM_OBJECT_ABSTRACT( Stepper )
//    {
//        INHERIT_PROPERTIES( EcsObject );
//        
//        PROPERTYSLOT_SET_GET( Integer, Priority );
//        PROPERTYSLOT_SET_GET( Real,    StepInterval );
//        PROPERTYSLOT_SET_GET( Real,    NextTime );
//        PROPERTYSLOT_SET_GET( Real,    MaxStepInterval );
//        PROPERTYSLOT_SET_GET( Real,    MinStepInterval );
//        PROPERTYSLOT_SET( String, RngSeed );
//
//        // these properties are not loaded/saved.
//        PROPERTYSLOT_GET_NO_LOAD_SAVE( Real,      CurrentTime );
//        PROPERTYSLOT_GET_NO_LOAD_SAVE( Polymorph, ProcessList );
//        PROPERTYSLOT_GET_NO_LOAD_SAVE( Polymorph, SystemList );
//        PROPERTYSLOT_GET_NO_LOAD_SAVE( Polymorph, ReadVariableList );
//        PROPERTYSLOT_GET_NO_LOAD_SAVE( Polymorph, WriteVariableList );
//    }


    class PriorityCompare
    {
    public:
        bool operator()( Stepper const* aLhs, Stepper const* aRhs ) const
        {
            return compare( aLhs->getPriority(), aRhs->getPriority() );
        }

        bool operator()( Stepper const* aLhs, long int aRhs ) const
        {
            return compare( aLhs->getPriority(), aRhs );
        }

        bool operator()( long int aLhs, Stepper const* aRhs ) const
        {
            return compare( aLhs, aRhs->getPriority() );
        }

    private:
        static bool compare( long int aLhs, long int aRhs )
        {
            return aLhs > aRhs;
        }
    };


    Stepper();
    virtual ~Stepper();


    /**
       Get the current time of this Stepper.

       @return the current time in Real.
    */
    const double getCurrentTime() const
    {
        return theCurrentTime;
    }
//    GET_METHOD( Real, CurrentTime )
//    {
//        return theCurrentTime;
//    }

    // change code (2010/12/22)
    void setCurrentTime( const double &value )
    {
        theCurrentTime = value;
    }
//    SET_METHOD( Real, CurrentTime )
//    {
//        theCurrentTime = value;
//    }

    /**
       Get the step interval of this Stepper.

       The step interval is a length of time that this Stepper proceeded
       in the last step.

       @return the step interval of this Stepper
    */
    // change code (2010/12/22)
    const double getStepInterval() const
    {
        const double aNextTime( getNextTime() );
        const double aCurrentTime( getCurrentTime() );
        if ( aCurrentTime == std::numeric_limits<double>::infinity() )
        {
            return std::numeric_limits<double>::infinity();
        }
        return aNextTime - aCurrentTime;
    }
//    GET_METHOD( Real, StepInterval )
//    {
//        const Real aNextTime( getNextTime() );
//        const Real aCurrentTime( getCurrentTime() );
//        if ( aCurrentTime == libecs::INF )
//        {
//            return libecs::INF;
//        }
//        return aNextTime - aCurrentTime;
//    }

    // change code (2010/12/22)
    virtual void setStepInterval( const double &value );
//    virtual SET_METHOD( Real, StepInterval );

    /**
       This may be overridden in dynamically scheduled steppers.
    */
    // change code (2010/12/22)
    virtual void setNextTime( const double &value );
//    virtual SET_METHOD( Real, NextTime );

    /**
       Get the next time to which this Stepper is scheduled

       @return the next time to be scheduled to
    */
    // change code (2010/12/22)
    const double getNextTime() const
    {
        return theNextTime;
    }
//    GET_METHOD( Real, NextTime )
//    {
//        return theNextTime;
//    }

    // change code (2010/12/22)
    virtual const double getTimeScale() const;
//    virtual GET_METHOD( Real, TimeScale );

    // change code (2010/12/22)
    void setID( const std::string &value )
    {
        theID = value;
    }
//    SET_METHOD( String, ID )
//    {
//        theID = value;
//    }

    // change code (2010/12/22)
    const std::string getID() const
    {
        return theID;
    }
//    GET_METHOD( String, ID )
//    {
//        return theID;
//    }


    // change code (2010/12/22)
    void setMinStepInterval( const double &value )
    {
        theMinStepInterval = value;
    }
//    SET_METHOD( Real, MinStepInterval )
//    {
//        theMinStepInterval = value;
//    }

    // change code (2010/12/22)
    const double getMinStepInterval() const
    {
        return theMinStepInterval;
    }
//    GET_METHOD( Real, MinStepInterval )
//    {
//        return theMinStepInterval;
//    }

    // change code (2010/12/22)
    void setMaxStepInterval( const double &value );
//    SET_METHOD( Real, MaxStepInterval );

    // change code (2010/12/22)
    const double getMaxStepInterval() const
    {
        return theMaxStepInterval;
    }
//    GET_METHOD( Real, MaxStepInterval )
//    {
//        return theMaxStepInterval;
//    }

    // delete (2011/02/01)
    // add from EcsObject(2010/12/27)
    // virtual PropertyInterfaceBase const& getPropertyInterface() const;

    /**
       Get a Model object to which this object belongs.

       @return a borrowed pointer to the Model.
    */
    // add from EcsObject(2010/12/27)
    Model* getModel() const
    {
        return theModel;
    }

    /**
       Associate a Model object with this object.

       @internal
       @param aModel the Model object to associate with this object.
     */
    // add from EcsObject(2010/12/27)
    void setModel( Model* const aModel )
    {
        theModel = aModel;
    }

    const Polymorph getWriteVariableList() const; // change code (2011/01/06)
    const Polymorph getReadVariableList() const; // change code (2011/01/06)
    const Polymorph getProcessList() const; // change code (2011/01/06)
    const Polymorph getSystemList() const; // change code (2011/01/06)
//    GET_METHOD( Polymorph, WriteVariableList );
//    GET_METHOD( Polymorph, ReadVariableList );
//    GET_METHOD( Polymorph, ProcessList );
//    GET_METHOD( Polymorph, SystemList );

    // change code (2010/12/22)
    void setRngSeed( const std::string &value );
//    SET_METHOD( String, RngSeed );

    // change code (2010/12/22)
    const std::string getRngType() const;
//    GET_METHOD( String, RngType );

    virtual void initialize();

    /**             
       Each subclass of Stepper defines this.

       @note Subclass of Stepper must call this by Stepper::calculate() from
       their step().
    */
    virtual void step() = 0;

    // Delete because no file in spatiocyte use integrate (2011/01/31)
    // revive for unit test (2011/01/28)
    // delete "virtual" (2011/01/28)
    // void integrate( double aTime );
    // virtual void integrate( double aTime );

    /**
       Let the Loggers log data.

       The default behavior is to call all the Loggers attached to
       any Entities related to this Stepper.
    */
// Delete because no file in spatiocyte use log (2011/01/11)
//    virtual void log();
    
    /**
       Register a System to this Stepper.

       @param aSystemPtr a pointer to a System object to register
    */
    void registerSystem( System* aSystemPtr );

    /**
       Remove a System from this Stepper.

       @param aSystemPtr a pointer to a System object
    */
    void unregisterSystem( System* aSystemPtr );

    /**
       Remove all the associated System from this Stepper.
    */
    void unregisterAllSystems();

    /**
         Register a Process to this Stepper.

         @param aProcessPtr a pointer to a Process object to register
    */
    void registerProcess( Process* aProcessPtr );

    /**
       Remove a Process from this Stepper.

       @note This method is not currently supported.

       @param aProcessPtr a pointer to a Process object
    */
    void unregisterProcess( Process* aProcessPtr );

    /**
       Remove all the associated Process from this Stepper.
    */
    void unregisterAllProcesses();

// Delete because no file in spatiocyte use registerLogger (2010/12/22)
//    void registerLogger( Logger* );

    void setSchedulerIndex( int anIndex )
    {
        theSchedulerIndex = anIndex;
    }

    int getSchedulerIndex() const
    {
        return theSchedulerIndex;
    }

    /**
       Set a priority value of this Stepper.

       The priority is an Int value which is used to determine the
       order of step when more than one Stepper is scheduled at the
       same point in time (such as starting up: t=0).     

       Larger value means higher priority, and called first.

       @param value the priority value as an Int.
       @see Scheduler
    */
    // change code (2010/12/22)
    void setPriority( const long int &value )
    {
        thePriority = value;
    }
//    SET_METHOD( Integer, Priority )
//    {
//        thePriority = value;
//    }

    /**
       @see setPriority()
    */
    // change code (2010/12/22)
    const long int getPriority() const
    {
        return thePriority;
    }
//    GET_METHOD( Integer, Priority )
//    {
//        return thePriority;
//    }

    SystemVector const& getSystemVector() const
    {
        return theSystemVector;
    }

    /**
       Get the reference to the ProcessVector of this Stepper.

       The ProcessVector holds a set of pointers to this Stepper's Processes.

       The ProcessVector is partitioned in this way:

       |    Continuous Processes    |    Discrete Processes |

       getDiscreteProcessOffset() method returns the offset (index number)
       of the first discrete Process in this Stepper.

       Each part of the ProcessVector is sorted by Priority properties
       of Processes.

    */
    ProcessVector const& getProcessVector() const
    {
        return theProcessVector;
    }

    /**
       @see getProcessVector()
    */
    ProcessVector::size_type getDiscreteProcessOffset() const
    {
        return theDiscreteProcessOffset;
    }

    /**
       Get the reference to the VariableVector of this Stepper.

       In the VariableVector, Variables are classified and partitioned
       into the following three groups:

       | Write-Only Variables | Read-Write Variables | Read-Only Variables |

       Use getReadWriteVariableOffset() method to get the index of the first 
       Read-Write Variable in the VariableVector.    

       Use getReadOnlyVariableOffset() method to get the index of the first
       Read-Only Variable in the VariableVector.
    */
    VariableVector const& getVariableVector() const
    {
        return theVariableVector;
    }

    /**
       @see getVariableVector()
    */
    VariableVector::size_type getReadWriteVariableOffset() const
    {
        return theReadWriteVariableOffset;
    }

    /**
       @see getVariableVector()
    */
    VariableVector::size_type getReadOnlyVariableOffset() const
    {
        return theReadOnlyVariableOffset;
    }


    RealVector const& getValueBuffer() const
    {
        return theValueBuffer;
    }


    VariableIndex getVariableIndex( Variable const* aVariable ) const;

    virtual void interrupt( double aTime ) = 0;

    /**
       Definition of the Stepper dependency:
       Stepper A depends on Stepper B 
       if:
       - A and B share at least one Variable, AND
       - A reads AND B writes on (changes) the same Variable.

       See VariableReference class about the definitions of
       Variable 'read' and 'write'.


       @see Process, VariableReference
    */
// Delete because no file in spatiocyte use isDependentOn (2011/01/11)
//    bool isDependentOn( Stepper const* aStepper );

    /** 
       This method updates theIntegratedVariableVector.

       theIntegratedVariableVector holds the Variables those
       isIntegrationNeeded() method return true.     
    
       This method must be called after initialize().

       @internal
     */
// Delete because no file in spatiocyte use updateIntegratedVariableVector (2011/01/11)
//    void updateIntegratedVariableVector();


// Delete because no file in spatiocyte use Interpolant (2010/12/22)
//    virtual Interpolant* createInterpolant( Variable const* aVariable ) const;

    gsl_rng* getRng() const
    {
        return theRng;
    }

    bool operator<( Stepper const& rhs )
    {
        return getCurrentTime() < rhs.getCurrentTime();
    }

    /**
       Get a string representation of this Entity as String.

       @return a description string of this Entity.
    */
    virtual std::string asString() const;

    virtual void detach();

    /**
       Set the Stepper name (2011/02/01)
       ( example : SpatiocyteStepper, ... )
    */
    void setStepperName(const std::string &value)
    {
        theStepperName = value;
    }

    /**
       Get the Stepper name (2011/02/01)
       ( example : SpatiocyteStepper, ... )
    */
    const std::string getStepperName() const
    {
        return theStepperName;
    }
    
protected:
    void clearVariables();

    void fireProcesses();

    virtual void reset();


    /**
       Update theProcessVector.
    */
    void updateProcessVector();


    /**
       Update theVariableVector.
    */
    void updateVariableVector();


    /**
       Create Interpolant objects and distribute the objects to 
       write Variables.

       Ownership of the Interpolant objects are given away to the Variables.

       @see Variable::registerInterpolant()
    */
// Delete because no file in spatiocyte use createInterpolants (2011/01/11)
//    void createInterpolants();

protected:
    SystemVector              theSystemVector;

    VariableVector            theVariableVector;
    VariableVector::size_type theReadWriteVariableOffset;
    VariableVector::size_type theReadOnlyVariableOffset;

    VariableVector            theIntegratedVariableVector;

    ProcessVector             theProcessVector;
    ProcessVector::size_type  theDiscreteProcessOffset;

    RealVector                theValueBuffer;

    Model*                    theModel;// add from EcsObject(2010/12/27)

    /** the index on the scheduler */
    int                       theSchedulerIndex;

    long int                   thePriority;

    double                      theCurrentTime;

    double                      theNextTime;

    double                      theMinStepInterval;
    double                      theMaxStepInterval;

    std::string                    theID;

    gsl_rng*                  theRng;

    std::string               theStepperName;// add (2011/02/01)

private:
    // delete (2011/02/01)
    // add from EcsObject(2011/01/17)
    // static char              thePropertyInterface[ sizeof( PropertyInterface< Stepper > ) ];

};

// } // namespace libecs // comment out (2010/12/24)

#endif /* __STEPPER_HPP */
