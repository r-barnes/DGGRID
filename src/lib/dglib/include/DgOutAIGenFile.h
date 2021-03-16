/*******************************************************************************
    Copyright (C) 2021 Kevin Sahr

    This file is part of DGGRID.

    DGGRID is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DGGRID is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
//
// DgOutAIGenFile.h: DgOutAIGenFile class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGOUTAIGENFILE_H
#define DGOUTAIGENFILE_H

#include <cstdio>

#include "DgOutLocTextFile.h"

class DgDVec2D;
class DgPolygon;

////////////////////////////////////////////////////////////////////////////////
class DgOutAIGenFile : public DgOutLocTextFile {

   using DgOutLocFile::insert;

   public:

      DgOutAIGenFile (const DgRFBase& rfIn, const string& fileNameIn = "",
                   int precision = 7, bool isPointFile = false,
                   DgReportLevel failLevel = DgBase::Fatal);

      virtual ~DgOutAIGenFile (void) { if (good()) close(); }
      
      virtual void close (void) { *this << "END" << endl; ofstream::close(); }

      virtual DgOutLocFile& insert (DgLocation& loc, const string* label = NULL);
      virtual DgOutLocFile& insert (DgLocVector& vec, const string* label = NULL, 
                                const DgLocation* cent = NULL);
      virtual DgOutLocFile& insert (DgPolygon& poly, const string* label = NULL, 
                                const DgLocation* cent = NULL);

   protected:

      virtual void setFormatStr(void)
      {
          ostringstream os;
          os << "%#." << getPrecision() << "LF"
             << " %#." << getPrecision() << "LF"
             << '\n';

          formatStr_ = os.str();
      }

   private:

      virtual DgOutLocFile& insert (const DgDVec2D& pt);
};

////////////////////////////////////////////////////////////////////////////////

#endif
