#include "createMap.h"

class brushfireMap
{
 public:
  
  brushfireMap():width_(0),height_(0),map_(0){}

  double** generate(obstacleMap& map);

  double** map();

  int width();

  int height();

  ~brushfireMap();

 private:

  void updateCellsAround(double value,int i, int j);

  int width_;

  int height_;

  double** map_;
};
