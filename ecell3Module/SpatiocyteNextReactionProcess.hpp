//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
//        This file is part of E-Cell Simulation Environment package
//
//                Copyright (C) 2006-2009 Keio University
//
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
//
// E-Cell is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
// 
// E-Cell is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public
// License along with E-Cell -- see the file COPYING.
// If not, write to the Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
// 
//END_HEADER
//
// written by Satya Arjunan <satya.arjunan@gmail.com>
// based on GillespieProcess.hpp
// E-Cell Project, Institute for Advanced Biosciences, Keio University.
//
// modified for stand-alone (2011/02/03)
// String -> std::string (2011/01/07)
// Integer -> long int (2011/01/17)
// Real -> double (2011/01/17)
// INF -> std::numeric_limits<double>::infinity() (2011/02/03)

#ifndef __SpatiocyteNextReactionProcess_hpp
#define __SpatiocyteNextReactionProcess_hpp

#include "libecs/MethodProxy.hpp" // change code (2010/12/27)
// #include <MethodProxy.hpp>
#include "ReactionProcess.hpp"

#include "libecs/Exceptions.hpp" // add (2011/01/11)

/**
   This macro throws UnexpectedError exception with a method name.

   Use this macro to indicate where must not be reached.
*/

// change code (2010/12/27)
class SpatiocyteNextReactionProcess : public ReactionProcess
{
// LIBECS_DM_CLASS(SpatiocyteNextReactionProcess, ReactionProcess)
// { 
  typedef MethodProxy<SpatiocyteNextReactionProcess, double> RealMethodProxy; 
  typedef double (SpatiocyteNextReactionProcess::*PDMethodPtr)(Variable*);
public:
// comment out (2010/12/27)
//  LIBECS_DM_OBJECT(SpatiocyteNextReactionProcess, Process)
//    {
//      INHERIT_PROPERTIES(ReactionProcess);
//      PROPERTYSLOT_GET_NO_LOAD_SAVE(Real, Propensity);
//    }
  SpatiocyteNextReactionProcess():
    initSizeA(0),
    initSizeB(0),
    initSizeC(0),
    initSizeD(0),
    theGetPropensityMethodPtr(RealMethodProxy::create<
            &SpatiocyteNextReactionProcess::getPropensity_ZerothOrder>()) {}
  virtual ~SpatiocyteNextReactionProcess() {}
  virtual void initialize()
    {
      if(isInitialized)
        {
          return;
        }
      ReactionProcess::initialize();
      if(!(getOrder() == 0 || getOrder() == 1 || getOrder() == 2))
        {
          // change code (2011/02/01)
          THROW_EXCEPTION(ValueError, 
                          std::string( getProcessName() ) + 
                          "[" + getFullID().asString() + 
                          "]: Only zeroth, first or second order scheme " + 
                          "is allowed.");
//          THROW_EXCEPTION(ValueError, 
//                          std::string(getPropertyInterface().getClassName()) + 
//                          "[" + getFullID().asString() + 
//                          "]: Only zeroth, first or second order scheme " + 
//                          "is allowed.");
        }
      if(variableA)
        {
          initSizeA = variableA->getValue();
        }
      if(variableB)
        {
          initSizeB = variableB->getValue();
        }
      if(variableC)
        {
          initSizeC = variableC->getValue();
        }
      if(variableD)
        {
          initSizeD = variableD->getValue();
        }
    }
  virtual void initializeSecond()
    {
      ReactionProcess::initializeSecond();
      if(variableA)
        {
          variableA->setValue(initSizeA);
        }
      if(variableB)
        {
          variableB->setValue(initSizeB);
        }
      if(variableC)
        {
          variableC->setValue(initSizeC);
        }
      if(variableD)
        {
          variableD->setValue(initSizeD);
        }
    }
  // change code (2010/12/27)
  const double getPropensity() const
    {
//  GET_METHOD(Real, Propensity)
//    {
      double aPropensity(theGetPropensityMethodPtr(this));
      if(aPropensity < 0.0)
        {
          THROW_EXCEPTION(SimulationError, "Variable value <= -1.0");
          return 0.0;
        }
      else
        {
          return aPropensity;
        }
    }
  // change code (2010/12/27)
  const double getPropensity_R() const
    {
//  GET_METHOD(Real, Propensity_R)
//    {
      double aPropensity(getPropensity());
      if(aPropensity > 0.0)
        {
          return 1.0/aPropensity;
        }
      else
        {
          return std::numeric_limits<double>::infinity();
//          return libecs::INF;
        }
    }
  virtual bool isContinuous() 
    {
      return true;
    }

