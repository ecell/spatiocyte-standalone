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
// E-Cell Project, Institute for Advanced Biosciences, Keio University.
//
// modified for stand-alone (2011/02/03)
// String -> std::string (2011/01/07)
// Real -> double (2011/01/17)
// Time -> double (2011/02/03)
// INF -> std::numeric_limits<double>::infinity() (2011/02/03)

#ifndef __SpatiocyteProcess_hpp
#define __SpatiocyteProcess_hpp

#include "libecs/Process.hpp" // change code (2010/12/24)
// #include <Process.hpp>
#include "SpatiocyteCommon.hpp"
#include "SpatiocyteStepper.hpp"

class SpatiocyteProcess : public Process// change code (2010/12/24)
{
// LIBECS_DM_CLASS(SpatiocyteProcess, Process)
// { 
public:
//   LIBECS_DM_OBJECT(SpatiocyteProcess, Process)
//     {
//       INHERIT_PROPERTIES(Process);
//     }
  SpatiocyteProcess(bool aPriority=true):
    isInitialized(false),
    isPriority(aPriority),
// change code (2010/12/24)
    theStepInterval( std::numeric_limits<double>::infinity() ),
    theTime( std::numeric_limits<double>::infinity() ) {}
//    theStepInterval(libecs::INF),
//    theTime(libecs::INF) {}
  virtual ~SpatiocyteProcess() {}
  virtual void fire() {}
  virtual void initializeSecond()
    {
      theSpecies = theSpatiocyteStepper->getSpecies();
    }
  virtual void initializeThird() {}
  virtual void initializeFourth() {}
  virtual void initializeLastOnce() {}
  virtual void printParameters() {}
  virtual void substrateValueChanged(double) {}
  virtual void initialize()
    {
      if(isInitialized)
        {
          return;
        }
      isInitialized = true;
      Process::initialize();
      theSpatiocyteStepper = reinterpret_cast<SpatiocyteStepper*>(getStepper());
      theSortedVariableReferences.resize(theVariableReferenceVector.size());
      for(VariableReferenceVector::iterator
          i(theVariableReferenceVector.begin());
          i != theVariableReferenceVector.end(); ++i)
        {
          theSortedVariableReferences[(*i).getEllipsisNumber()] = *i; 
        }
      for(VariableReferenceVector::iterator
          i(theSortedVariableReferences.begin());
          i != theSortedVariableReferences.end(); ++i)
        {
          Variable* aVariable((*i).getVariable()); 
          // if Name is HD, it is a homogeneously distributed species.
          // only create Species for immobile or diffusing species
          if(aVariable->getName() != "HD")
            {
              Species* aSpecies(theSpatiocyteStepper->addSpecies(aVariable));
              theProcessSpecies.push_back(aSpecies);
            }
          if((*i).getCoefficient() > 0)
            {
              thePositiveVariableReferences.push_back(*i);
            }
          else if((*i).getCoefficient() < 0)
            {
              theNegativeVariableReferences.push_back(*i);
            }
          else
            {
              theZeroVariableReferences.push_back(*i);
            }
        }
    }
  // change code (2010/12/24)
  virtual const double getStepInterval() const
    {
      return theStepInterval;
    }
//  virtual GET_METHOD(Real, StepInterval)
//    {
//      return theStepInterval;
//    }
  void setPriorityQueue(ProcessPriorityQueue* aPriorityQueue)
    {
      thePriorityQueue = aPriorityQueue;
    }
  const double getTime() const
    {
      return theTime;
    }
  const bool getIsPriority() const
    {
      return isPriority;
    }
  void setTime(double aTime)
    {
      theTime = aTime;
    }
  void setQueueID(ProcessID anID)
    {
      theQueueID = anID;
    }
  virtual void addSubstrateInterrupt(Species* aSpecies, Voxel* aMolecule) {}
  virtual void removeSubstrateInterrupt(Species* aSpecies, Voxel* aMolecule) {}
  Species* id2species(unsigned short id)
    {
      return theSpatiocyteStepper->id2species(id);
    }
protected:
  const std::string getIDString(Voxel*) const;
  const std::string getIDString(Species*) const;
protected:
  bool isInitialized;
  bool isPriority;
  double theStepInterval;
  double theTime;
  ProcessID theQueueID;
  ProcessPriorityQueue* thePriorityQueue; 
  SpatiocyteStepper* theSpatiocyteStepper;
  vector<Species*> theSpecies;
  vector<Species*> theProcessSpecies;
  VariableReferenceVector thePositiveVariableReferences;
  VariableReferenceVector theNegativeVariableReferences;
  VariableReferenceVector theZeroVariableReferences;
  VariableReferenceVector theSortedVariableReferences;
};

#endif /* __SpatiocyteProcess_hpp */
