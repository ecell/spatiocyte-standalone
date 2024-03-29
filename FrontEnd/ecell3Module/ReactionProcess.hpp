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
// Integer -> long int (2011/01/17)
// Real -> double (2011/01/17)
// Time -> double (2011/02/03)

#ifndef __ReactionProcess_hpp
#define __ReactionProcess_hpp

#include "SpatiocyteProcess.hpp"

class ReactionProcess : public SpatiocyteProcess //change code (2010/12/22)
{
// LIBECS_DM_CLASS(ReactionProcess, SpatiocyteProcess)
// { 
public:
//   LIBECS_DM_OBJECT(ReactionProcess, Process)
//     {
//       INHERIT_PROPERTIES(SpatiocyteProcess);
//       PROPERTYSLOT_SET_GET(Real, k);
//       PROPERTYSLOT_SET_GET(Real, p);
//       PROPERTYSLOT_GET_NO_LOAD_SAVE(Integer, Order);
//     }
  ReactionProcess():
    k(-1),
    p(-1),
    theOrder(0),
    A(NULL),
    B(NULL),
    C(NULL),
    D(NULL),
    E(NULL),
    variableA(NULL),
    variableB(NULL),
    variableC(NULL),
    variableD(NULL), 
    variableE(NULL) {}
  virtual ~ReactionProcess() {}
  void setk( double &value ) // change code (2011/01/06)
    {
      k = value;
    }
  const double getk() const
    {
      return k;
    }
//  SIMPLE_SET_GET_METHOD(Real, k);
  void setp( double &value ) // change code (2011/01/06)
    {
      p = value;
    }
  const double getp() const
    {
      return p;
    }
//  SIMPLE_SET_GET_METHOD(Real, p);
  virtual bool isInterrupting(Process*);
  virtual void fire()
    {
      const double aCurrentTime(theTime); // do this only for the Processes in Q
      requeue();
      for(vector<ReactionProcess*>::const_iterator 
          i(theInterruptingProcesses.begin());
          i!=theInterruptingProcesses.end(); ++i)
        {
          (*i)->substrateValueChanged(aCurrentTime);
        }
    }
  void requeue()
    {
      theTime += getStepInterval(); // do this only for the Processes in Q
      thePriorityQueue->moveTop(); // do this only for the Processes in Q
    }
  const long int getOrder() const // change code (2011/01/06)
//   GET_METHOD(Integer, Order)
     {
       return theOrder;
     }
  virtual void initialize()
    {
      if(isInitialized)
        {
          return;
        }
      SpatiocyteProcess::initialize();
      declareUnidirectional();
      calculateOrder();
    }
  virtual void initializeSecond()
    {
      SpatiocyteProcess::initializeSecond();
      theInterruptingProcesses.resize(0);
    }
  virtual void initializeThird()
    {
      SpatiocyteProcess::initializeThird();
    }
  //This method is called whenever the substrate (-1 and 0 coefficients)
  //value of the process listed in isInterrupting method has changed:
  virtual void substrateValueChanged(double aCurrentTime)
    {
      const double anOldTime(theTime);
      theTime = aCurrentTime + getStepInterval();
      if(theTime >= anOldTime)
        {
          thePriorityQueue->moveDown(theQueueID);
        }
      else
        {
          thePriorityQueue->moveUp(theQueueID);
        }          
    }
  void setInterrupt(vector<Process*> &aProcessList, Process* aProcess)
    {
      for(vector<Process*>::const_iterator i(aProcessList.begin());
          i != aProcessList.end(); ++i)
        {
          if(aProcess != (*i) && isInterrupting(*i))
            {
              theInterruptingProcesses.push_back(
                                     reinterpret_cast<ReactionProcess*>(*i));

            }
        }
    }
  const Species* getA() const
    {
      return A;
    }
  const Species* getB() const
    {
      return B;
    }
  const Species* getC() const
    {
      return C;
    }
  const Species* getD() const
    {
      return D;
    }
  const Species* getE() const
    {
      return E;
    }
  virtual void addSubstrateInterrupt(Species* aSpecies, Voxel* aMolecule) {}
  virtual void removeSubstrateInterrupt(Species* aSpecies, Voxel* aMolecule) {}
protected:
  virtual void calculateOrder();
protected:
  double k;
  double p;
  int theOrder;
  //Species are for non HD species:
  Species* A;
  Species* B;
  Species* C;
  Species* D;
  Species* E;
  //Variables are for HD species:
  Variable* variableA;
  Variable* variableB;
  Variable* variableC;
  Variable* variableD;
  Variable* variableE;
  vector<ReactionProcess*> theInterruptingProcesses;
};