  // change code (2010/12/27)
  virtual const double getStepInterval() const;
//  virtual GET_METHOD(Real, StepInterval);

  virtual void fire();
  virtual void initializeThird();
protected:
  virtual void calculateOrder();
  double getPropensity_ZerothOrder() 
    {
      return p;
    }
  double getPropensity_FirstOrder() 
    {
      double aValue(theVariableReferenceVector[0].getVariable()->getValue());
      if(aValue > 0.0)
        {
          return p*aValue;
        }
      else
        {
          return 0.0;
        }
    }
  double getPropensity_SecondOrder_TwoSubstrates() 
    {
      double aValue1(theVariableReferenceVector[0].getVariable()->getValue());
      double aValue2(theVariableReferenceVector[1].getVariable()->getValue());
      if(aValue1 > 0.0 && aValue2 > 0.0)
        {
          return p*aValue1*aValue2;
        }
      else
        {
          return 0.0;
        }
    }
  double getPropensity_SecondOrder_OneSubstrate() 
    {
      double aValue(theVariableReferenceVector[0].getVariable()->getValue());
      //There must be two or more molecules:
      if(aValue > 1.0)
        {
          return p*aValue*(aValue-1.0);
        }
      else
        {
          return 0.0;
        }
    }
protected:
  RealMethodProxy theGetPropensityMethodPtr;  
  int initSizeA;
  int initSizeB;
  int initSizeC;
  int initSizeD;
};

inline void SpatiocyteNextReactionProcess::calculateOrder()
{
  ReactionProcess::calculateOrder();
  for(VariableReferenceVector::iterator
      i(theVariableReferenceVector.begin());
      i != theVariableReferenceVector.end(); ++i)
    {
      VariableReference aVariableReference(*i);
      long int aCoefficient(aVariableReference.getCoefficient());
      // here assume aCoefficient != 0
      if(aCoefficient == 0)
        {
          THROW_EXCEPTION(InitializationFailed,
                          "[" + getFullID().asString() + 
                          "]: Zero stoichiometry is not allowed.");
        }
    }
  // set theGetPropensityMethodPtr
  if(getOrder() == 0) // no substrate
    {
      theGetPropensityMethodPtr = RealMethodProxy::create<
        &SpatiocyteNextReactionProcess::getPropensity_ZerothOrder>();
    }
  else if(getOrder() == 1)   // one substrate, first order.
    {
      theGetPropensityMethodPtr = RealMethodProxy::create<
        &SpatiocyteNextReactionProcess::getPropensity_FirstOrder>();
    }
  else if(getOrder() == 2)
    { 
      //Two unique substrate species, second order
      //A + B -> products:
      if(getZeroVariableReferenceOffset() == 2)
        {  
          theGetPropensityMethodPtr = RealMethodProxy::
            create<&SpatiocyteNextReactionProcess::
            getPropensity_SecondOrder_TwoSubstrates>();
        }
      //One substrate species, second order
      //A + A -> products:
      else
        {
          theGetPropensityMethodPtr = RealMethodProxy::
            create<&SpatiocyteNextReactionProcess::
            getPropensity_SecondOrder_OneSubstrate>();
        }
    }
  else
    {
      // change code (2011/02/03)
      THROW_EXCEPTION( UnexpectedError,
                       "never get here (" + std::string( __PRETTY_FUNCTION__ )
                       + ")" );
//      NEVER_GET_HERE;
    }
}

#endif /* __SpatiocyteNextReactionProcess_hpp */
