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
// modifide for stand-alone (2011/01/17)
// Integer -> long int (2011/01/17)
// Real -> double (2011/01/17)

#ifndef __FluorescentProteinImagingProcess_hpp
#define __FluorescentProteinImagingProcess_hpp

#include "VisualizationLogProcess.hpp"
#include "SpatiocyteSpecies.hpp"

class FluorescentProteinImagingProcess : public VisualizationLogProcess // change code (2011/01/06)
//LIBECS_DM_CLASS(FluorescentProteinImagingProcess, VisualizationLogProcess)
{ 
public:
//  LIBECS_DM_OBJECT(FluorescentProteinImagingProcess, Process)
//    {
//      INHERIT_PROPERTIES(VisualizationLogProcess);
//      PROPERTYSLOT_SET_GET(Integer, MeanCount);
//      PROPERTYSLOT_SET_GET(Real, ExposureTime);
//    }
  FluorescentProteinImagingProcess():
    MeanCount(-1),
    theLastExposedTime(0) {}
  virtual ~FluorescentProteinImagingProcess() {}
  void setMeanCount( long int &value ) // change code (2011/01/06)
    {
      MeanCount = value;
    }
  const long int getMeanCount() const
    {
      return MeanCount;
    }
//  SIMPLE_SET_GET_METHOD(Integer, MeanCount);
  void setExposureTime( double &value ) // change code (2011/01/06)
    {
      ExposureTime = value;
    }
  const double getExposureTime() const
    {
      return ExposureTime;
    }
//  SIMPLE_SET_GET_METHOD(Real, ExposureTime);
  virtual void initializeFourth()
    {
      theLatticeSize = theSpatiocyteStepper->getLatticeSize();
      theStartCoord = theSpatiocyteStepper->getStartCoord();
      theProcessSpecies.resize(0);
      //Put all the unique negative species in the process species list:
      for(VariableReferenceVector::iterator 
          i(theNegativeVariableReferences.begin());
          i != theNegativeVariableReferences.end(); ++i)
        {
          Variable* aVariable((*i).getVariable());
          Species* aSpecies(theSpatiocyteStepper->getSpecies(aVariable));
          if(!aSpecies->getIsLipid() &&
             find(theProcessSpecies.begin(), theProcessSpecies.end(), 
                  aSpecies) == theProcessSpecies.end())
            {
              theProcessSpecies.push_back(aSpecies);
            }
        }
      VariableReferenceVector::iterator aNegativeVariableIter(
                                   theNegativeVariableReferences.begin());
      for(VariableReferenceVector::iterator
          i(thePositiveVariableReferences.begin());
          i != thePositiveVariableReferences.end(); ++i)
        {
          int aPositiveCoefficient((*i).getCoefficient());
          Variable* aVariable((*i).getVariable());
          Species* aSpecies(theSpatiocyteStepper->getSpecies(aVariable));
          if(!aSpecies->getIsLipid())
            {
              if(aPositiveCoefficient > 0)
                {
                  thePositiveSpecies.push_back(aSpecies);
                  vector<int> aProcessSpeciesIndices;
                  do
                    {
                      int aNegativeCoefficient(
                               (*aNegativeVariableIter).getCoefficient());
                      Variable* aNegativeVariable(
                                (*aNegativeVariableIter).getVariable());
                      Species* aNegativeSpecies(
                          theSpatiocyteStepper->getSpecies(aNegativeVariable));
                      int aProcessSpeciesIndex(
                       find(theProcessSpecies.begin(), theProcessSpecies.end(), 
                            aNegativeSpecies) - theProcessSpecies.begin());
                      while(aNegativeCoefficient)
                        {
                          aNegativeCoefficient += 1;
                          aPositiveCoefficient -= 1;
                          aProcessSpeciesIndices.push_back(
                                                   aProcessSpeciesIndex);
                        }
                      ++aNegativeVariableIter;
                    }
                  while(aPositiveCoefficient);
                  theProcessSpeciesIndices.push_back(aProcessSpeciesIndices);
                }
            }
        }
      theLattice.resize(theProcessSpecies.size());
      for(unsigned int i(0); i != theLattice.size(); ++i)
        {
          theLattice[i].resize(theLatticeSize);
        }
      resetLattice();
      if(MeanCount != -1)
        {
          theStepInterval = ExposureTime/MeanCount;
        }
      else
        {
          for(vector<Species*>::const_iterator i(thePositiveSpecies.begin());
              i != thePositiveSpecies.end(); ++i)
            {
              if((*i)->getDiffusionInterval() < theStepInterval)
                {
                  theStepInterval = (*i)->getDiffusionInterval();
                }
              Species* reactantPair((*i)->getDiffusionInfluencedReactantPair());
              if(reactantPair != NULL && 
                 reactantPair->getDiffusionInterval() < theStepInterval)
                {
                  theStepInterval = reactantPair->getDiffusionInterval();
                }
            }
          MeanCount = (int)ceil(ExposureTime/theStepInterval);
        }
      theMeanCount = (unsigned int)MeanCount;
      theTime = theStepInterval;
      theLastExposedTime = theTime;
      thePriorityQueue->move(theQueueID);
    }
  virtual void initializeLastOnce()
    {  
      ostringstream aFilename;
      aFilename << FileName << ends;
      theLogFile.open(aFilename.str().c_str(), ios::binary | ios::trunc);
      initializeLog();
      logSurfaceVoxels();
    }
  virtual void fire()
    {
      incSpeciesLatticeCount();
      if(theTime-theLastExposedTime >= ExposureTime)
        {
          theLastExposedTime = theTime;
          logFluorescentSpecies();
          resetLattice();
        }
      theTime += theStepInterval;
      thePriorityQueue->moveTop();
    }
protected:
  void incSpeciesLatticeCount();
  void logFluorescentSpecies();
  void resetLattice()
    {
      for(unsigned int i(0); i != theLattice.size(); ++i)
        {
          for(unsigned int j(0); j != theLatticeSize; ++j)
            {
              theLattice[i][j] = 0;
            }
        }
    }
protected:
  unsigned int theStartCoord;
  unsigned int theLatticeSize;
  int MeanCount;
  double ExposureTime;
  double theLastExposedTime;
  vector<Species*> thePositiveSpecies;
  vector<vector<int> > theLattice;
  vector<vector<int> > theProcessSpeciesIndices;
};

#endif /* __FluorescentProteinImagingProcess_hpp */
