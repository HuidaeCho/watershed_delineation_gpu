#ifndef RASTER_INFO_H
#define RASTER_INFO_H


#include <string>


struct RasterInfo
{
public:
    std::string projection;
    double geotransform[6];
    int hasNoDataValue;
    double noDataValue;
};


#endif
