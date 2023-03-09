#include "BasinIndexLoader.h"


BasinIndexMatrix BasinIndexLoader::loadGdal(std::string filename, RasterInfo& rasterInfo, int bandIndex)
{
  return GdalMatrixLoader::load<BasinIndexMatrix, unsigned char>(filename, GDT_Byte, BASIN_NONE, rasterInfo, bandIndex);
}


void BasinIndexLoader::saveGdal(std::string filename, const BasinIndexMatrix& basinIndexMatrix, RasterInfo& rasterInfo, int bandIndex)
{
  GdalMatrixLoader::save<BasinIndexMatrix>(filename, basinIndexMatrix, GDT_Byte, rasterInfo, bandIndex);
}
