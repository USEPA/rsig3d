/******************************************************************************
PURPOSE: LookupTable.h - defines LookupTable class

NOTES:

HISTORY: 2012/11/11, zhang.nan@epa.gov, Created.
******************************************************************************/

#ifndef INC_LOOKUPTABLE_H_
#define INC_LOOKUPTABLE_H_

#if _MSC_VER
#include <windows.h>
#endif

#include <vector>

#include <GL/glew.h>

#include <QObject>

#include <vector4f.h>


class LookupTable {

private:
  bool nonlinearFlag;
  bool strictFlag; // do not mess with color scaling (e.g. for AQI colormaps)
  int colormapIndex; // 0-based index of RGBColormap for this object.

public:
  LookupTable();

  ~LookupTable();

  void setMattLookupTable(int, float, float); // Also sets colormapIndex.
  int getColormapIndex() const { return colormapIndex; }
  void setNonlinearLookupTable(const unsigned char r[],
                               const unsigned char g[],
                               const unsigned char b[],
                               const float values[], int len_rgb);
  void adjustRangeNonlinear(float values[]);
  bool getNonlinearFlag() const;
  void setNonlinearFlag(bool x);
  bool getStrictFlag() const;
  void setStrictFlag(bool x);
  const std::vector<float>& getValues() const;
  void setDefaultLookupTable();
  void setNasaCloudAerosol();
  void setNasaDepolarization( void );

  //Note: t is a normalized value [0..1], no color interpolation
  Vector4f colorByParameter(float t) const;

  //Note: x is unnormalized value, no color interpolation
  Vector4f colorByValue(float t) const;

  void setLinearLookupTable(const unsigned char r[],
                            const unsigned char g[],
                            const unsigned char b[],
                            const float values[], int len_rgb);

  std::vector<Vector4f>& colorPalette();

  GLuint texureID() { return mTexID; }

  //use new range to linearly scale the old range
  virtual void adjustRange(float minval, float maxval);

  //generate the texture in GPU, load texture into GPU memory
  virtual GLuint genTexture();
  virtual void updateTexture();

protected:
  //rebuild the palette using interpolation / no interpolation
  void rebuildPalette(bool interpolation);

  //discrete input of color points
  std::vector<Vector4f> mColors;

  //discrete input of values, should be same size of colors
  std::vector<float> mValues;

  //a linear lookup table created, based on the color and scalar values
  std::vector<Vector4f> mPalette;

  GLuint mTexID;
};


Vector4f interpolate_list(float t, const std::vector<Vector4f>& color,
                          const std::vector<float>& x, bool interpolationFlag);

float interpolate_list(float t, const std::vector<float>& color,
                       const std::vector<float>& x, bool interpolationFlag);


#endif  //INC_LOOKUPTABLE_H_