inline void ReactionProcess::calculateOrder()
{ 
  A = NULL;
  B = NULL;
  C = NULL;
  D = NULL;
  E = NULL;
  variableA = NULL;
  variableB = NULL;
  variableC = NULL;
  variableD = NULL;
  variableE = NULL;
  theOrder = 0;
  for(VariableReferenceVector::iterator 
      i(theSortedVariableReferences.begin());
      i != theSortedVariableReferences.end(); ++i)
    {
      const int aCoefficient((*i).getCoefficient());
      Variable* aVariable((*i).getVariable());
      if(aCoefficient < 0)
        {
          theOrder -= aCoefficient; 
          //The first reactant, A:
          if(A == NULL && variableA == NULL)
            {
              if(aVariable->getName() == "HD")
                {
                  variableA = aVariable;
                }
              else
                {
                  A = theSpatiocyteStepper->getSpecies(aVariable);
                }
            }
          //The second reactant, B:
          else
            {
              if(aVariable->getName() == "HD")
                {
                  variableB = aVariable;
                }
              else
                {
                  B = theSpatiocyteStepper->getSpecies(aVariable);
                }
            }
        }
      else if(aCoefficient > 0)
        {
          //The first product, C:
          if(C == NULL && variableC == NULL)
            {
              if(aVariable->getName() == "HD")
                {
                  variableC = aVariable;
                }
              else
                {
                  C = theSpatiocyteStepper->getSpecies(aVariable);
                }
            }
          //The second product, D:
          else if(D == NULL && variableD == NULL)
            {
              if(aVariable->getName() == "HD")
                {
                  variableD = aVariable;
                }
              else
                {
                  D = theSpatiocyteStepper->getSpecies(aVariable);
                }
            }
          //The third product, E:
          else
            {
              if(aVariable->getName() == "HD")
                {
                  variableE = aVariable;
                }
              else
                {
                  E = theSpatiocyteStepper->getSpecies(aVariable);
                }
            }
        }
    }
} 

// change code (2011/01/17)
inline bool ReactionProcess::isInterrupting(Process* aProcess)
// bool ReactionProcess::isInterrupting(Process* aProcess)
{
  //List all the processes here that need to be notified when their
  //substrateValueChanged:
  // temporary change code( 2011/01/26)
  if(aProcess->getProcessName() ==
//  if(aProcess->getPropertyInterface().getClassName() ==
     "SpatiocyteNextReactionProcess") 
    {
      //First get the unique variable pointers of this process:
      vector<Variable*> aVariableList;
      for(VariableReferenceVector::iterator
          i(theVariableReferenceVector.begin());
          i != theVariableReferenceVector.end(); ++i)
        {
          vector<Variable*>::const_iterator j(aVariableList.begin());
          while(j!=aVariableList.end())
            {
              if((*i).getVariable() == (*j))
                {
                  break;
                }
              ++j;
            }
          if(j == aVariableList.end())
            {
              aVariableList.push_back((*i).getVariable());
            }
        }
      //Find out if the values of the unique variables will be changed
      //by this process, i.e, netCoefficient != 0:
      vector<int> aNetCoefficientList;
      aNetCoefficientList.resize(aVariableList.size());
      for(vector<int>::iterator i(aNetCoefficientList.begin());
          i!=aNetCoefficientList.end(); ++i)
        {
          (*i) = 0;
        }
      for(VariableReferenceVector::iterator
          i(theVariableReferenceVector.begin());
          i != theVariableReferenceVector.end(); ++i)
        {
          for(vector<Variable*>::const_iterator j(aVariableList.begin());
              j!=aVariableList.end(); ++j)
            {
              if((*i).getVariable() == (*j))
                {
                  aNetCoefficientList[j-aVariableList.begin()] +=
                    (*i).getCoefficient();
                }
            }
        }
      //Check if any variable with netCoefficient != 0 is a substrate
      //of aProcess:
      VariableReferenceVector
        aVariableReferenceVector(aProcess->getVariableReferenceVector()); 
      for(VariableReferenceVector::iterator
          i(aVariableReferenceVector.begin());
          i != aVariableReferenceVector.end(); ++i)
        {
          if((*i).isAccessor())
            {
              for(vector<Variable*>::const_iterator j(aVariableList.begin());
                  j!=aVariableList.end(); ++j)
                {
                  if((*i).getVariable() == (*j) && 
                     aNetCoefficientList[j-aVariableList.begin()])
                    {
                      return true;
                    }
                }
            }
        }
    }
  return false;
}


#endif /* __ReactionProcess_hpp */
