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
// INF -> std::numeric_limits<double>::infinity() (2011/02/03)

#ifndef __DiffusionProcess_hpp
#define __DiffusionProcess_hpp

#include <sstream>
// #include <MethodProxy.hpp> comment out(2010/12/20)
#include "SpatiocyteProcess.hpp"
#include "SpatiocyteSpecies.hpp"

#include "libecs/Exceptions.hpp" // add (2011/01/11)

// change code (2010/12/27)
class DiffusionProcess : public SpatiocyteProcess
{ 
// LIBECS_DM_CLASS(DiffusionProcess, SpatiocyteProcess)
// { 
  typedef const void (DiffusionProcess::*WalkMethod)(void) const;
public:
//  LIBECS_DM_OBJECT(DiffusionProcess, Process)
//    {
//      INHERIT_PROPERTIES(Process);
//      PROPERTYSLOT_SET_GET(Real, D);
//      PROPERTYSLOT_SET_GET(Real, P);
//      PROPERTYSLOT_SET_GET(Real, WalkProbability);
//    }
  DiffusionProcess():
    D(0),
    P(1),
    WalkProbability(1),
    theWalkMethod(&DiffusionProcess::volumeWalk) {}
  virtual ~DiffusionProcess() {}

  // change code (2010/12/27)
  void setD( double &value )
    {
      D = value;
    }
  const double getD() const
    {
      return D;
    }
  //SIMPLE_SET_GET_METHOD(Real, D);

  // change code (2010/12/27)
  void setP( double &value )
    {
      P = value;
    }
  const double getP() const
    {
      return P;
    }
  //SIMPLE_SET_GET_METHOD(Real, P);

