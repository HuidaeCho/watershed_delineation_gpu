#include "FlowDirectionLoader.h"


FlowDirectionMatrix FlowDirectionLoader::loadGdal(std::string filename, RasterInfo& rasterInfo, int bandIndex)
{
  return GdalMatrixLoader::load<FlowDirectionMatrix, unsigned char>(filename, GDT_Byte, DIRECTION_NONE, rasterInfo, bandIndex);
}


void FlowDirectionLoader::saveGdal(std::string filename, const FlowDirectionMatrix& directionMatrix, RasterInfo& rasterInfo, int bandIndex)
{
  GdalMatrixLoader::save<FlowDirectionMatrix>(filename, directionMatrix, GDT_Byte, rasterInfo, bandIndex);
}
