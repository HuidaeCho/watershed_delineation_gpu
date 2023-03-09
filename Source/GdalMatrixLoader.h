#ifndef GDAL_MATRIX_LOADER_H
#define GDAL_MATRIX_LOADER_H


#include "Rasterinfo.h"
#include <gdal_priv.h>
#include <cpl_conv.h>


class GdalMatrixLoader
{
  public:
    template <class M, typename T> static M load(std::string filename, GDALDataType dataType, T noDataValue, RasterInfo& rasterInfo, int bandIndex = 1);
    template <class M> static void save(std::string filename, const M& matrix, GDALDataType dataType, RasterInfo& rasterInfo, int bandIndex = 1);
};


template <class M, typename T>
M GdalMatrixLoader::load(std::string filename, GDALDataType dataType, T noDataValue, RasterInfo& rasterInfo, int bandIndex)
{
  GDALAllRegister();

#ifdef USE_GDAL_C_API
  GDALDatasetH dataset = GDALOpen(filename.c_str(), GA_ReadOnly);
  rasterInfo.projection = GDALGetProjectionRef(dataset);
  GDALGetGeoTransform(dataset, rasterInfo.geotransform);

  GDALRasterBandH band = GDALGetRasterBand(dataset, bandIndex);

  const int height = GDALGetRasterYSize(dataset);
  const int width = GDALGetRasterXSize(dataset);

  M matrix(height, width);

  for (int i = 0; i < height; ++i)
  {
    CPLErr errorCode = GDALRasterIO(band, GF_Read, 0, i, width, 1, matrix.value[i + 1] + 1, width, 1, dataType, 0, 0);
  }

  rasterInfo.noDataValue = GDALGetRasterNoDataValue(band, &rasterInfo.hasNoDataValue);
#else
  GDALDataset* dataset = (GDALDataset*) GDALOpen(filename.c_str(), GA_ReadOnly);
  rasterInfo.projection = dataset->GetProjectionRef();
  dataset->GetGeoTransform(rasterInfo.geotransform);

  GDALRasterBand* band = dataset->GetRasterBand(bandIndex);

  const int height = band->GetYSize();
  const int width = band->GetXSize();

  M matrix(height, width);

  for (int i = 0; i < height; ++i)
  {
    CPLErr errorCode = band->RasterIO(GF_Read, 0, i, width, 1, matrix.value[i + 1] + 1, width, 1, dataType, 0, 0);
  }

  rasterInfo.noDataValue = band->GetNoDataValue(&rasterInfo.hasNoDataValue);
#endif

  if ((rasterInfo.hasNoDataValue == 1) && (rasterInfo.noDataValue != noDataValue))
  {
    for (int row = 1; row <= height; ++row)
    {
      for (int col = 1; col <= width; ++col)
      {
        if (matrix.value[row][col] == rasterInfo.noDataValue)
        {
          matrix.value[row][col] = noDataValue;
        }
      }
    }
  }

  GDALClose(dataset);
  return matrix;
}


template <class M>
void GdalMatrixLoader::save(std::string filename, const M& matrix, GDALDataType dataType, RasterInfo& rasterInfo, int bandIndex)
{
  GDALAllRegister();
#ifdef USE_GDAL_C_API
  GDALDriverH driver = GDALGetDriverByName("GTiff");

  const int height = matrix.height;
  const int width = matrix.width;

  GDALDatasetH dataset = GDALCreate(driver, filename.c_str(), width, height, 1, dataType, NULL);
  GDALSetProjection(dataset, rasterInfo.projection.c_str());
  GDALSetGeoTransform(dataset, rasterInfo.geotransform);

  GDALRasterBandH band = GDALGetRasterBand(dataset, bandIndex);
  if (rasterInfo.hasNoDataValue)
  {
      GDALSetRasterNoDataValue(band, rasterInfo.noDataValue);
  }

  for (int i = 0; i < height; ++i)
  {
    CPLErr errorCode = GDALRasterIO(band, GF_Write, 0, i, width, 1, matrix.value[i + 1] + 1, width, 1, dataType, 0, 0);
  }
#else
  GDALDriver* driver = GetGDALDriverManager()->GetDriverByName("GTiff");

  const int height = matrix.height;
  const int width = matrix.width;

  GDALDataset* dataset = driver->Create(filename.c_str(), width, height, 1, dataType, NULL);
  dataset->SetProjection(rasterInfo.projection.c_str());
  dataset->SetGeoTransform(rasterInfo.geotransform);

  GDALRasterBand* band = dataset->GetRasterBand(bandIndex);
  if (rasterInfo.hasNoDataValue)
  {
      band->SetNoDataValue(rasterInfo.noDataValue);
  }

  for (int i = 0; i < height; ++i)
  {
    CPLErr errorCode = band->RasterIO(GF_Write, 0, i, width, 1, matrix.value[i + 1] + 1, width, 1, dataType, 0, 0);
  }
#endif

  GDALClose(dataset);
}


#endif
