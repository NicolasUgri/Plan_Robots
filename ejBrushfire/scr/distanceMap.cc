#include "distanceMap.h"

#include <cmath>
#include <iostream>
#include <string>
#include <stdio.h>


double** distanceMap::generate(int x, int y,double resolution, int width, int height)
{
  width_=width;
  height_=height;
  
  map_ = new double*[height_];
  
  for(int i=0;i<height_;i++)
    {
      map_[i]=new double[width_];
    }
  
  for(int i=0;i<height_;i++)
    {
      double ydist=resolution*fabs(y-i);

      for(int j=0;j<width_;j++)
	{
	  double xdist=resolution*fabs(x-j);

	  map_[i][j]=sqrt(pow(xdist,2)+pow(ydist,2));
	}

    }

  return map_;
}

double** distanceMap::map()
{
  return map_;
}

int distanceMap::width()
{
  return width_;
}

int distanceMap::height()
{
  return height_;
}

distanceMap::~distanceMap()
{
  for(int i=0;i<height_;i++)
    delete map_[i];

  delete [] map_;
}
