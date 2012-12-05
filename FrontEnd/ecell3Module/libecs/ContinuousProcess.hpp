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
// modified for stand-alone (2011/02/03)

// change code (2010/12/28)
// #include "Defs.hpp" // comment out (2011/02/03)
#include "Process.hpp"
//#include "libecs/Defs.hpp"
//#include "libecs/Process.hpp"

//namespace libecs
//{

class ContinuousProcess : public Process // change code (2010/12/28)
{
//LIBECS_DM_CLASS( ContinuousProcess, Process )
//{

public:
// comment out (2010/12/28)
//    LIBECS_DM_OBJECT_ABSTRACT( ContinuousProcess )
//    {
//        INHERIT_PROPERTIES( Process );
//    }

    ContinuousProcess()
    {
        ; // do nothing
    }

    virtual ~ContinuousProcess()
    {
        ;
    }

    virtual bool isContinuous() const
    {
        return true;
    }
};

// } // namespace libecs // comment out (2010/12/28)
