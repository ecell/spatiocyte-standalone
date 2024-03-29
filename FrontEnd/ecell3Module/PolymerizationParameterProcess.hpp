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
// modified for stand-alone (2011/02/01)
// String -> std::string (2011/01/07)
// Integer -> long int (2011/01/17)

#ifndef __PolymerizationParameterProcess_hpp
#define __PolymerizationParameterProcess_hpp

#include "SpatiocyteProcess.hpp"
#include "SpatiocyteSpecies.hpp"

#include "libecs/Exceptions.hpp" // add (2011/01/11)

class PolymerizationParameterProcess : public SpatiocyteProcess // change code (2010/12/28)
{ 
// LIBECS_DM_CLASS(PolymerizationParameterProcess, SpatiocyteProcess)
// { 
public:
// comment out (2010/12/28)
//  LIBECS_DM_OBJECT(PolymerizationParameterProcess, Process)
//    {
//      INHERIT_PROPERTIES(Process);
//      PROPERTYSLOT_SET_GET(Polymorph, BendAngles);
//      PROPERTYSLOT_SET_GET(Integer, PolymerDirectionality);
//    }
  const Polymorph getBendAngles() const // change code (2011/01/06)
    { 
      return BendAngles;
    }
//  SIMPLE_GET_METHOD(Polymorph, BendAngles);
  void setPolymerDirectionality( long int &value ) // change code (2011/01/06)
    {
      PolymerDirectionality = value;
    }
  const long int getPolymerDirectionality() const
    {
      return PolymerDirectionality;
    }
//  SIMPLE_SET_GET_METHOD(Integer, PolymerDirectionality);
  PolymerizationParameterProcess():
    PolymerDirectionality(0) {}
  virtual ~PolymerizationParameterProcess() {}
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
          (*i)->setIsPolymer(theBendAngles, PolymerDirectionality);
        }
    }
  void setBendAngles(const Polymorph& aValue)
    {
      BendAngles = aValue;
      PolymorphVector aValueVector(aValue.as<PolymorphVector>());
      for(unsigned int i(0); i != aValueVector.size(); ++i)
        {
          theBendAngles.push_back(aValueVector[i].as<double>());
        }
    }
  virtual void initializeThird()
    {
      for(vector<Species*>::const_iterator i(theProcessSpecies.begin());
          i != theProcessSpecies.end(); ++i)
        {
          if((*i)->getIsDiffusing())
            {
              // change code (2011/02/01)
              THROW_EXCEPTION(ValueError, 
                              std::string( getProcessName() ) + 
                              "[" + getFullID().asString() + 
                              "]: All polymer subunit species must be" +
                              " immobile. " +
                              (*i)->getVariable()->getFullID().asString() +
                              " is a diffusing species.");
//              THROW_EXCEPTION(ValueError, 
//                              std::string(getPropertyInterface().getClassName()) + 
//                              "[" + getFullID().asString() + 
//                              "]: All polymer subunit species must be" +
//                              " immobile. " +
//                              (*i)->getVariable()->getFullID().asString() +
//                              " is a diffusing species.");
            }
          if((*i)->getIsVolume())
            {
              // change code (2011/02/01)
              THROW_EXCEPTION(ValueError, 
                              std::string( getProcessName() ) + 
                              "[" + getFullID().asString() + 
                              "]: All polymer subunit species must be on a " +
                              "surface. " +
                              (*i)->getVariable()->getFullID().asString() +
                              " is a volume species.");
//              THROW_EXCEPTION(ValueError, 
//                              std::string(getPropertyInterface().getClassName()) + 
//                              "[" + getFullID().asString() + 
//                              "]: All polymer subunit species must be on a " +
//                              "surface. " +
//                              (*i)->getVariable()->getFullID().asString() +
//                              " is a volume species.");
            }
        }
    }
protected:
  Polymorph BendAngles;
  vector<double> theBendAngles;
  int PolymerDirectionality;
};

#endif /* __PolymerizationParameterProcess_hpp */
