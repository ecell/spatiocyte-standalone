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
// modified for stand-alone (2011/01/17)
// Integer -> long int (2011/01/17)

#include "MassActionProcess.hpp"
#include "SpatiocyteSpecies.hpp"

// LIBECS_DM_INIT(MassActionProcess, Process); // comment out (2010/12/28)

void MassActionProcess::fire()
{ 
  if(theSpace == 0)
    {
      Species* aSpecies(NULL);
      for(VariableReferenceVector::iterator
          i(theVariableReferenceVector.begin());
          i != theVariableReferenceVector.end(); ++i)
        {
          Variable* aVariable((*i).getVariable()); 
          aSpecies = theSpatiocyteStepper->getSpecies(aVariable);
          if(aSpecies != NULL)
            {
              break;
            }
        }
      if(aSpecies->getIsVolume())
        {
          theSpace = aSpecies->getCompartment()->actualVolume;
          cout << "Mass Action Volume:" << theSpace << endl;
        }
      else
        {
          theSpace = aSpecies->getCompartment()->actualArea;
          cout << "Mass Action Area:" << theSpace << endl;
        }
    }
  double velocity(k);
  velocity *= theSpace;
  for(VariableReferenceVector::iterator
      s(theVariableReferenceVector.begin());
      s != theZeroVariableReferenceIterator; ++s)
    {
      VariableReference aVariableReference(*s);
      long int aCoefficient(aVariableReference.getCoefficient());
      do
        {
          ++aCoefficient;
          velocity *= aVariableReference.getVariable()->getValue()/theSpace;
        }
      while(aCoefficient != 0); 
    } 
  setFlux(velocity);
}

