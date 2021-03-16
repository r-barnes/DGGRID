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
// DgOutLocFile.cpp: DgOutLocFile class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include <list>
#include <sstream>

#include "DgOutLocFile.h"
#include "DgLocList.h"
#include "DgCell.h"
#include "DgPolygon.h"
#include "DgLocation.h"
#include "DgOutAIGenFile.h"
#include "DgOutGdalFile.h"
#include "DgOutKMLfile.h"
#include "DgOutGeoJSONFile.h"
#include "DgOutShapefile.h"
#include "DgOutPtsText.h"
#include "DgOutPRCellsFile.h"
#include "DgOutPRPtsFile.h"
#include "DgGeoSphRF.h"

const string DgOutLocFile::defaultKMLColor = "ffffffff";
const int    DgOutLocFile::defaultKMLWidth = 4;
const string DgOutLocFile::defaultKMLName = "";
const string DgOutLocFile::defaultKMLDescription = "Generated by DGGRID " DGGRID_VERSION;

////////////////////////////////////////////////////////////////////////////////
DgOutLocFile::~DgOutLocFile (void) 
{ 
   // empty destructor
}

////////////////////////////////////////////////////////////////////////////////
DgOutLocFile* 
DgOutLocFile::makeOutLocFile (const string& type, const string& fileName, 
                    const string& gdalDriver,
                    const DgRFBase& rf, bool isPointFile, int precision,
                    int shapefileIdLen, const string& kmlColor, int kmlWidth,
                    const string& kmlName, const string& kmlDesc,
                    DgReportLevel failLevelIn)
{
   DgOutLocFile* file = NULL;
   if (!type.compare("AIGEN"))
      file = new DgOutAIGenFile(rf, fileName, precision, isPointFile, 
                                 failLevelIn);
   else if (!type.compare("TEXT"))
      file = new DgOutPtsText(rf, fileName, precision, failLevelIn);
   else // must be KML, GEOJSON, SHAPEFILE, or GDAL
   {
      const DgGeoSphDegRF* geoRF = dynamic_cast<const DgGeoSphDegRF*>(&rf);
      if (geoRF == NULL)
         ::report("DgOutLocFile::makeOutLoc(): invalid RF type", failLevelIn);

      if (!type.compare("KML"))
         file = new DgOutKMLfile(*geoRF, fileName, precision, isPointFile, 
                                 kmlColor, kmlWidth, kmlName, kmlDesc, failLevelIn);
      else if (!type.compare("GEOJSON"))
         file = new DgOutGeoJSONFile(*geoRF, fileName, precision, isPointFile,
                                     failLevelIn);
      else if (!type.compare("SHAPEFILE"))
         file = new DgOutShapefile(*geoRF, fileName, precision, isPointFile, 
                                    shapefileIdLen, failLevelIn);
// USE_GDAL is set in MakeIncludes
#ifdef USE_GDAL
      else if (!type.compare("GDAL"))
         file = new DgOutGdalFile(*geoRF, fileName, gdalDriver, precision, isPointFile, failLevelIn);
#endif
      else if (type.compare("NONE"))
         ::report("DgOutLocFile::makeOutLoc(): invalid file type " + type, 
                                 failLevelIn);
   }

   return file;

} // DgOutLocFile::makeOutLoc

////////////////////////////////////////////////////////////////////////////////
DgOutLocFile&
DgOutLocFile::insert (DgLocList& dlist)
//
// Output the drawlist dlist.
//
////////////////////////////////////////////////////////////////////////////////
{
   list<DgLocBase*>::iterator it;
   for (it = dlist.begin(); it != dlist.end(); it++)
   {
      if (DgLocList* d = dynamic_cast<DgLocList*>(*it))
      {
         this->insert(*d);
      }
      else if (DgPolygon* d = dynamic_cast<DgPolygon*>(*it))
      {
         this->insert(*d);
      }
      else if (DgCell* d = dynamic_cast<DgCell*>(*it))
      {
         this->insert(*d);
      }
      else if (DgLocation* d = dynamic_cast<DgLocation*>(*it))
      {
         this->insert(*d);
      }
      else if (DgLocVector* d = dynamic_cast<DgLocVector*>(*it))
      {
         this->insert(*d);
      }
      else
      {
         report("DgOutLocFile::insert() invalid location not inserted", Warning);
      }
   }

   //this->flush();

   return *this;

} // DgOutLocFile& DgOutLocFile::insert

////////////////////////////////////////////////////////////////////////////////
DgOutLocFile&
DgOutLocFile::insert (DgCell& cell)
//
// Put the cell.
//
////////////////////////////////////////////////////////////////////////////////
{
   rf().convert(&cell);

   if (!isPointFile() && cell.hasRegion())
      this->insert(cell.region(), &(cell.label()), &(cell.node()));
   else
      this->insert(cell.node(), &(cell.label()));

   return *this;

} // DgOutLocFile& DgOutLocFile::insert


