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
// Real -> double (2011/01/17)
// INF -> std::numeric_limits<double>::infinity() (2011/02/03)

#ifndef __PolymerFragmentationProcess_hpp
#define __PolymerFragmentationProcess_hpp

// #include <MethodProxy.hpp> comment out (2010/12/20)
#include "ReactionProcess.hpp"
#include "PolymerizationProcess.hpp"

#include "libecs/Exceptions.hpp" // add (2011/01/11)

class PolymerizationProcess;

// change code (2011/01/04)
class PolymerFragmentationProcess : public ReactionProcess
{ 
// LIBECS_DM_CLASS(PolymerFragmentationProcess, ReactionProcess)
// { 
public:
// comment out (2010/12/22)
//   LIBECS_DM_OBJECT(PolymerFragmentationProcess, Process)
//     {
//       INHERIT_PROPERTIES(ReactionProcess);
//       PROPERTYSLOT_SET_GET(Real, BendAngle);
//     }

  // change code (2010/12/22)
  void setBendAngle( double &value )
    {
      BendAngle = value;
    }
  const double getBendAngle() const
    {
      return BendAngle;
    }
//   SIMPLE_SET_GET_METHOD(Real, BendAngle);
  PolymerFragmentationProcess():
    theReactantSize(0),
    BendAngle(0) {}
  virtual ~PolymerFragmentationProcess() {}
  virtual void initialize()
    {
      if(isInitialized)
        {
          return;
        }
      ReactionProcess::initialize();
      if(getOrder() != 1 && getOrder() != 2)
        {
              // change code (2011/02/01)
              THROW_EXCEPTION(ValueError, 
                              std::string( getProcessName() ) + 
                              "[" + getFullID().asString() + 
                              "]: This PolymerFragmentationProcess requires " +
                              "two substrates.");
//              THROW_EXCEPTION(ValueError, 
//                              std::string(getPropertyInterface().getClassName()) + 
//                              "[" + getFullID().asString() + 
//                              "]: This PolymerFragmentationProcess requires " +
//                              "two substrates.");
        }
      if(p == -1)
        {
          p = k;
        }
      else
        {
          k = p;
        }
    }

  virtual void initializeLastOnce();
  virtual bool isContinuous() const
    {
      return true;
    }
  // change code (2010/12/22)
  virtual const double getStepInterval() const
    {
      return getPropensity()*
        (-log(gsl_rng_uniform_pos(getStepper()->getRng())));
    }
//   virtual GET_METHOD(Real, StepInterval)
//     {
//       return getPropensity()*
//         (-log(gsl_rng_uniform_pos(getStepper()->getRng())));
//     }
  virtual void fire();
  virtual void addSubstrateInterrupt(Species* aSpecies, Voxel* aMolecule);
  virtual void removeSubstrateInterrupt(Species* aSpecies, Voxel* aMolecule);
  void addMoleculeA(Voxel* aMolecule)
    { 
      ++theReactantSize;
      if(theReactantSize > theReactants.size())
        {
          theReactants.push_back(aMolecule);
        }
      else
        {
          theReactants[theReactantSize-1] = aMolecule;
        }
      substrateValueChanged(theSpatiocyteStepper->getCurrentTime());
    }
  void setPolymerizeProcess(PolymerizationProcess* aProcess)
    {
      thePolymerizeProcess = aProcess;
    }
protected:
  const double getPropensity() const
    {
      if(theReactantSize > 0 && p > 0)
        {
          return 1/(p*theReactantSize);
        }
      else
        {
          return std::numeric_limits<double>::infinity();// change code (2010/12/22)
          // return libecs::INF;
        }
    }
protected:
  unsigned int theReactantSize;
  int theBendIndexA;
  int theBendIndexB;
  double BendAngle;
  PolymerizationProcess* thePolymerizeProcess;
  vector<Voxel*> theReactants;
};


#endif /* __PolymerFragmentationProcess_hpp */


