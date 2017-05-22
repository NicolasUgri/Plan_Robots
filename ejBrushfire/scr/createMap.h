// #include <libplayerc++/playerc++.h>

#include <player-3.0/libplayerc++/playerc++.h>
#include <iostream>
#include <string>


class obstacleMap 
{
 public:

  obstacleMap():width_(0),height_(0),resolution_(0),map_(0){}

  int** getMap(playerc_client_t*);
  
  int width();

  int height();

  int** map();

  int size();
  
  double resolution();

  ~obstacleMap();

 private:
  int** map_;

  int width_;

  int height_;

  double resolution_;

};