  // change code (2010/12/27)
  void setWalkProbability( double &value )
    {
      WalkProbability = value;
    }
  const double getWalkProbability() const
    {
      return WalkProbability;
    }
  //SIMPLE_SET_GET_METHOD(Real, WalkProbability);
  virtual void initialize()
    {
      if(isInitialized)
        {
          return;
        }
      SpatiocyteProcess::initialize();
      for(vector<Species*>::const_iterator i(theProcessSpecies.begin());
          i != theProcessSpecies.end(); ++i)
        {
          (*i)->setDiffusionCoefficient(D);
        }
    }
  virtual void initializeThird()
    {
      Species* aSpecies(theProcessSpecies[0]);
      isVolume = aSpecies->getIsVolume();
      double rho(aSpecies->getMaxReactionProbability());
      if(D > 0)
        {
          for(vector<Species*>::const_iterator i(theProcessSpecies.begin());
              i != theProcessSpecies.end(); ++i)
            {
              if((*i)->getIsVolume() != isVolume)
                {
                  // change code (2011/02/01)
                  THROW_EXCEPTION(ValueError, std::string(
                                   getProcessName() ) +
                                  "[" + getFullID().asString() + 
//                  THROW_EXCEPTION(ValueError, std::string(
//                                   getPropertyInterface().getClassName()) +
//                                  "[" + getFullID().asString() + 
                                  "]: A DiffusionProcess can only execute" +
                                  " multiple species when they are all either" +
                                  " in a volume compartment or a surface" +
                                  " compartment, not both concurrently. " +
                                  getIDString(theProcessSpecies[0]) + " and " +
                                  getIDString(*i) + " belong to different" +
                                  " types of compartment.");
                }
              if(rho < (*i)->getMaxReactionProbability())
                {
                  if(rho > P)
                    {
                      // change code (2011/02/01)
                      THROW_EXCEPTION(ValueError, std::string(
                                       getProcessName() ) +
                                      "[" + getFullID().asString() + 
//                      THROW_EXCEPTION(ValueError, std::string(
//                                       getPropertyInterface().getClassName()) + 
//                                      "[" + getFullID().asString() + 
                                      "]: Create separate" +
                                      " DiffusionProcesses for " +
                                      getIDString(aSpecies) + " and " +
                                      getIDString(*i) + " since their" +
                                      " reaction probabilities are not the" +
                                      " same and the latter's reaction" +
                                      " probability is higher than P.");
                    }
                  aSpecies = *i;
                  rho = (*i)->getMaxReactionProbability();
                }
            }
        }
      if(rho > P)
        {
          WalkProbability = P/rho;
        }
      for(vector<Species*>::const_iterator i(theProcessSpecies.begin());
          i != theProcessSpecies.end(); ++i)
        {
          (*i)->rescaleReactionProbabilities(WalkProbability);
        }
      if(D > 0)
        {
          double r_v(theSpatiocyteStepper->getVoxelRadius());

          // change integer argument to double in sqrt (2011/01/31)
          double lambda(2.0/3.0);
          //double lambda(2.0/3);
          if(!isVolume)
            {
              // change integer argument to double in sqrt (2011/01/31)
              lambda = pow((2.0*sqrt(2.0)+4.0*sqrt(3.0)+3.0*sqrt(6.0)+sqrt(22.0))/
                           (6.0*sqrt(2.0)+4.0*sqrt(3.0)+3.0*sqrt(6.0)), 2);
              //lambda = pow((2*sqrt(2)+4*sqrt(3)+3*sqrt(6)+sqrt(22))/
              //             (6*sqrt(2)+4*sqrt(3)+3*sqrt(6)), 2);
            }
          theStepInterval = lambda*r_v*r_v*WalkProbability/D;
        }
      for(vector<Species*>::const_iterator i(theProcessSpecies.begin());
          i != theProcessSpecies.end(); ++i)
        {
          (*i)->setDiffusionInterval(theStepInterval);
        }
      if(isVolume)
        {
          if(WalkProbability == 1)
            {
              theWalkMethod = &DiffusionProcess::volumeWalk;
            }
          else
            {
              theWalkMethod = &DiffusionProcess::volumeWalkCollide;
            }
        }
      else
        {
          if(WalkProbability == 1)
            {
              theWalkMethod = &DiffusionProcess::surfaceWalk;
            }
          else
            {
              theWalkMethod = &DiffusionProcess::surfaceWalkCollide;
            }
        }
      //At the start of the simulation, we must make sure the CollisionProcess
      //is fired first before the DiffusionProcess. This is to make sure
      //the reaction probability is valid for reactants that are initially
      //at contact:
    }
  virtual void printParameters()
    {
      for(vector<Species*>::const_iterator i(theProcessSpecies.begin());
          i != theProcessSpecies.end(); ++i)
        {
          cout << getIDString(*i) << " ";
        }
      cout << ":" << endl << "  Diffusion interval=" << theStepInterval <<
        ", D=" << D << ", Walk probability (P/rho)=" <<
        WalkProbability << endl;
    }
  virtual void fire()
    {
      (this->*theWalkMethod)();
      theTime += theStepInterval;
      thePriorityQueue->moveTop();
    }
  const void volumeWalk() const
    {
      for(vector<Species*>::const_iterator i(theProcessSpecies.begin());
          i != theProcessSpecies.end(); ++i)
        {
          (*i)->volumeWalk();
        }
    }
  const void volumeWalkCollide() const
    {
      for(vector<Species*>::const_iterator i(theProcessSpecies.begin());
          i != theProcessSpecies.end(); ++i)
        {
          (*i)->volumeWalkCollide();
        }
    }
  const void surfaceWalk() const
    {
      for(vector<Species*>::const_iterator i(theProcessSpecies.begin());
          i != theProcessSpecies.end(); ++i)
        {
          (*i)->surfaceWalk();
        }
    }
  const void surfaceWalkCollide() const
    {
      for(vector<Species*>::const_iterator i(theProcessSpecies.begin());
          i != theProcessSpecies.end(); ++i)
        {
          (*i)->surfaceWalkCollide();
        }
    }
  virtual void initializeLastOnce()
    {
      for(vector<Species*>::const_iterator i(theProcessSpecies.begin());
          i != theProcessSpecies.end(); ++i)
        {
          (*i)->addInterruptedProcess(
                            reinterpret_cast<SpatiocyteProcess*>(this));
        }
    }
  virtual void addSubstrateInterrupt(Species* aSpecies, Voxel* aMolecule)
    {
      if(theStepInterval == std::numeric_limits<double>::infinity())// change code (2010/12/27)
//      if(theStepInterval == libecs::INF)
        {
          theStepInterval = theProcessSpecies[0]->getDiffusionInterval();
          theTime = theSpatiocyteStepper->getCurrentTime() + theStepInterval; 
          thePriorityQueue->move(theQueueID);
        }
    }
  virtual void removeSubstrateInterrupt(Species* aSpecies, Voxel* aMolecule)
    {
      if(theStepInterval != std::numeric_limits<double>::infinity())// change code (2010/12/27)
//      if(theStepInterval != libecs::INF)
        {
          for(vector<Species*>::const_iterator i(theProcessSpecies.begin());
              i != theProcessSpecies.end(); ++i)
            {
              if((*i)->size())
                {
                  return;
                }
            }
          theStepInterval = std::numeric_limits<double>::infinity();// change code (2010/12/27)
          //theStepInterval = libecs::INF;
          theTime = theStepInterval; 
          thePriorityQueue->move(theQueueID);
        }
    }
protected:
  bool isVolume;
  double D;
  double P;
  double WalkProbability;
  WalkMethod theWalkMethod;
};

#endif /* __DiffusionProcess_hpp */
